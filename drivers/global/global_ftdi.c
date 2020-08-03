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
 * COPYRIGHT (c) 2020 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "string.h"
#include "ftd2xx.h"
#include <sched.h>

#include "../../drivers/WE-common.h"
#include "global.h"


#ifndef _global_ftdi_defined
#define _global_ftdi_defined
#ifdef _global_serial_defined
#error "second interface defined"
#endif

/**************************************
 *     Static function declarations   *
 **************************************/
static bool ConfigureCBUS0(int interface);

/**************************************
 *          Static variables          *
 **************************************/

FT_HANDLE ft_handle = 0;                    /* handle for the FTDI interface */
FT_STATUS ft_status = FT_OK;                /* used to return if ftdi fuctions are successfull or if an error occured */
int interface = 0;

/**************************************
 *         Global functions           *
 **************************************/

bool SetPin( int pin_number, SetPin_InputOutput_t inout, SetPin_Pull_t pull, SetPin_Out_t out)
{
    if(pin_number == PIN_INVALID)
    {
        /* do nothing */
    }
    else if(pin_number == PIN_FTDI_DTR)
    {
        /* workaround for DTR */
        switch(inout)
        {
        case SetPin_InputOutput_Input:
        {

        }
        break;

        case SetPin_InputOutput_Output:
        {
            switch(out)
            {
            case SetPin_Out_Low:
            {
                ft_status = FT_SetDtr(ft_handle);
            }
            break;

            case SetPin_Out_High:
            {
                ft_status = FT_ClrDtr(ft_handle);
            }
            break;

            default:
            {
                return false;
            }
            break;
            }
        }
        break;

        default:
        {
            return false;
        }
        break;
        }
    }
    else if(pin_number == PIN_FTDI_CBUS0)
    {
        uint8_t mode = 0x00;
        pin_number = 0; /* CBUS0 */

        switch(inout)
        {
        case SetPin_InputOutput_Input:
        {
            ft_status = FT_SetBitMode(ft_handle, 0x00, FT_BITMODE_RESET);
        }
        break;

        case SetPin_InputOutput_Output:
        {
            /* set cbus0 to output mode
               0x10 -> cbus0 output, cbus1-3 input */
            mode = mode | (0x10 << pin_number);

            switch(out)
            {
            case SetPin_Out_Low:
            {
                mode = mode | (0x00 << pin_number);
            }
            break;

            case SetPin_Out_High:
            {
                mode = mode | (0x01 << pin_number);
            }
            break;

            default:
            {
                return false;
            }
            break;
            }
            ft_status = FT_SetBitMode(ft_handle, mode, FT_BITMODE_CBUS_BITBANG);
        }
        break;

        default:
        {
            return false;
        }
        break;
        }
    }
    else
    {
        /* not implemented */
        return false;
    }

    return true;
}

bool SendBytes(uint8_t* dataP, uint16_t length)
{
    DWORD bytesWritten = 0;
    ft_status = FT_Write(ft_handle, dataP, length, &bytesWritten);
    if(ft_status != FT_OK)
    {
        fprintf(stdout,"SendBytes failed with ftdi error code %d\n",(int)ft_status);
        return false;
    }
    return true;
}

bool BytesAvailable()
{
    DWORD availableBytes = -1;
    ft_status = FT_GetQueueStatus(ft_handle, &availableBytes);
    if(ft_status == FT_OK)
    {
        return (availableBytes > 0);
    }
    else
    {
        fprintf(stdout,"BytesAvailable failed with ftdi error code %d\n",(int)ft_status);
        return false;
    }
}

bool ReadByte(uint8_t *readBufferP)
{
    DWORD BytesRead = 0;
    ft_status = FT_Read(ft_handle, readBufferP, 1, &BytesRead);
    if(ft_status != FT_OK)
    {
        fprintf(stdout,"ReadByte failed with ftdi error code %d\n",(int)ft_status);
        return false;
    }

    return true;
}

bool CloseSerial()
{
    if(ft_handle != 0)
    {
        /* empty rx/tx buffer */
        ft_status = FT_Purge(ft_handle, FT_PURGE_RX | FT_PURGE_TX);
        if(ft_status != FT_OK)
        {
            fprintf(stdout, "CloseSerial: Could not empty RX and TX Buffer\n");
        }

        /*close connection*/
        ft_status = FT_Close(ft_handle);
        ft_handle = 0;
        if(ft_status != FT_OK)
        {
            fprintf(stdout,"CloseSerial failed with ftdi error code %d\n",(int)ft_status);
            return false;
        }
    }
    return true;
}

bool FlushSerial()
{
    if(ft_handle != 0)
    {
        ft_status = FT_Purge(ft_handle, FT_PURGE_RX | FT_PURGE_TX);
        if(ft_status != FT_OK)
        {
            fprintf(stdout, "FlushSerial: Could not empty RX and TX Buffer\n");
        }
    }
    return true;
}


bool InitPin(int pin_number)
{
    if (pin_number == PIN_INVALID)
    {
        /* nothing to do */
        return true;
    }
    else if (pin_number == PIN_FTDI_DTR)
    {
        /* nothing to do */
        return true;
    }
    else if (pin_number == PIN_FTDI_CBUS0)
    {
        return ConfigureCBUS0(interface);
    }
    else
    {
        /* not implemented */
        return false;
    }
}

bool DeinitPin(int pin_number)
{
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
    return OpenSerialWithParity(baudrate, Serial_ParityBit_NONE);
}

bool OpenSerialWithParity(int baudrate, Serial_ParityBit_t parityBit)
{
    UCHAR latency = 1;
    uint transferSize = 15*64;

    /* start FTDI interface */
    ft_status = FT_Open(interface, &ft_handle);
    if(ft_status != FT_OK)
    {
        fprintf(stdout, "FT_Open(%d) failed, with error %d.\n", interface, (int)ft_status);
        fprintf(stdout, "Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        fprintf(stdout, "If so, unload them using 'modprobe -r', as they conflict with ftd2xx.\n");
        return false;
    }

    /* set baudrate as specified by paramter */
    ft_status = FT_SetBaudRate(ft_handle, baudrate);
    if(ft_status != FT_OK)
    {
        fprintf(stdout, "Setting Baudrate failed.\n");
        CloseSerial();
        return false;
    }

    /* set to 8 Bits, 1 Stop bit and parity none (8n1) */
    ft_status = FT_SetDataCharacteristics(ft_handle, FT_BITS_8, FT_STOP_BITS_1, (UCHAR)parityBit);
    if (ft_status != FT_OK)
    {
        printf("Setting Data Characeristics to 8n1 failed\n");
        CloseSerial();
        return false;
    }

    ft_status = FT_SetLatencyTimer(ft_handle, latency);
    if (ft_status != FT_OK)
    {
        printf("Setting Latency failed\n");
        CloseSerial();
        return false;
    }

    ft_status = FT_SetUSBParameters(ft_handle,transferSize, 0);
    if (ft_status != FT_OK)
    {
        printf("Setting InTransferSize failed\n");
        CloseSerial();
        return false;
    }

    /* empty the ftdi buffers */
    ft_status = FT_Purge(ft_handle, FT_PURGE_RX | FT_PURGE_TX);
    if (ft_status != FT_OK)
    {
        printf("init: Could not empty RX and TX Puffer\n");
        CloseSerial();
        return false;
    }

    return true;
}

/**************************************
 *         Static functions           *
 **************************************/

/*
* Configure FTDI chip for bitbang mode used in pinReset()
*
* input:
* - interface: interface
* return: true, if success
*         false, otherwise
*
*/
static bool ConfigureCBUS0(int interface)
{
#if 0
    FT_PROGRAM_DATA eepromData, ctrleepromData;

    /* start FTDI interface */
    ft_status = FT_Open(interface, &ft_handle);
    if(ft_status != FT_OK)
    {
        fprintf(stdout, "FT_Open(%d) failed, with error %d.\n", interface, (int)ft_status);
        fprintf(stdout, "Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        fprintf(stdout, "If so, unload them using 'modprobe -r', as they conflict with ftd2xx.\n");
        return false;
    }

    FT_DEVICE pftType;
    DWORD deviceID;
    char serialNumber[16];
    char description[64];
    FT_GetDeviceInfo(&ft_handle, &pftType, &deviceID, serialNumber, description, NULL);

    if(pftType == FT_DEVICE_232R)
    {

        /* initalize the data structure - if it is too small, a Segmention Fault will arise*/
        char ManufacturerBuf[32];
        char ManufacturerIdBuf[16];
        char DescriptionBuf[64];
        char SerialNumberBuf[16];

        eepromData.Signature1 = 0x00000000;
        eepromData.Signature2 = 0xffffffff;
        eepromData.Version = 0x00000002; // EEPROM structure with FT232R extensions
        eepromData.Manufacturer = ManufacturerBuf;
        eepromData.ManufacturerId = ManufacturerIdBuf;
        eepromData.Description = DescriptionBuf;
        eepromData.SerialNumber = SerialNumberBuf;

        ctrleepromData.Signature1 = 0x00000000;
        ctrleepromData.Signature2 = 0xffffffff;
        ctrleepromData.Manufacturer = ManufacturerBuf;
        ctrleepromData.ManufacturerId = ManufacturerIdBuf;
        ctrleepromData.Description = DescriptionBuf;
        ctrleepromData.SerialNumber = SerialNumberBuf;

        /* read eeprom to check if value of cbus0 pin is IOMode */
        ft_status = FT_EE_Read(ft_handle, &eepromData);
        if(ft_status != FT_OK)
        {
            fprintf(stdout, "FT_EE_Read failed with error %d.\n", (int)ft_status);
            ft_status = FT_Close(ft_handle);
            return false;
        }

        /* check if CBUS pin of FTDI chip is configured so that cbus0 pin is set to IOmode */
        if(eepromData.Cbus0 != FT_232R_CBUS_IOMODE)
        {
            /* if not already, set CBUS0 pin to IOMode to use bitbang */
            eepromData.Cbus0 = FT_232R_CBUS_IOMODE;
            ft_status = FT_EE_Program(ft_handle, &eepromData);
            if(ft_status != FT_OK)
            {
                fprintf(stdout, "FT_EE_Program failed with error %d.\n", (int)ft_status);
                ft_status = FT_Close(ft_handle);
                return false;
            }

            /* read eeprom out again to check if cbus0 pin was set correctly */
            ft_status = FT_EE_Read(ft_handle, &ctrleepromData);
            if(ft_status != FT_OK)
            {
                fprintf(stdout, "FT_EE_Read failed with error %d.\n", (int)ft_status);
                ft_status = FT_Close(ft_handle);
                return false;
            }

            if(ctrleepromData.Cbus0 != eepromData.Cbus0)
            {
                /* configuration of cbus0 pin failed */
                ft_status = FT_Close(ft_handle);
                return false;
            }

            /* reset device to apply change */
            ft_status = FT_ResetDevice(ft_handle);
            if(ft_status != FT_OK)
            {
                fprintf(stdout, "FT_ResetDevice failed with error %d.\n", (int)ft_status);
                ft_status = FT_Close(ft_handle);
                return false;
            }
        }
    }
    /* close connection */
    ft_status = FT_Close(ft_handle);
    if(ft_status != FT_OK)
    {
        fprintf(stdout,"CloseSerial failed with ftdi error code %d\n",(int)ft_status);
        ft_status = FT_Close(ft_handle);
        return false;
    }

#endif // 0
    return true;
}


#endif // _global_ftdi_defined
