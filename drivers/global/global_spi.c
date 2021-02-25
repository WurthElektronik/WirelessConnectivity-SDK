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

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "linux/spi/spidev.h"
#include "global.h"

#ifndef _global_spi_defined
#define _global_spi_defined

#elif defined _global_serial_defined
#error "interface already defined"

#elif defined _global_ftdi_defined
#error "interface already defined"
#endif

/**************************************
 *          Static variables          *
 **************************************/
static int spi_handle = 0; // global handle to the  i2c interface

static int spi_bitsPerWords = 8;
static int spi_speed  = 0;

/**************************************
 *     Static function declarations   *
 **************************************/
static bool SPI_Init(int channel, int speed, int mode);
static bool SPI_ReadWrite(uint8_t *dataReadP, uint8_t *dataWriteP, int numByte);

/**************************************
 *         Static functions           *
 **************************************/
/**
* @brief  Initialize the SPI Interface
* @param  channel
* @param  speed
* @param  mode
* @retval true on success, false otherwise
*/
static bool SPI_Init(int channel, int speed, int mode)
{
    char spiDevice[32];
    snprintf (spiDevice, 31, "/dev/spidev0.%d", channel) ;

    if((spi_handle = open(spiDevice, O_RDWR)) < 0)
    {
        Debug_out("Could not open spi device", false);
        return false;
    }

    spi_speed = speed;

    if(ioctl(spi_handle, SPI_IOC_WR_MODE, &mode) < 0)
    {
        Debug_out("Could not set spi mode", false);
        return false;
    }
    if(ioctl(spi_handle, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWords) < 0)
    {
        Debug_out("Could not set spi bitsPerWords", false);
        return false;
    }
    if(ioctl(spi_handle, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0)
    {
        Debug_out("Could not set spi speed", false);
        return false;
    }

    return true;
}

/**
* @brief  Read or Write data
* @param  dataReadP: Data buffer to read into
* @param  dataWriteP: Data buffer to write
* @param  numByte: Number of bytes to read/write
* @retval true on success, false otherwise
*/
static bool SPI_ReadWrite(uint8_t *dataReadP, uint8_t *dataWriteP, int numByte)
{
    if(spi_handle < 0)
    {
        Debug_out("SPI read/write: Invalid spi handle", false);
        return false;
    }

    struct spi_ioc_transfer spi;

    memset (&spi, 0, sizeof (spi)) ;

    spi.tx_buf        = (unsigned long)dataWriteP;
    spi.rx_buf        = (unsigned long)dataReadP;
    spi.len           = numByte;
    spi.speed_hz      = spi_speed;
    spi.bits_per_word = spi_bitsPerWords ;

    if(ioctl(spi_handle, SPI_IOC_MESSAGE(1), &spi) < 0)
    {
        Debug_out("Could not read/write SPI data", false);
        return false;
    }

    return true;
}

bool OpenSPI(int datarate, Serial_Mode_t mode)
{
    int spiMode;
    switch(mode)
    {
        case Serial_Mode_0:
        {
            spiMode = SPI_MODE_0;
            break;
        }
        case Serial_Mode_1:
        {
            spiMode = SPI_MODE_1;
            break;
        }
        case Serial_Mode_2:
        {
            spiMode = SPI_MODE_2;
            break;
        }
        case Serial_Mode_3:
        {
            spiMode = SPI_MODE_3;
            break;
        }
        default:
            return false;
    }

    /* Chip select will be handled manually */
    mode |= SPI_NO_CS;

    return SPI_Init(0, datarate, spiMode);
}

bool CloseSerial()
{
    if(spi_handle < 0)
    {
        Debug_out("CloseSerial: No spi open", true);
        return true;
    }

    if((close(spi_handle)) < 0)
    {
        Debug_out("Close SPI: Could not close spi device", false);
        printf("error: %s\n", strerror(errno));
        return false;
    }

    spi_handle = -1;

    return true;
}

bool ReadBytesN(uint8_t *dataP, uint16_t length)
{
    if(spi_handle < 0)
    {
        Debug_out("SPI read bytes: Invalid spi handle", false);
        return false;
    }

    return SPI_ReadWrite(dataP, NULL, length);
}

bool SendBytes(uint8_t* dataP, uint16_t length)
{
    if(spi_handle < 0)
    {
        Debug_out("SPI write bytes: Invalid spi handle", false);
        return false;
    }

    return SPI_ReadWrite(NULL, dataP, length);
}

bool InitSerial()
{
    return true;
}

bool DeinitSerial()
{
    return true;
}

/**         EOF         */
