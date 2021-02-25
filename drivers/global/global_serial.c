/**
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK:
 * https://www.we-online.com/wireless-connectivity
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2021 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "string.h"
#include <stdio.h>

#include "../../drivers/WE-common.h"
#include "global.h"


#ifndef _global_serial_defined
#define _global_serial_defined

#ifdef _global_serialWiringPi_defined
#error "interface already defined"
#elifdef _global_ftdi_defined
#error "interface already defined"
#endif

/**************************************
 *     Static function declarations   *
 **************************************/

static bool ConfigureBaudrate(struct termios *serialConfigurationP, int baudrate);
static bool ConfigureParity(struct termios *serialConfigurationP, Serial_ParityBit_t parity);
static bool SetPortAttributes(int serial_handle, int baudrate, Serial_ParityBit_t parity);

/**************************************
 *          Static variables          *
 **************************************/
int serial_handle = -1;
const char *serial_device = "/dev/serial0";

/**************************************
 *         Global functions           *
 **************************************/

bool SendBytes(uint8_t* dataP, uint16_t length)
{
    uint16_t bytesWritten = 0;

    if(-1 == serial_handle)
    {
        /* invalid serial_handle*/
        return false;
    }

    if(NULL == dataP)
    {
        return false;
    }

    bytesWritten = write(serial_handle, dataP, length);
    if (bytesWritten != length)
    {
        Debug_out("SendBytes: Could not write bytes.", false);
        printf("Error when sending bytes: %s\n", strerror(errno));
        return false;
    }

    return true;
}

bool BytesAvailable()
{
    int bytesInBuffer = 0;

    if(-1 == serial_handle)
    {
        /* invalid handle */
        return false;
    }

    ioctl(serial_handle, FIONREAD, &bytesInBuffer);

    return (0 < bytesInBuffer);
}

bool ReadByte(uint8_t *readBufferP)
{
    return ReadBytesN(readBufferP,1);
}

bool ReadBytesN(uint8_t *dataP, uint16_t length)
{
    int bytesRead = 0;

    if(-1 == serial_handle)
    {
        return false;
    }

    if(NULL == dataP)
    {
        return false;
    }

    bytesRead = read(serial_handle, dataP, length);

    /* no bytes were read */
    if(bytesRead != length)
    {
        Debug_out("ReadBytesN: Could not read bytes", false);
        printf("Error when reading bytes: %s\n", strerror(errno));
        return false;
    }

    return true;
}

bool CloseSerial()
{
    if(-1 == serial_handle)
    {
        /* already closed */
        return true;
    }

    if(0 != close(serial_handle))
    {
        Debug_out("CloseSerial: Could not close serial", false);
        printf("Error when closing serial: %s\n", strerror(errno));
        return false;
    }

    serial_handle = -1;

    return true;
}

bool FlushSerial()
{
    if(-1 == serial_handle)
    {
        return false;
    }

    /* flush input and output */
    if(0 != tcflush(serial_handle, TCIOFLUSH))
    {
        Debug_out("FlushSerial: Could not flush serial", false);
        printf("Error when flushing serial: %s \n", strerror(errno));
        return false;
    }

    return true;
}

bool InitSerial()
{
    return true;
}

bool DeinitSerial()
{
    return true;
}

bool OpenSerial(int baudrate)
{
    /* 8n1 as default configuration*/
    return OpenSerialWithParity(baudrate, Serial_ParityBit_NONE);
}

bool OpenSerialWithParity(int baudrate, Serial_ParityBit_t parityBit)
{
    /* Open serial with read/write, no controlling tty and
     * sync to only return when entire data was written */
    serial_handle = open(serial_device, O_RDWR | O_NOCTTY | O_SYNC);
    if(serial_handle < 0)
    {
        Debug_out("OpenSerialWithParity: Could not open serial device.", false);
        printf("Failed with error: %s\n", strerror(errno));
        return false;
    }

    if(false == SetPortAttributes(serial_handle, baudrate, parityBit))
    {
        CloseSerial();
        Debug_out("OpenSerialWithParity: Could not configure serial.", false);
        return false;
    }

    return true;
}

/**************************************
 *         Static functions           *
 **************************************/

/**
* @brief  Configures the baudrate.
* @param  serialConfigurationP : termios configuration to write
* @param  baudrate             : baudrate to use
* @retval true on success, false otherwise
*/
static bool ConfigureBaudrate(struct termios *serialConfigurationP, int baudrate)
{
    int baudrateToSet = -1;

    if(NULL == serialConfigurationP)
    {
        return false;
    }

    switch(baudrate)
    {
        case 1200:
        {
            baudrateToSet = B1200;
            break;
        }

        case 2400:
        {
            baudrateToSet = B2400;
            break;
        }

        case 4800:
        {
            baudrateToSet = B4800;
            break;
        }

        case 9600:
        {
            baudrateToSet = B9600;
            break;
        }

        case 19200:
        {
            baudrateToSet = B19200;
            break;
        }

        case 38400:
        {
            baudrateToSet = B38400;
            break;
        }

        case 57600:
        {
            baudrateToSet = B57600;
            break;
        }

        case 115200:
        {
            baudrateToSet = B115200;
            break;
        }

        case 230400:
        {
            baudrateToSet = B230400;
            break;
        }

        case 460800:
        {
            baudrateToSet = B460800;
            break;
        }

        case 921600:
        {
            baudrateToSet = B921600;
            break;
        }

        case 1000000:
        {
            baudrateToSet = B1000000;
            break;
        }

        default:
        {
            /* invalid baudrate */
            Debug_out("ConfigureBaudrate: Invalid baudrate", false);
            return false;
        }
    }

    if(0 != cfsetospeed(serialConfigurationP, baudrateToSet))
    {
        Debug_out("ConfigureBaudrate: Could not set out baudrate", false);
        printf("Error when setting out baudrate: %s\n", strerror(errno));
        return false;
    }

    if(0 != cfsetispeed(serialConfigurationP, baudrateToSet))
    {
        Debug_out("ConfigureBaudrate: Could not set in baudrate", false);
        printf("Error when setting in baudrate: %s\n", strerror(errno));
        return false;
    }

    return true;
}

/**
* @brief  Configures the parity bit according to parameter. Will set 8 bit/1 stop bit fix.
* @param  serialConfigurationP : termios configuration to write
* @param  parity               : parity bit to use
* @retval true on success, false otherwise
*/
static bool ConfigureParity(struct termios *serialConfigurationP, Serial_ParityBit_t parity)
{
    if(NULL == serialConfigurationP)
    {
        return false;
    }

    switch(parity)
    {
        /* 8n1 */
        case Serial_ParityBit_NONE:
        {
            serialConfigurationP->c_cflag &= ~PARENB; /* no parity bit */
            serialConfigurationP->c_cflag &= ~CSTOPB; /* 1 stop bit */
            serialConfigurationP->c_cflag &= ~CSIZE;  /* clear size mask */
            serialConfigurationP->c_cflag |= CS8;     /* Set size mask to 8 bit */
            break;
        }

        /* 8e1 */
        case Serial_ParityBit_EVEN:
        {
            serialConfigurationP->c_cflag |= PARENB;  /* use parity bit */
            serialConfigurationP->c_cflag &= ~PARODD; /* even parity bit */
            serialConfigurationP->c_cflag &= ~CSTOPB; /* 1 stop bit */
            serialConfigurationP->c_cflag &= ~CSIZE;  /* clear size mask */
            serialConfigurationP->c_cflag |= CS8;     /* Set size mask to 8 bit */
            break;
        }

        /* 8o1 */
        case Serial_ParityBit_ODD:
        {
            serialConfigurationP->c_cflag |= PARENB;  /* use parity bit */
            serialConfigurationP->c_cflag |= PARODD;  /* odd parity bit*/
            serialConfigurationP->c_cflag &= ~CSTOPB; /* 1 stop bit */
            serialConfigurationP->c_cflag &= ~CSIZE;  /* clear size mask */
            serialConfigurationP->c_cflag |= CS8;     /* Set size mask to 8 bit */
            break;
        }

        default:
        {
            Debug_out("ConfigureParity: Invalid parity", false);
            return false;
        }
    }

    return true;
}

/**
* @brief  Configures the UART to non-canonical mode. Will set 8 bit/1 stop bit fix.
* @param  serial_handle : (already opened) uart handle to configure
* @param  baudrate      : baudrate to use
* @param  parity        : parity configuration to use
* @retval true on success, false otherwise
*/
static bool SetPortAttributes(int serial_handle, int baudrate, Serial_ParityBit_t parity)
{
    if(-1 == serial_handle)
    {
        /* serial_handle not open */
        return false;
    }

    struct termios tty;
    /* get current attributes/configuration*/
    if (tcgetattr (serial_handle, &tty) != 0)
    {
        Debug_out("SetPortAttributes: Could not get current settings", false);
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return false;
    }

    if(false == ConfigureBaudrate(&tty, baudrate))
    {
        /* invalid baudrate  */
        Debug_out("SetPortAttributes:Could not set baudrate", false);
        return false;
    }

    if(false == ConfigureParity(&tty, parity))
    {
        /* invalid parity*/
        Debug_out("SetPortAttributes:Could not set parity bit", false);
        return false;
    }

    /* See man page of termios for more information aboud the specific settings */
    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CRTSCTS;            /* no hardware flowcontrol */

    /* setup for non-canonical mode - characters will be send immediatly when typed */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN); /* raw input - received as is*/
    tty.c_oflag &= ~OPOST; /* raw output - transmitted as is */

    /* Set attributes/configurations*/
    if (tcsetattr (serial_handle, TCSANOW, &tty) != 0)
    {
        Debug_out("SetPortAttributes: Could not set new settings", false);
        printf("Error from tcsetattr: %s", strerror(errno));
        return false;
    }

    return true;
}

#endif // _global_serial_defined
