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
 * COPYRIGHT (c) 2019 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include "string.h"

#include <sched.h>

#include "../../drivers/WE-common.h"
#include "global.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef _global_serial_defined
#define _global_serial_defined

#ifdef _global_ftdi_defined
#error "second interface defined"
#endif

/**************************************
 *     Static function declarations   *
 **************************************/

/**************************************
 *          Static variables          *
 **************************************/
int serial_handle = 0;
int* serial_handleP = &serial_handle;

/**************************************
 *         Global functions           *
 **************************************/

bool SetPin( int pin_number, SetPin_InputOutput_t inout, SetPin_Pull_t pull, SetPin_Out_t out)
{
    switch(inout)
    {
        case SetPin_InputOutput_Input:
        {
            switch(pull)
            {
                case SetPin_Pull_Up:
                {
                    pullUpDnControl(pin_number, PUD_UP);
                }
                break;

                case SetPin_Pull_Down:
                {
                    pullUpDnControl(pin_number, PUD_DOWN);
                }
                break;

                case SetPin_Pull_No:
                {
                    pullUpDnControl(pin_number, PUD_OFF);
                }
                break;

                default:
                {
                    return false;
                }
                break;
            }
            pinMode (pin_number, INPUT) ;
        }
        break;

        case SetPin_InputOutput_Output:
        {
            switch(out)
            {
                case SetPin_Out_Low:
                {
                    digitalWrite (pin_number, LOW) ;
                }
                break;

                case SetPin_Out_High:
                {
                    digitalWrite (pin_number, HIGH) ;
                }
                break;

                default:
                {
                    return false;
                }
                break;
            }
            pinMode (pin_number, OUTPUT) ;
            pullUpDnControl(pin_number, PUD_OFF);
        }
        break;

        default:
        {
            return false;
        }
        break;
    }

    return true;
}

bool SendBytes(uint8_t* dataP, uint16_t length)
{
    if(*serial_handleP != 0)
    {
        int i = 0;
        for(i=0; i<length; i++)
        {
            serialPutchar (*serial_handleP, dataP[i]);
        }
    }

    return true;
}

bool BytesAvailable()
{
    if(*serial_handleP != 0)
    {
        int availableBytes = -1;
        availableBytes = serialDataAvail(*serial_handleP);
        return (availableBytes > 0);
    }
    else
    {
        return false;
    }
}

bool ReadByte(uint8_t *readBufferP)
{
    if(*serial_handleP != 0)
    {
        *readBufferP = serialGetchar(*serial_handleP);
    }

    return true;
}

bool CloseSerial()
{
    if(*serial_handleP != 0)
    {
        /*empty UART buffer and close connection*/
        serialFlush(*serial_handleP);
        serialClose(*serial_handleP);
        *serial_handleP = 0;
    }
    return true;
}

bool FlushSerial()
{
    if(*serial_handleP != 0)
    {
        /* empty UART buffer and close connection */
        serialFlush(*serial_handleP);
    }
    return true;
}

bool InitPin(int pin_number)
{
    return true;
}

bool DeinitPin(int pin_number)
{
    return true;
}

bool InitSerial()
{
    /* initialize wiringPi */
    if (wiringPiSetup() == -1)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        CloseSerial();
        return false;
    }
    else
    {
        /* wiring PI started successfully */
        int wiring_version_major;
        int wiring_version_minor;
        wiringPiVersion(&wiring_version_major, &wiring_version_minor);
        fprintf (stdout, "WiringPi library version %d.%d\n\n",wiring_version_major,wiring_version_minor);
    }

    return true;
}

bool DeinitSerial()
{
    return true;
}

bool OpenSerial(int baudrate)
{
    /* start serial interface */
    if ((*serial_handleP = serialOpen ("/dev/serial0", baudrate)) < 0)
    {
        fprintf (stdout, "Opening the serial interface failed. Probably invalid serial configuration.\n");
        return false ;
    }

    return true;
}

#endif // _global_serial_defined
#ifdef __cplusplus
}
#endif
