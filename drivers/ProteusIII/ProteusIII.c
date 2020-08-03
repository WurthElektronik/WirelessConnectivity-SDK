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


#include "ProteusIII.h"
#include "../../drivers/WE-common.h"
#include "../../drivers/global/global.h"

#define CMD_WAIT_TIME 500
#define CNFINVALID 255

#define LENGTH_CMD_OVERHEAD             (uint16_t)5
#define LENGTH_CMD_OVERHEAD_WITHOUT_CRC (uint16_t)(LENGTH_CMD_OVERHEAD - 1)
#define MAX_PAYLOAD_LENGTH              (uint16_t)964
#define MAX_CMD_LENGTH                  (uint16_t)(MAX_PAYLOAD_LENGTH + LENGTH_CMD_OVERHEAD)

#define CMD_POSITION_STX        (uint8_t)0
#define CMD_POSITION_CMD        (uint8_t)1
#define CMD_POSITION_LENGTH_LSB (uint8_t)2
#define CMD_POSITION_LENGTH_MSB (uint8_t)3
#define CMD_POSITION_DATA       (uint8_t)4

#define CMD_STX 0x02

#define ProteusIII_CMD_TYPE_REQ (uint8_t)(0 << 6)
#define ProteusIII_CMD_TYPE_CNF (uint8_t)(1 << 6)
#define ProteusIII_CMD_TYPE_IND (uint8_t)(2 << 6)
#define ProteusIII_CMD_TYPE_RSP (uint8_t)(3 << 6)


#define ProteusIII_CMD_RESET (uint8_t)0x00
#define ProteusIII_CMD_RESET_REQ (ProteusIII_CMD_RESET | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_RESET_CNF (ProteusIII_CMD_RESET | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_GETSTATE (uint8_t)0x01
#define ProteusIII_CMD_GETSTATE_REQ (ProteusIII_CMD_GETSTATE | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GETSTATE_CNF (ProteusIII_CMD_GETSTATE | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_GETSTATE_IND (ProteusIII_CMD_GETSTATE | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_SLEEP (uint8_t)0x02
#define ProteusIII_CMD_SLEEP_REQ (ProteusIII_CMD_SLEEP | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_SLEEP_CNF (ProteusIII_CMD_SLEEP | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_SLEEP_IND (ProteusIII_CMD_SLEEP | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_DATA (uint8_t)0x04
#define ProteusIII_CMD_DATA_REQ (ProteusIII_CMD_DATA | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_DATA_CNF (ProteusIII_CMD_DATA | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_DATA_IND (ProteusIII_CMD_DATA | ProteusIII_CMD_TYPE_IND)
#define ProteusIII_CMD_TXCOMPLETE_RSP (ProteusIII_CMD_DATA | ProteusIII_CMD_TYPE_RSP)

#define ProteusIII_CMD_CONNECT (uint8_t)0x06
#define ProteusIII_CMD_CONNECT_REQ (ProteusIII_CMD_CONNECT | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_CONNECT_CNF (ProteusIII_CMD_CONNECT | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_CONNECT_IND (ProteusIII_CMD_CONNECT | ProteusIII_CMD_TYPE_IND)
#define ProteusIII_CMD_CHANNELOPEN_RSP (ProteusIII_CMD_CONNECT | ProteusIII_CMD_TYPE_RSP)

#define ProteusIII_CMD_DISCONNECT (uint8_t)0x07
#define ProteusIII_CMD_DISCONNECT_REQ (ProteusIII_CMD_DISCONNECT | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_DISCONNECT_CNF (ProteusIII_CMD_DISCONNECT | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_DISCONNECT_IND (ProteusIII_CMD_DISCONNECT | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_SECURITY_IND (uint8_t)0x88

#define ProteusIII_CMD_SCANSTART (uint8_t)0x09
#define ProteusIII_CMD_SCANSTART_REQ (ProteusIII_CMD_SCANSTART  | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_SCANSTART_CNF (ProteusIII_CMD_SCANSTART  | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_SCANSTART_IND (ProteusIII_CMD_SCANSTART  | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_SCANSTOP (uint8_t)0x0A
#define ProteusIII_CMD_SCANSTOP_REQ (ProteusIII_CMD_SCANSTOP  | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_SCANSTOP_CNF (ProteusIII_CMD_SCANSTOP  | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_SCANSTOP_IND (ProteusIII_CMD_SCANSTOP  | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_GETDEVICES (uint8_t)0x0B
#define ProteusIII_CMD_GETDEVICES_REQ (ProteusIII_CMD_GETDEVICES  | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GETDEVICES_CNF (ProteusIII_CMD_GETDEVICES  | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_GETDEVICES_IND (ProteusIII_CMD_GETDEVICES  | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_PASSKEY (uint8_t)0x0D
#define ProteusIII_CMD_PASSKEY_REQ (ProteusIII_CMD_PASSKEY  | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_PASSKEY_CNF (ProteusIII_CMD_PASSKEY  | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_PASSKEY_IND (ProteusIII_CMD_PASSKEY  | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_GET (uint8_t)0x10
#define ProteusIII_CMD_GET_REQ (ProteusIII_CMD_GET | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GET_CNF (ProteusIII_CMD_GET | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_SET (uint8_t)0x11
#define ProteusIII_CMD_SET_REQ (ProteusIII_CMD_SET | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_SET_CNF (ProteusIII_CMD_SET | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_PHYUPDATE (uint8_t)0x1A
#define ProteusIII_CMD_PHYUPDATE_REQ (ProteusIII_CMD_PHYUPDATE | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_PHYUPDATE_CNF (ProteusIII_CMD_PHYUPDATE | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_PHYUPDATE_IND (ProteusIII_CMD_PHYUPDATE | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_FACTORYRESET (uint8_t)0x1C
#define ProteusIII_CMD_FACTORYRESET_REQ (ProteusIII_CMD_FACTORYRESET | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_FACTORYRESET_CNF (ProteusIII_CMD_FACTORYRESET | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_NUMERIC_COMP (uint8_t)0x24
#define ProteusIII_CMD_NUMERIC_COMP_REQ    (ProteusIII_CMD_NUMERIC_COMP | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_NUMERIC_COMP_CNF    (ProteusIII_CMD_NUMERIC_COMP | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_DISPLAY_PASSKEY_IND (ProteusIII_CMD_NUMERIC_COMP | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG (uint8_t)0x25
#define ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG_REQ (ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG_CNF (ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_GPIO_LOCAL_READCONFIG (uint8_t)0x2B
#define ProteusIII_CMD_GPIO_LOCAL_READCONFIG_REQ (ProteusIII_CMD_GPIO_LOCAL_READCONFIG | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_LOCAL_READCONFIG_CNF (ProteusIII_CMD_GPIO_LOCAL_READCONFIG | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_GPIO_LOCAL_WRITE (uint8_t)0x26
#define ProteusIII_CMD_GPIO_LOCAL_WRITE_REQ (ProteusIII_CMD_GPIO_LOCAL_WRITE | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_LOCAL_WRITE_CNF (ProteusIII_CMD_GPIO_LOCAL_WRITE | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_GPIO_LOCAL_WRITE_IND (ProteusIII_CMD_GPIO_LOCAL_WRITE | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_GPIO_LOCAL_READ (uint8_t)0x27
#define ProteusIII_CMD_GPIO_LOCAL_READ_REQ (ProteusIII_CMD_GPIO_LOCAL_READ | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_LOCAL_READ_CNF (ProteusIII_CMD_GPIO_LOCAL_READ | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG (uint8_t)0x28
#define ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG_REQ (ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG_CNF (ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG_IND (ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_GPIO_REMOTE_READCONFIG (uint8_t)0x2C
#define ProteusIII_CMD_GPIO_REMOTE_READCONFIG_REQ (ProteusIII_CMD_GPIO_REMOTE_READCONFIG | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_REMOTE_READCONFIG_CNF (ProteusIII_CMD_GPIO_REMOTE_READCONFIG | ProteusIII_CMD_TYPE_CNF)

#define ProteusIII_CMD_GPIO_REMOTE_WRITE (uint8_t)0x29
#define ProteusIII_CMD_GPIO_REMOTE_WRITE_REQ (ProteusIII_CMD_GPIO_REMOTE_WRITE | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_REMOTE_WRITE_CNF (ProteusIII_CMD_GPIO_REMOTE_WRITE | ProteusIII_CMD_TYPE_CNF)
#define ProteusIII_CMD_GPIO_REMOTE_WRITE_IND (ProteusIII_CMD_GPIO_REMOTE_WRITE | ProteusIII_CMD_TYPE_IND)

#define ProteusIII_CMD_GPIO_REMOTE_READ (uint8_t)0x2A
#define ProteusIII_CMD_GPIO_REMOTE_READ_REQ (ProteusIII_CMD_GPIO_REMOTE_READ | ProteusIII_CMD_TYPE_REQ)
#define ProteusIII_CMD_GPIO_REMOTE_READ_CNF (ProteusIII_CMD_GPIO_REMOTE_READ | ProteusIII_CMD_TYPE_CNF)

#define CMD_ARRAY_SIZE() ((((uint16_t)CMD_Array[CMD_POSITION_LENGTH_LSB] << 0) | ((uint16_t)CMD_Array[CMD_POSITION_LENGTH_MSB] << 8)) + LENGTH_CMD_OVERHEAD)


/* type used to check the response, when a command was sent to the ProteusIII */
typedef enum CMD_Status_t
{
    CMD_Status_Success = (uint8_t)0x00,
    CMD_Status_Failed = (uint8_t)0x01,
    CMD_Status_Invalid,
    CMD_Status_Reset,
    CMD_Status_NoStatus,
} CMD_Status_t;

typedef struct
{
    uint8_t cmd; /* variable to check if correct CMD has been confirmed */
    CMD_Status_t status; /* variable used to check the response (*_CNF), when a request (*_REQ) was sent to the ProteusIII */
} CMD_Confirmation_t;

/**************************************
 *     Static function declarations   *
 **************************************/

static void HandleRxPacket(uint8_t*RxBuffer);                                                      /* RX packet interpreter */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate);
static bool FillChecksum(uint8_t* array, uint16_t length);                                           /* add the CS needed to finalize the command */
static bool InitDriver(ProteusIII_CallbackConfig_t callbackConfig);

/**************************************
 *          Static variables          *
 **************************************/
static uint8_t CMD_Array[MAX_CMD_LENGTH]; /* for UART TX to module*/
static uint8_t RxPacket[MAX_CMD_LENGTH];

bool AbortUartRxThread;                     /* boolean to abort the UART RX thread */
bool ResetUartRxThread;                     /* boolean to reset the UART RX thread */

pthread_t thread_read;

#define CMDCONFIRMATIONARRAY_LENGTH 2

CMD_Confirmation_t cmdConfirmation_array[CMDCONFIRMATIONARRAY_LENGTH];

static ProteusIII_GetDevices_t* ProteusIII_GetDevicesP = NULL;
static ProteusIII_States_t ble_state;
static bool askedForState;

int reset_pin = 0;                              /* reset pin number */
int wakeup_pin = 0;                             /* wakeup pin number */
int boot_pin = 0;                               /* boot pin number */


ProteusIII_CallbackConfig_t callbacks;
/**************************************
 *         Static functions           *
 **************************************/

/* thread function to receive bytes from interface */
void *rx_thread()
{
    uint8_t checksum = 0;
    uint16_t RxByteCounter = 0;
    uint16_t BytesToReceive = 0;
    uint8_t readBuffer;
    static uint8_t RxBuffer[MAX_CMD_LENGTH]; /* For UART RX from module */

    ResetUartRxThread = false;
    AbortUartRxThread = false;

    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_UARTRXTHREAD);
    while(1)
    {
        /* wait for 1ms, then check if new RX data is available */
        delay (1);

        if(AbortUartRxThread == true)
        {
            /* jump out of the while loop and finish the thread */
            AbortUartRxThread = false;
            break;
        }

        if(ResetUartRxThread == true)
        {
            /* reset the RX thread */
            ResetUartRxThread = false;
            RxByteCounter = 0;
        }

        while (BytesAvailable())
        {
            /* interprete received byte */
            if(ReadByte(&readBuffer))
            {
                RxBuffer[RxByteCounter] = readBuffer;

                switch (RxByteCounter)
                {
                case 0:
                    /* wait for start byte of frame */
                    if (RxBuffer[RxByteCounter] == CMD_STX)
                    {
                        BytesToReceive = 0;
                        RxByteCounter = 1;
                    }
                    break;

                case 1:
                    /* CMD */
                    RxByteCounter++;
                    break;

                case 2:
                    /* length field lsb */
                    RxByteCounter++;
                    BytesToReceive = (uint16_t)(RxBuffer[RxByteCounter - 1]);
                    break;

                case 3:
                    /* length field msb */
                    RxByteCounter++;
                    BytesToReceive += (((uint16_t)RxBuffer[RxByteCounter - 1]<<8) + LENGTH_CMD_OVERHEAD); /* len_msb + len_lsb + crc + sfd + cmd */
                    break;

                default:
                    /* data field */
                    RxByteCounter++;
                    if (RxByteCounter == BytesToReceive)
                    {
                        /* check CRC */
                        checksum = 0;
                        int i = 0;
                        for (i = 0; i < (BytesToReceive - 1); i++)
                        {
                            checksum ^= RxBuffer[i];
                        }

                        if (checksum == RxBuffer[BytesToReceive - 1])
                        {
                            /* received frame ok, interprete it now */
                            HandleRxPacket(RxBuffer);
                        }

                        RxByteCounter = 0;
                        BytesToReceive = 0;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}

/*
 *Initialize the ProteusIII and driver
 *
 *input:
 * -RXcb:      RX callback function
 * -Ccb:       Connect callback function
 * -DCcb:      Disconnect callback function
 * -COcb:      Channel open callback function
 * -Scb:       Security callback function
 * -DPKcb:     Display Passkey callback function
 * -PKcb:      Passkey callback function
 *
 *return true if initialization succeeded
 *       false otherwise
 */
static bool InitDriver(ProteusIII_CallbackConfig_t callbackConfig)
{
    /* set RX callback function */
    callbacks.rxCb = callbackConfig.rxCb;
    callbacks.connectCp = callbackConfig.connectCp;
    callbacks.securityCb = callbackConfig.securityCb;
    callbacks.passkeyCb = callbackConfig.passkeyCb;
    callbacks.disconnectCb = callbackConfig.disconnectCb;
    callbacks.channelOpenCb = callbackConfig.channelOpenCb;
    callbacks.phyUpdateCb = callbackConfig.phyUpdateCb;

    askedForState = false;
    ble_state = ProteusIII_State_BLE_Invalid;
    ProteusIII_GetDevicesP = NULL;

    /* start RX thread */
    if(pthread_create(&thread_read, NULL, &rx_thread, NULL))
    {
        fprintf(stdout, "Failed to start rx_Thread\n");
        ProteusIII_Deinit();
        return false;
    }

    /* reset module*/
    if(ProteusIII_PinReset())
    {
        delay(300);
    }
    else
    {
        fprintf(stdout, "Pin Reset failed\n");
        ProteusIII_Deinit();
        return false;
    }

    uint8_t driver_version[3];
    if(GetDriverVersion(driver_version))
    {
        fprintf (stdout, COLOR_CYAN "ProteusIII driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(100);


    uint8_t firmware_version[3];
    if(ProteusIII_GetFWVersion(firmware_version))
    {
        fprintf (stdout, COLOR_CYAN "Firmware version %d.%d.%d\n" COLOR_RESET,firmware_version[2],firmware_version[1],firmware_version[0]);
    }
    delay(100);

    return true;
}

static void HandleRxPacket(uint8_t * pRxBuffer)
{
    CMD_Confirmation_t cmdConfirmation;
    cmdConfirmation.cmd = CNFINVALID;
    cmdConfirmation.status = CMD_Status_Invalid;

    uint16_t cmd_length = (uint16_t)(pRxBuffer[CMD_POSITION_LENGTH_LSB]+(pRxBuffer[CMD_POSITION_LENGTH_MSB]<<8));
    memcpy(&RxPacket[0], pRxBuffer, cmd_length + LENGTH_CMD_OVERHEAD);

    switch (RxPacket[CMD_POSITION_CMD])
    {
    case ProteusIII_CMD_RESET_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_SCANSTART_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_SCANSTOP_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GETDEVICES_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        if((cmdConfirmation.status == CMD_Status_Success)&&(ProteusIII_GetDevicesP != NULL))
        {
            uint8_t size = RxPacket[CMD_POSITION_DATA+1];

            if (size >= MAX_NUMBER_OF_DEVICES)
            {
                size = MAX_NUMBER_OF_DEVICES;
            }
            ProteusIII_GetDevicesP->numberofdevices = size;

            int i;
            int len = CMD_POSITION_DATA+2;
            for(i=0; i<ProteusIII_GetDevicesP->numberofdevices; i++)
            {
                memcpy(&ProteusIII_GetDevicesP->devices[i].btmac[0],&RxPacket[len],6);
                ProteusIII_GetDevicesP->devices[i].rssi = RxPacket[len+6];
                ProteusIII_GetDevicesP->devices[i].txpower = RxPacket[len+7];
                ProteusIII_GetDevicesP->devices[i].devicenamelength = RxPacket[len+8];
                memcpy(&ProteusIII_GetDevicesP->devices[i].devicename[0],&RxPacket[len+9],ProteusIII_GetDevicesP->devices[i].devicenamelength);
                len += (9+ProteusIII_GetDevicesP->devices[i].devicenamelength);
            }
        }
        break;
    }

    case ProteusIII_CMD_GET_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_SET_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_PASSKEY_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_PHYUPDATE_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GETSTATE_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = CMD_Status_NoStatus;
        break;
    }

    case ProteusIII_CMD_CONNECT_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_DATA_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_DISCONNECT_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_FACTORYRESET_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_SLEEP_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_LOCAL_READCONFIG_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_LOCAL_WRITE_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_LOCAL_READ_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_REMOTE_READCONFIG_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_REMOTE_WRITE_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_GPIO_REMOTE_READ_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_TXCOMPLETE_RSP:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_CMD_CHANNELOPEN_RSP:
    {
        /* Payload of CHANNELOPEN_RSP: Status (1 byte), BTMACH (6 byte), Max Payload (1byte)*/
        ble_state = ProteusIII_State_BLE_Channel_Open;
        if(callbacks.channelOpenCb != NULL)
        {
            callbacks.channelOpenCb(&RxPacket[CMD_POSITION_DATA+1], (uint16_t)RxPacket[CMD_POSITION_DATA + 7]);
        }
        break;
    }

    case ProteusIII_CMD_CONNECT_IND:
    {
        ble_state = ProteusIII_State_BLE_Connected;
        if(callbacks.connectCp != NULL)
        {
            callbacks.connectCp(&RxPacket[CMD_POSITION_DATA+1]);
        }
        break;
    }

    case ProteusIII_CMD_DISCONNECT_IND:
    {
        if(callbacks.disconnectCb != NULL)
        {
            callbacks.disconnectCb();
        }
        break;
    }

    case ProteusIII_CMD_DATA_IND:
    {
        if(callbacks.rxCb != NULL)
        {
            uint16_t payload_length = ((((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) | ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8))) - 7;
            callbacks.rxCb(&RxPacket[CMD_POSITION_DATA + 7], payload_length, &RxPacket[CMD_POSITION_DATA], RxPacket[CMD_POSITION_DATA + 6]);
        }
        break;
    }

    case ProteusIII_CMD_SECURITY_IND:
    {
        if(callbacks.securityCb != NULL)
        {
            callbacks.securityCb(&RxPacket[CMD_POSITION_DATA+1],RxPacket[CMD_POSITION_DATA]);
        }
        break;
    }

    case ProteusIII_CMD_PASSKEY_IND:
    {
        if(callbacks.passkeyCb != NULL)
        {
            callbacks.passkeyCb(&RxPacket[CMD_POSITION_DATA+1]);
        }
        break;
    }

    case ProteusIII_CMD_DISPLAY_PASSKEY_IND:
    {
        if(callbacks.displayPasskeyCb != NULL)
        {
            callbacks.displayPasskeyCb((ProteusIII_DisplayPasskeyAction_t)RxPacket[CMD_POSITION_DATA],&RxPacket[CMD_POSITION_DATA+1],&RxPacket[CMD_POSITION_DATA+7]);
        }
        break;
    }

    case ProteusIII_CMD_PHYUPDATE_IND:
    {
        if(callbacks.phyUpdateCb != NULL)
        {
            callbacks.phyUpdateCb(&RxPacket[CMD_POSITION_DATA+3],(ProteusIII_Phy_t)RxPacket[CMD_POSITION_DATA+1],(ProteusIII_Phy_t)RxPacket[CMD_POSITION_DATA+2]);
        }
        break;
    }

    default:
    {
        /* invalid*/
        break;
    }
    }

    int i = 0;
    for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
    {
        if(cmdConfirmation_array[i].cmd == CNFINVALID)
        {
            cmdConfirmation_array[i].cmd = cmdConfirmation.cmd;
            cmdConfirmation_array[i].status = cmdConfirmation.status;
            break;
        }
    }
}

/* function that waits for the return value of ProteusIII (*_CNF), when a command (*_REQ) was sent before */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate)
{
    int count = 0;
    int time_step_ms = 5; /* 5ms */
    int max_count = max_time_ms / time_step_ms;
    int i = 0;

    if(reset_confirmstate)
    {
        for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
        {
            cmdConfirmation_array[i].cmd = CNFINVALID;
        }
    }
    while (1)
    {
        for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
        {
            if(expectedCmdConfirmation == cmdConfirmation_array[i].cmd)
            {
                return (cmdConfirmation_array[i].status == expectedStatus);
            }
        }

        if (count >= max_count)
        {
            /* received no correct response within timeout */
            return false;
        }

        /* wait */
        count++;
        delay(time_step_ms);
    }
    return true;
}

/* function to add the checksum at the end of the data packet */
static bool
FillChecksum(uint8_t* pArray, uint16_t length)
{
    bool ret = false;

    if ((length >= LENGTH_CMD_OVERHEAD) && (pArray[0] == CMD_STX))
    {
        uint8_t checksum = (uint8_t)0;
        uint16_t payload_length = (uint16_t) (pArray[CMD_POSITION_LENGTH_MSB] << 8) + pArray[CMD_POSITION_LENGTH_LSB];
        uint16_t i = 0;
        for (i = 0;
                i < (payload_length + LENGTH_CMD_OVERHEAD_WITHOUT_CRC);
                i++)
        {
            checksum ^= pArray[i];
        }
        pArray[payload_length + LENGTH_CMD_OVERHEAD_WITHOUT_CRC] = checksum;
        ret = true;
    }
    return ret;
}


/**************************************
 *         Global functions           *
 **************************************/

/*
 *Initialize the ProteusIII interface for serial interface
 *
 *input:
 * -baudrate:       baudrate of the interface
 * -rp:             Reset Pin
 * -wp:             Wake-up Pin
 * -bp:             Boot Pin
 * -callbackConfig  Function pointers for callbacks
 *
 *Caution: the parameter baudrate must match the configured UserSettings of the ProteusIII
 *         -the baudrate parameter must match to perform a successful FTDI communication
 *          *updating this parameter during runtime may lead to communication errors
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool ProteusIII_Init(int baudrate, int rp, int wp, int bp, ProteusIII_CallbackConfig_t callbackConfig)
{
    if (false == InitSerial())
    {
        /* error */
        return false ;
    }

    if (false == OpenSerial(baudrate))
    {
        /* error */
        return false ;
    }

    /* initialize the boot pin */
    if (false == InitPin(bp))
    {
        /* error */
        return false ;
    }
    boot_pin = bp;
    SetPin(boot_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);

    /* initialize the wakeup pin */
    if (false == InitPin(wp))
    {
        /* error */
        return false ;
    }
    wakeup_pin = wp;
    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);

    /* initialize the reset pin,
     * we have to define it as input/pull-up such that the reset button
     * can pull the pin level down if the button is pressed
     */
    if (false == InitPin(rp))
    {
        /* error */
        return false ;
    }
    reset_pin = rp;
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);
    delay(10);

    /* empty the UART buffers */
    FlushSerial();

    /* init the driver and module*/
    return InitDriver(callbackConfig);
}

/*
 *Deinitialize the ProteusIII interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ProteusIII_Deinit()
{
    /* close the communication interface to the module */
    CloseSerial();
    DeinitSerial();

    /* abort RX thread */
    AbortUartRxThread = true;

    /* deinit pins */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    DeinitPin(reset_pin);
    SetPin(wakeup_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    DeinitPin(wakeup_pin);
    SetPin(boot_pin, SetPin_InputOutput_Input, SetPin_Pull_Down, SetPin_Out_High);
    DeinitPin(boot_pin);

    /* reset callbacks */
    callbacks.rxCb = NULL;
    callbacks.connectCp = NULL;
    callbacks.securityCb = NULL;
    callbacks.passkeyCb = NULL;
    callbacks.disconnectCb = NULL;
    callbacks.channelOpenCb = NULL;
    callbacks.phyUpdateCb = NULL;

    return true;
}

/*
 *Wakeup the ProteusIII from sleep by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool ProteusIII_PinWakeup()
{
    int i = 0;

    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
    delay (5);
    for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
    {
        cmdConfirmation_array[i].status = CMD_Status_Invalid;
        cmdConfirmation_array[i].cmd = CNFINVALID;
    }
    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);

    /* wait for cnf */
    return Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GETSTATE_CNF, CMD_Status_NoStatus, false);
}

/*
 *Reset the ProteusIII by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIII_PinReset()
{
    /* set to output mode */
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
    delay (5);
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    FlushSerial();
    ResetUartRxThread = true;
    delay (5);
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);

    /* set to input mode again */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);

    /* wait for cnf */
    return Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true);
}

/*
 *Reset the ProteusIII by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIII_Reset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_RESET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        return Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true);
    }
    return ret;
}

/*
 *Disconnect the ProteusIII connection if open
 *
 *return true if disconnection succeeded
 *       false otherwise
 */
bool
ProteusIII_Disconnect()
{
    bool ret = false;
    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_DISCONNECT_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* Confirmation is sent before perfoming the disconnect. After disconnect, module sends dicsonnect indication */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_DISCONNECT_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Put the ProteusIII into sleep mode
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIII_Sleep()
{
    bool ret = false;
    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_SLEEP_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes( CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_SLEEP_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Transmit data if a connection is open
 *
 *input:
 * -PayloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIII_Transmit(uint8_t *PayloadP, uint16_t length)
{
    bool ret = false;
    if ((length < MAX_PAYLOAD_LENGTH)&&(ProteusIII_State_BLE_Channel_Open == ProteusIII_GetDriverState()))
    {
        CMD_Array[CMD_POSITION_STX] = CMD_STX;
        CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_DATA_REQ;
        CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t) (length >> 0);
        CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t) (length >> 8);

        memcpy(&CMD_Array[CMD_POSITION_DATA], PayloadP, length);

        if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
        {
            SendBytes( CMD_Array, CMD_ARRAY_SIZE());
            ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_TXCOMPLETE_RSP, CMD_Status_Success, true);
        }
    }
    return ret;
}


/*
 *Factory reset the module
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIII_FactoryReset()
{
    bool ret = false;
    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_FACTORYRESET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for reset after factory reset */
        return Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true);
    }
    return ret;
}

/*
 *Set a special user setting
 *
 *input:
 * -us:     user setting to be updated
 * -value:  pointer to the new settings value
 * -length: length of the value
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_Set(ProteusIII_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_SET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t) (1 + length);
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_DATA] = userSetting;
    memcpy(&CMD_Array[CMD_POSITION_DATA + 1], ValueP, length);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        return Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_SET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Set the BLE device name
 *
 *input:
 * -deviceNameP: pointer to the device name
 * -nameLength:  length of the device name
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength)
{
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_DEVICE_NAME, deviceNameP, nameLength);
}

/*
 *Set the BLE advertising timeout
 *
 *input:
 * -advTimeout: advertising timeout
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetAdvertisingTimeout(uint16_t advTimeout)
{
    uint8_t help[2];
    memcpy(help,(uint8_t*)&advTimeout,2);
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT, help, 2);
}

/*
 *Set the CFG flags
 *
 *input:
 * -cfgflags: CFG flags
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetCFGFlags(uint16_t cfgflags)
{
    uint8_t help[2];
    memcpy(help,(uint8_t*)&cfgflags,2);
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_CFGFLAGS, help, 2);
}

/*
 *Set the BLE connection timing
 *
 *input:
 * -connectionTiming: connection timing
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetConnectionTiming(ProteusIII_ConnectionTiming_t connectionTiming)
{
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_CONNECTION_TIMING, (uint8_t*)&connectionTiming, 1);
}

/*
 *Set the BLE scan timing
 *
 *input:
 * -scanTiming: scan timing
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetScanTiming(ProteusIII_ScanTiming_t scanTiming)
{
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_SCAN_TIMING, (uint8_t*)&scanTiming, 1);
}

/*
 *Set the BLE TX power
 *
 *input:
 * -txpower: TX power
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetTXPower(ProteusIII_TXPower_t txpower)
{
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_TX_POWER, (uint8_t*)&txpower, 1);
}

/*
 *Set the BLE security flags
 *
 *input:
 * -secflags: security flags
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetSecFlags(ProteusIII_SecFlags_t secflags)
{
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_SEC_FLAGS, (uint8_t*)&secflags, 1);
}

/*
 *Set the UART baudrate index
 *
 *input:
 * -baudrate: UART baudrate
 * -parity: parity bit
 * -flowcontrolEnable: enable/disable flow conrol
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SetBaudrateIndex(ProteusIII_BaudRate_t baudrate, ProteusIII_UartParity_t parity, bool flowcontrolEnable)
{
    uint8_t baudrateIndex = (uint8_t)baudrate;

    /* If flowcontrol is to be enabled UART index has to be increased by one in regard to base value */
    if(flowcontrolEnable)
    {
        baudrateIndex++;
    }

    /* If parity bit is even, UART index has to be increased by 64 in regard of base value*/
    if(ProteusIII_UartParity_Even == parity)
    {
        baudrateIndex += 64;
    }

    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_UART_CONFIG_INDEX, (uint8_t*)&baudrateIndex, 1);
}

/*
 *Set the BLE static passkey
 *
 *input:
 * -staticPasskeyP: pointer to the static passkey
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SetStaticPasskey(uint8_t *staticPasskeyP)
{
    return ProteusIII_Set(ProteusIII_USERSETTING_POSITION_RF_STATIC_PASSKEY, staticPasskeyP, 6);
}

/*
 *Request the current user settings
 *
 *input:
 * -userSetting: user setting to be requested
 *
 *output:
 * -responseP: pointer of the memory to put the requested content
 * -response_lengthP: length of the requested content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_Get(ProteusIII_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)1;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_DATA] = userSetting;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GET_CNF, CMD_Status_Success, true))
        {
            uint16_t length = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8);
            memcpy(ResponseP, &RxPacket[CMD_POSITION_DATA + 1], length - 1); /* First Data byte is status, following bytes response*/
            *Response_LengthP = length - 1;
            ret = true;
        }
    }
    return ret;
}

/*
 *Request the 3 byte firmware version
 *
 *output:
 * -versionP: pointer to the 3 byte firmware version
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetFWVersion(uint8_t *versionP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_FS_FWVersion, versionP, &length);
}

/*
 *Request the current BLE device name
 *
 *output:
 * -deviceNameP: pointer to device name
 * -nameLengthP: pointer to the length of the device name
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP)
{
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_DEVICE_NAME, deviceNameP, nameLengthP);
}

/*
 *Request the 8 digit MAC
 *
 *output:
 * -macP: pointer to the MAC
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetMAC(uint8_t *MacP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_FS_MAC, MacP, &length);
}

/*
 *Request the 6 digit Bluetooth MAC
 *
 *output:
 * -BTMacP: pointer to the Bluetooth MAC
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetBTMAC(uint8_t *BTMacP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_FS_BTMAC, BTMacP, &length);
}

/*
 *Request the advertising timeout
 *
 *output:
 * -advTimeoutP: pointer to the advertising timeout
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetAdvertisingTimeout(uint16_t *advTimeoutP)
{
    uint16_t length;
    bool ret = false;
    uint8_t help[2];

    ret = ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT, help, &length);
    memcpy((uint8_t*)advTimeoutP,help,2);

    return ret;
}

/*
 *Request the connection timing
 *
 *output:
 * -connectionTimingP: pointer to the  connection timing
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetConnectionTiming(ProteusIII_ConnectionTiming_t *connectionTimingP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_CONNECTION_TIMING, (uint8_t*)connectionTimingP, &length);
}

/*
 *Request the scan timing
 *
 *output:
 * -scanTimingP: pointer to the  scan timing
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetScanTiming(ProteusIII_ScanTiming_t *scanTimingP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_SCAN_TIMING, (uint8_t*)scanTimingP, &length);
}

/*
 *Request the TX power
 *
 *output:
 * -txpowerP: pointer to the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetTXPower(ProteusIII_TXPower_t *txpowerP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_TX_POWER, (uint8_t*)txpowerP, &length);
}

/*
 *Request the security flags
 *
 *output:
 * -secflagsP: pointer to the security flags
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetSecFlags(ProteusIII_SecFlags_t *secflagsP)
{
    uint16_t length;
    return ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_SEC_FLAGS, (uint8_t*)secflagsP, &length);
}

/*
 *Request the UART baudrate index
 *
 *output:
 * -baudrateIndexP: pointer to the UART baudrate index
 *
 *return true if request succeeded
*       false otherwise
*/
bool ProteusIII_GetBaudrateIndex(ProteusIII_BaudRate_t *baudrateP, ProteusIII_UartParity_t *parityP, bool *flowcontrolEnableP)
{
    bool ret = false;
    uint16_t length;
    uint8_t uartIndex;

    if(ProteusIII_Get(ProteusIII_USERSETTING_POSITION_UART_CONFIG_INDEX, (uint8_t*)&uartIndex, &length))
    {
        /* if index is even, flow control is off.
         * If flow control is on, decrease index by one to later determin the base baudrate */
        if(0x01 == (uartIndex & 0x01))
        {
            /* odd */
            *flowcontrolEnableP = true;
            uartIndex--;
        }
        else
        {
            /* even */
            *flowcontrolEnableP = false;
        }

        /* If baudrate index is greater than or equal to 64, parity bit is even*/
        if(uartIndex < 64)
        {
            *parityP = ProteusIII_UartParity_None;
        }
        else
        {

            *parityP = ProteusIII_UartParity_Even;
            uartIndex -= 64;
        }

        *baudrateP = (ProteusIII_BaudRate_t)uartIndex;
        ret = true;
    }

    return ret;
}

/*
 *Request the CFG flags
 *
 *output:
 * -cfgflags: pointer to the CFG flags
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetCFGFlags(uint16_t *cfgflags)
{
    uint16_t length;
    bool ret = false;
    uint8_t help[2];

    ret = ProteusIII_Get(ProteusIII_USERSETTING_POSITION_RF_CFGFLAGS, help, &length);
    memcpy((uint8_t*)cfgflags,help,2);

    return ret;
}

/*
 *Request the module state
 *
 *output:
 * -BLE_roleP:   pointer to the BLE role of the module
 * -BLE_actionP: pointer to the BLE action of the module
 * -InfoP:       pointer to the state info of the module
 * -LengthP:     pointer to the length of the state info of the module
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetState(ProteusIII_BLE_Role_t *BLE_roleP, ProteusIII_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GETSTATE_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());
        askedForState = true;
        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true))
        {
            uint16_t length = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8);
            *BLE_roleP = RxPacket[CMD_POSITION_DATA];
            *BLE_actionP = RxPacket[CMD_POSITION_DATA + 1];
            memcpy(InfoP, &RxPacket[CMD_POSITION_DATA + 2], length - 2);

            *LengthP = length-2;
            ret = true;
        }
    }
    askedForState = false;
    return ret;
}

/*
 *Request the current state of the driver
 *
 *
 *return driver state
 */
ProteusIII_States_t ProteusIII_GetDriverState()
{
    return ble_state;
}

/*
 *Start scan
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_Scanstart()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_SCANSTART_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_SCANSTART_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Stop a scan
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_Scanstop()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_SCANSTOP_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_SCANSTOP_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Request the scan results
 *
 *output:
 * -devicesP: pointer to scan result struct
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_GetDevices(ProteusIII_GetDevices_t* devicesP)
{
    bool ret = false;

    ProteusIII_GetDevicesP = devicesP;
    if (ProteusIII_GetDevicesP != NULL)
    {
        ProteusIII_GetDevicesP->numberofdevices = 0;
    }

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GETDEVICES_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GETDEVICES_CNF, CMD_Status_Success, true);
    }

    ProteusIII_GetDevicesP = NULL;

    return ret;
}

/*
 *Connect to the BLE device with the corresponding BTMAC
 *
 *input:
 * -btmac: pointer to btmac
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_Connect(uint8_t *btmac)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_CONNECT_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)6;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    memcpy(&CMD_Array[CMD_POSITION_DATA], btmac, 6);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(3000, ProteusIII_CMD_CONNECT_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Answer on a passkey request with a passkey to setup a connection
 *
 *input:
 * -passkey: pointer to passkey
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_Passkey(uint8_t* passkey)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_PASSKEY_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)6;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    memcpy(&CMD_Array[CMD_POSITION_DATA], passkey, 6);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_PASSKEY_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Answer on a numeric comparison request
 *
 *input:
 * -keyIsOk: boolean to confirm if the key shown is correct
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_NumericCompareConfirm(bool keyIsOk)
{
    bool ret = false;
    uint8_t lescStatus;

    /* If key is ok, status byte is "0x00"
        ohterwise it's "0x01" */
    if(keyIsOk)
    {
        lescStatus = 0x00;
    }
    else
    {
        lescStatus = 0x01;
    }

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_NUMERIC_COMP_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)1;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_DATA] = lescStatus;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_NUMERIC_COMP_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Update the phy during an open connection
 *
 *input:
 * -phy: new phy
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_PhyUpdate(ProteusIII_Phy_t phy)
{
    bool ret = false;

    if (ProteusIII_State_BLE_Channel_Open == ProteusIII_GetDriverState())
    {
        /* fill CMD_ARRAY packet */
        CMD_Array[CMD_POSITION_STX] = CMD_STX;
        CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_PHYUPDATE_REQ;
        CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)1;
        CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
        CMD_Array[CMD_POSITION_DATA] = (uint8_t)phy;

        if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
        {
            /* now send CMD_ARRAY */
            SendBytes(CMD_Array, CMD_ARRAY_SIZE());

            /* wait for cnf */
            ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_PHYUPDATE_CNF, CMD_Status_Success, true);
        }
    }
    return ret;
}

/*
 *Configure the local GPIO of the module
 *
 *input:
 * -configP: pointer to one or more pin configurations
 * -configLength: length of data configP points to
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIOLocalWriteConfig(ProteusIII_GPIOConfigBlock_t* configP, uint16_t configLength)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (configLength & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (configLength & 0xFF00) >> 8;
    memcpy(&CMD_Array[CMD_POSITION_DATA], configP, configLength);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_LOCAL_WRITECONFIG_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the local GPIO configuration of the module
 *
 *output:
 * -configP: pointer to one or more pin configurations
 * -configLengthP: length of data configP points to
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIOLocalReadConfig(ProteusIII_GPIOConfigBlock_t* configP, uint16_t *configLengthP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_LOCAL_READCONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= 0;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_LOCAL_READCONFIG_CNF, CMD_Status_Success, true);
        /* config length is packetlength - 1 (status byte)*/
        *configLengthP = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8) - 1;
        memcpy(configP, &RxPacket[CMD_POSITION_DATA+1], *configLengthP);
    }

    return ret;
}

/*
 *Set the output value of the local pin. Pin has to be configured first.
 *See ProteusIII_GPIOLocalWriteConfig
 *
 *input:
 * -controlP: pointer to one or more pin controls
 * -configLength: length of data configP controlP to
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIOLocalWrite(ProteusIII_GPIOControlBlock_t* controlP, uint16_t controlLength)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_LOCAL_WRITE_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (controlLength & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (controlLength & 0xFF00) >> 8;
    memcpy(&CMD_Array[CMD_POSITION_DATA], controlP, controlLength);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_LOCAL_WRITE_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the input of the pin. Pin has to be configured first.
 *See ProteusIII_GPIOLocalWriteConfig
 *
 *input:
 * -GPIOToReadP: One or more pins to read.
 * -amountGPIOToRead: amount of pins to read and therefore length of GPIOToRead
 *output:
 * -controlP: Pointer to controlBlock
 * -controlLengthP: length of controlP
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIOLocalRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIII_GPIOControlBlock_t* controlP, uint16_t* controlLengthP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_LOCAL_READ_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= amountGPIOToRead + 1;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;
    CMD_Array[CMD_POSITION_DATA] = amountGPIOToRead;
    memcpy(&CMD_Array[CMD_POSITION_DATA + 1], GPIOToReadP, amountGPIOToRead);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_LOCAL_READ_CNF, CMD_Status_Success, true);
        /* config length is packetlength - 1 (status byte)*/
        *controlLengthP = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8) - 1;
        memcpy(controlP, &RxPacket[CMD_POSITION_DATA+1], *controlLengthP);
    }

    return ret;
}

/*
 *Configure the remote GPIO of the module
 *
 *input:
 * -configP: pointer to one or more pin configurations
 * -configLength: length of data configP points to
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIORemoteWriteConfig(ProteusIII_GPIOConfigBlock_t* configP, uint16_t configLength)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (configLength & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (configLength & 0xFF00) >> 8;
    memcpy(&CMD_Array[CMD_POSITION_DATA], configP, configLength);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_REMOTE_WRITECONFIG_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the remote GPIO configuration of the module
 *
 *output:
 * -configP: pointer to one or more pin configurations
 * -configLengthP: length of data configP points to
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIORemoteReadConfig(ProteusIII_GPIOConfigBlock_t* configP, uint16_t *configLengthP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_REMOTE_READCONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= 0;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_REMOTE_READCONFIG_CNF, CMD_Status_Success, true);
        /* config length is packetlength - 1 (status byte)*/
        *configLengthP = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8) - 1;
        memcpy(configP, &RxPacket[CMD_POSITION_DATA+1], *configLengthP);
    }

    return ret;
}

/*
 *Set the output value of the remote pin. Pin has to be configured first.
 *See ProteusIII_GPIORemoteWriteConfig
 *
 *input:
 * -controlP: pointer to one or more pin controls
 * -configLength: length of data configP controlP to
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIORemoteWrite(ProteusIII_GPIOControlBlock_t* controlP, uint16_t controlLength)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_REMOTE_WRITE_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (controlLength & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (controlLength & 0xFF00) >> 8;
    memcpy(&CMD_Array[CMD_POSITION_DATA], controlP, controlLength);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_REMOTE_WRITE_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the input of the pins. Pin has to be configured first.
 *See ProteusIII_GPIORemoteWriteConfig
 *
 *input:
 * -GPIOToReadP: One or more pins to read.
 * -amountGPIOToRead: amount of pins to read and therefore length of GPIOToReadP
 *output:
 * -controlP: Pointer to controlBlock
 * -controlLengthP: length of controlP
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_GPIORemoteRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIII_GPIOControlBlock_t* controlP, uint16_t* controlLengthP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_CMD_GPIO_REMOTE_READ_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= amountGPIOToRead + 1;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;
    CMD_Array[CMD_POSITION_DATA] = amountGPIOToRead;
    memcpy(&CMD_Array[CMD_POSITION_DATA + 1], GPIOToReadP, amountGPIOToRead);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_CMD_GPIO_REMOTE_READ_CNF, CMD_Status_Success, true);
        /* config length is packetlength - 1 (status byte)*/
        *controlLengthP = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8) - 1;
        memcpy(controlP, &RxPacket[CMD_POSITION_DATA+1], *controlLengthP);
    }

    return ret;
}

