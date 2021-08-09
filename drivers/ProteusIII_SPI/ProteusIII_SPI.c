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
#include <stdlib.h>
#include <stdint.h>

#include <errno.h>
#include "string.h"


#include "ProteusIII_SPI.h"
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

#define ProteusIII_SPI_CMD_TYPE_REQ (uint8_t)(0 << 6)
#define ProteusIII_SPI_CMD_TYPE_CNF (uint8_t)(1 << 6)
#define ProteusIII_SPI_CMD_TYPE_IND (uint8_t)(2 << 6)
#define ProteusIII_SPI_CMD_TYPE_RSP (uint8_t)(3 << 6)


#define ProteusIII_SPI_CMD_RESET (uint8_t)0x00
#define ProteusIII_SPI_CMD_RESET_REQ (ProteusIII_SPI_CMD_RESET | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_RESET_CNF (ProteusIII_SPI_CMD_RESET | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_GETSTATE (uint8_t)0x01
#define ProteusIII_SPI_CMD_GETSTATE_REQ (ProteusIII_SPI_CMD_GETSTATE | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GETSTATE_CNF (ProteusIII_SPI_CMD_GETSTATE | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_GETSTATE_IND (ProteusIII_SPI_CMD_GETSTATE | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_SLEEP (uint8_t)0x02
#define ProteusIII_SPI_CMD_SLEEP_REQ (ProteusIII_SPI_CMD_SLEEP | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_SLEEP_CNF (ProteusIII_SPI_CMD_SLEEP | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_SLEEP_IND (ProteusIII_SPI_CMD_SLEEP | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_DATA (uint8_t)0x04
#define ProteusIII_SPI_CMD_DATA_REQ (ProteusIII_SPI_CMD_DATA | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_DATA_CNF (ProteusIII_SPI_CMD_DATA | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_DATA_IND (ProteusIII_SPI_CMD_DATA | ProteusIII_SPI_CMD_TYPE_IND)
#define ProteusIII_SPI_CMD_TXCOMPLETE_RSP (ProteusIII_SPI_CMD_DATA | ProteusIII_SPI_CMD_TYPE_RSP)

#define ProteusIII_SPI_CMD_CONNECT (uint8_t)0x06
#define ProteusIII_SPI_CMD_CONNECT_REQ (ProteusIII_SPI_CMD_CONNECT | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_CONNECT_CNF (ProteusIII_SPI_CMD_CONNECT | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_CONNECT_IND (ProteusIII_SPI_CMD_CONNECT | ProteusIII_SPI_CMD_TYPE_IND)
#define ProteusIII_SPI_CMD_CHANNELOPEN_RSP (ProteusIII_SPI_CMD_CONNECT | ProteusIII_SPI_CMD_TYPE_RSP)

#define ProteusIII_SPI_CMD_DISCONNECT (uint8_t)0x07
#define ProteusIII_SPI_CMD_DISCONNECT_REQ (ProteusIII_SPI_CMD_DISCONNECT | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_DISCONNECT_CNF (ProteusIII_SPI_CMD_DISCONNECT | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_DISCONNECT_IND (ProteusIII_SPI_CMD_DISCONNECT | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_SECURITY_IND (uint8_t)0x88

#define ProteusIII_SPI_CMD_SCANSTART (uint8_t)0x09
#define ProteusIII_SPI_CMD_SCANSTART_REQ (ProteusIII_SPI_CMD_SCANSTART  | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_SCANSTART_CNF (ProteusIII_SPI_CMD_SCANSTART  | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_SCANSTART_IND (ProteusIII_SPI_CMD_SCANSTART  | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_SCANSTOP (uint8_t)0x0A
#define ProteusIII_SPI_CMD_SCANSTOP_REQ (ProteusIII_SPI_CMD_SCANSTOP  | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_SCANSTOP_CNF (ProteusIII_SPI_CMD_SCANSTOP  | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_SCANSTOP_IND (ProteusIII_SPI_CMD_SCANSTOP  | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_GETDEVICES (uint8_t)0x0B
#define ProteusIII_SPI_CMD_GETDEVICES_REQ (ProteusIII_SPI_CMD_GETDEVICES  | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GETDEVICES_CNF (ProteusIII_SPI_CMD_GETDEVICES  | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_GETDEVICES_IND (ProteusIII_SPI_CMD_GETDEVICES  | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_PASSKEY (uint8_t)0x0D
#define ProteusIII_SPI_CMD_PASSKEY_REQ (ProteusIII_SPI_CMD_PASSKEY  | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_PASSKEY_CNF (ProteusIII_SPI_CMD_PASSKEY  | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_PASSKEY_IND (ProteusIII_SPI_CMD_PASSKEY  | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_GET (uint8_t)0x10
#define ProteusIII_SPI_CMD_GET_REQ (ProteusIII_SPI_CMD_GET | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GET_CNF (ProteusIII_SPI_CMD_GET | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_SET (uint8_t)0x11
#define ProteusIII_SPI_CMD_SET_REQ (ProteusIII_SPI_CMD_SET | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_SET_CNF (ProteusIII_SPI_CMD_SET | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_PHYUPDATE (uint8_t)0x1A
#define ProteusIII_SPI_CMD_PHYUPDATE_REQ (ProteusIII_SPI_CMD_PHYUPDATE | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_PHYUPDATE_CNF (ProteusIII_SPI_CMD_PHYUPDATE | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_PHYUPDATE_IND (ProteusIII_SPI_CMD_PHYUPDATE | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_FACTORYRESET (uint8_t)0x1C
#define ProteusIII_SPI_CMD_FACTORYRESET_REQ (ProteusIII_SPI_CMD_FACTORYRESET | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_FACTORYRESET_CNF (ProteusIII_SPI_CMD_FACTORYRESET | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_NUMERIC_COMP (uint8_t)0x24
#define ProteusIII_SPI_CMD_NUMERIC_COMP_REQ    (ProteusIII_SPI_CMD_NUMERIC_COMP | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_NUMERIC_COMP_CNF    (ProteusIII_SPI_CMD_NUMERIC_COMP | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_DISPLAY_PASSKEY_IND (ProteusIII_SPI_CMD_NUMERIC_COMP | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG (uint8_t)0x25
#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG_REQ (ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG_CNF (ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG (uint8_t)0x2B
#define ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG_REQ (ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG_CNF (ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE (uint8_t)0x26
#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE_REQ (ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE_CNF (ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE_IND (ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_GPIO_LOCAL_READ (uint8_t)0x27
#define ProteusIII_SPI_CMD_GPIO_LOCAL_READ_REQ (ProteusIII_SPI_CMD_GPIO_LOCAL_READ | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_LOCAL_READ_CNF (ProteusIII_SPI_CMD_GPIO_LOCAL_READ | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG (uint8_t)0x28
#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG_REQ (ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG_CNF (ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG_IND (ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG (uint8_t)0x2C
#define ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG_REQ (ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG_CNF (ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE (uint8_t)0x29
#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE_REQ (ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE_CNF (ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE | ProteusIII_SPI_CMD_TYPE_CNF)
#define ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE_IND (ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE | ProteusIII_SPI_CMD_TYPE_IND)

#define ProteusIII_SPI_CMD_GPIO_REMOTE_READ (uint8_t)0x2A
#define ProteusIII_SPI_CMD_GPIO_REMOTE_READ_REQ (ProteusIII_SPI_CMD_GPIO_REMOTE_READ | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_GPIO_REMOTE_READ_CNF (ProteusIII_SPI_CMD_GPIO_REMOTE_READ | ProteusIII_SPI_CMD_TYPE_CNF)

#define ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS        (uint8_t)0x2D
#define ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS_REQ    (ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS | ProteusIII_SPI_CMD_TYPE_REQ)
#define ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS_CNF    (ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS | ProteusIII_SPI_CMD_TYPE_CNF)

#define CMD_ARRAY_SIZE() ((((uint16_t)CMD_Array[CMD_POSITION_LENGTH_LSB] << 0) | ((uint16_t)CMD_Array[CMD_POSITION_LENGTH_MSB] << 8)) + LENGTH_CMD_OVERHEAD)

/* type used to check the response, when a command was sent to the ProteusIII_SPI */
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
    CMD_Status_t status; /* variable used to check the response (*_CNF), when a request (*_REQ) was sent to the ProteusIII_SPI */
} CMD_Confirmation_t;

/**************************************
 *     Static function declarations   *
 **************************************/

static void HandleRxPacket(uint8_t*RxBuffer);                                                      /* RX packet interpreter */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate);
static bool FillChecksum(uint8_t* array, uint16_t length);                                           /* add the CS needed to finalize the command */
static bool InitDriver(ProteusIII_SPI_CallbackConfig_t callbackConfig);

/* functions to handle the chip select for SPI */
#define CS_CLK_DELAY_US 6

static bool SPI_CS_Init();
static bool SPI_CS_Deinit();
static bool SPI_CS_SetActive();
static bool SPI_CS_SetInactive();
static bool SendCommand(uint8_t *pCmd, uint16_t cmdLength);

/**************************************
 *          Static variables          *
 **************************************/
static uint8_t CMD_Array[MAX_CMD_LENGTH]; /* for TX to module*/
static uint8_t RxPacket[MAX_CMD_LENGTH];

bool AbortSpiRxThread;                     /* boolean to abort the SPI RX thread */
bool ResetSpiRxThread;                     /* boolean to reset the SPI RX thread */

pthread_t thread_read;

#define CMDCONFIRMATIONARRAY_LENGTH 5

CMD_Confirmation_t cmdConfirmation_array[CMDCONFIRMATIONARRAY_LENGTH];

static ProteusIII_SPI_GetDevices_t* ProteusIII_SPI_GetDevicesP = NULL;
static ProteusIII_SPI_States_t ble_state;
static bool askedForState;

int reset_pin = 0;                              /* reset pin number */
int spi_int_wakeup_pin = 0;                     /* wakeup and SPI interrupt pin number */
int boot_pin = 0;                               /* boot pin number */

SetPin_Out_t spi_int_active_level = SetPin_Out_High;

ProteusIII_SPI_CallbackConfig_t callbacks;
/**************************************
 *         Static functions           *
 **************************************/
/* thread function to receive bytes from interface */
void *rx_thread()
{
    uint8_t checksum = 0;
    uint16_t RxByteCounter = 0;
    uint16_t BytesToReceive = 0;
    static uint8_t RxBuffer[MAX_CMD_LENGTH]; /* For UART RX from module */
    memset(&RxBuffer[0],0,MAX_CMD_LENGTH);
    ResetSpiRxThread = false;
    AbortSpiRxThread = false;
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_UARTRXTHREAD);
    while(1)
    {
        /* wait for 1ms, then check if new RX data is available */
        delay (1);

        if(AbortSpiRxThread == true)
        {
            /* jump out of the while loop and finish the thread */
            AbortSpiRxThread = false;
            break;
        }

        if(ResetSpiRxThread == true)
        {
            /* reset the RX thread */
            ResetSpiRxThread = false;
            RxByteCounter = 0;
        }

        if (true == BytesAvailable())
        {
            RxByteCounter = 0;
            BytesToReceive = 0;

            if(true == SPI_CS_SetActive())
            {
                delay_us(CS_CLK_DELAY_US);

                /* Get first 4 bytes - STX, CMD, Length */
                if(true == ReadBytesN(&RxBuffer[0], LENGTH_CMD_OVERHEAD_WITHOUT_CRC))
                {
                    /* wait for start byte of frame */
                    if (RxBuffer[0] == CMD_STX)
                    {
                        /* See length to determine data left */
                        BytesToReceive = (((uint16_t)RxBuffer[CMD_POSITION_LENGTH_MSB]<<8)) + (uint16_t)(RxBuffer[CMD_POSITION_LENGTH_LSB]) + 1 ; /* len_msb + len_lsb. Only data bytes without the CMD overhead */
                        RxByteCounter = LENGTH_CMD_OVERHEAD_WITHOUT_CRC;
                    }
                    else
                    {
                        /* error */
                        SPI_CS_SetInactive();
                        continue;
                    }
                }
                else
                {
                    /* error */
                    SPI_CS_SetInactive();
                    continue;
                }
            }
            else
            {
                /* error */
                SPI_CS_SetInactive();
                continue;
            }

            /* BytesToReceive is 0, if first part was not received */
            if((0 < BytesToReceive) && (BytesToReceive <= MAX_PAYLOAD_LENGTH))
            {
                /* Get rest of frame */
                if(true == ReadBytesN(&RxBuffer[LENGTH_CMD_OVERHEAD_WITHOUT_CRC], BytesToReceive))
                {
                    RxByteCounter += BytesToReceive;
                    /* data field */
                    if (RxByteCounter == (BytesToReceive + LENGTH_CMD_OVERHEAD_WITHOUT_CRC))
                    {
                        /* check CRC */
                        checksum = 0;
                        int i = 0;
                        for (i = 0; i < (RxByteCounter - 1); i++)
                        {
                            checksum ^= RxBuffer[i];
                        }

                        if (checksum == RxBuffer[RxByteCounter - 1])
                        {
                            /* received frame ok, interprete it now */
                            HandleRxPacket(RxBuffer);
                        }
                    }
                }
            }

            /* Set CS to inactive, once reception is done */
            SPI_CS_SetInactive();
        }
    }
    return 0;
}

/*
 *Initialize the ProteusIII_SPI and driver
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
static bool InitDriver(ProteusIII_SPI_CallbackConfig_t callbackConfig)
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
    ble_state = ProteusIII_SPI_State_BLE_Invalid;
    ProteusIII_SPI_GetDevicesP = NULL;

    /* start RX thread */
    if(pthread_create(&thread_read, NULL, &rx_thread, NULL))
    {
        fprintf(stdout, "Failed to start rx_Thread\n");
        ProteusIII_SPI_Deinit();
        return false;
    }

    /* reset module*/
    if(ProteusIII_SPI_PinReset())
    {
        delay(300);
    }
    else
    {
        fprintf(stdout, "Pin Reset failed\n");
        ProteusIII_SPI_Deinit();
        return false;
    }

    uint8_t driver_version[3];
    if(GetDriverVersion(driver_version))
    {
        fprintf (stdout, COLOR_CYAN "ProteusIII_SPI driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(100);


    uint8_t firmware_version[3];
    if(ProteusIII_SPI_GetFWVersion(firmware_version))
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
    case ProteusIII_SPI_CMD_GETDEVICES_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        if((cmdConfirmation.status == CMD_Status_Success)&&(ProteusIII_SPI_GetDevicesP != NULL))
        {
            uint8_t size = RxPacket[CMD_POSITION_DATA+1];

            if (size >= MAX_NUMBER_OF_DEVICES)
            {
                size = MAX_NUMBER_OF_DEVICES;
            }
            ProteusIII_SPI_GetDevicesP->numberofdevices = size;

            int i;
            int len = CMD_POSITION_DATA+2;
            for(i=0; i<ProteusIII_SPI_GetDevicesP->numberofdevices; i++)
            {
                memcpy(&ProteusIII_SPI_GetDevicesP->devices[i].btmac[0],&RxPacket[len],6);
                ProteusIII_SPI_GetDevicesP->devices[i].rssi = RxPacket[len+6];
                ProteusIII_SPI_GetDevicesP->devices[i].txpower = RxPacket[len+7];
                ProteusIII_SPI_GetDevicesP->devices[i].devicenamelength = RxPacket[len+8];
                memcpy(&ProteusIII_SPI_GetDevicesP->devices[i].devicename[0],&RxPacket[len+9],ProteusIII_SPI_GetDevicesP->devices[i].devicenamelength);
                len += (9+ProteusIII_SPI_GetDevicesP->devices[i].devicenamelength);
            }
        }
        break;
    }
    case ProteusIII_SPI_CMD_RESET_CNF:
    case ProteusIII_SPI_CMD_SCANSTART_CNF:
    case ProteusIII_SPI_CMD_SCANSTOP_CNF:
    case ProteusIII_SPI_CMD_GET_CNF:
    case ProteusIII_SPI_CMD_SET_CNF:
    case ProteusIII_SPI_CMD_PASSKEY_CNF:
    case ProteusIII_SPI_CMD_PHYUPDATE_CNF:
    case ProteusIII_SPI_CMD_CONNECT_CNF:
    case ProteusIII_SPI_CMD_DATA_CNF:
    case ProteusIII_SPI_CMD_DISCONNECT_CNF:
    case ProteusIII_SPI_CMD_FACTORYRESET_CNF:
    case ProteusIII_SPI_CMD_SLEEP_CNF:
    case ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG_CNF:
    case ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG_CNF:
    case ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE_CNF:
    case ProteusIII_SPI_CMD_GPIO_LOCAL_READ_CNF:
    case ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG_CNF:
    case ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG_CNF:
    case ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE_CNF:
    case ProteusIII_SPI_CMD_GPIO_REMOTE_READ_CNF:
    case ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS_CNF:
    case ProteusIII_SPI_CMD_TXCOMPLETE_RSP:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        cmdConfirmation.status = RxPacket[CMD_POSITION_DATA];
        break;
    }

    case ProteusIII_SPI_CMD_GETSTATE_CNF:
    {
        cmdConfirmation.cmd = RxPacket[CMD_POSITION_CMD];
        /* GETSTATE_CNF has no status field*/
        cmdConfirmation.status = CMD_Status_NoStatus;
        break;
    }

    case ProteusIII_SPI_CMD_CHANNELOPEN_RSP:
    {
        /* Payload of CHANNELOPEN_RSP: Status (1 byte), BTMACH (6 byte), Max Payload (1byte)*/
        ble_state = ProteusIII_SPI_State_BLE_Channel_Open;
        if(callbacks.channelOpenCb != NULL)
        {
            callbacks.channelOpenCb(&RxPacket[CMD_POSITION_DATA+1], (uint16_t)RxPacket[CMD_POSITION_DATA + 7]);
        }
        break;
    }

    case ProteusIII_SPI_CMD_CONNECT_IND:
    {
        ble_state = ProteusIII_SPI_State_BLE_Connected;
        if(callbacks.connectCp != NULL)
        {
            callbacks.connectCp(&RxPacket[CMD_POSITION_DATA+1]);
        }
        break;
    }

    case ProteusIII_SPI_CMD_DISCONNECT_IND:
    {
        if(callbacks.disconnectCb != NULL)
        {
            callbacks.disconnectCb();
        }
        break;
    }


    case ProteusIII_SPI_CMD_DATA_IND:
    {
        if(callbacks.rxCb != NULL)
        {
            uint16_t payload_length = ((((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) | ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8))) - 7;
            callbacks.rxCb(&RxPacket[CMD_POSITION_DATA + 7], payload_length, &RxPacket[CMD_POSITION_DATA], RxPacket[CMD_POSITION_DATA + 6]);
        }
        break;
    }

    case ProteusIII_SPI_CMD_SECURITY_IND:
    {
        if(callbacks.securityCb != NULL)
        {
            callbacks.securityCb(&RxPacket[CMD_POSITION_DATA+1],RxPacket[CMD_POSITION_DATA]);
        }
        break;
    }

    case ProteusIII_SPI_CMD_PASSKEY_IND:
    {
        if(callbacks.passkeyCb != NULL)
        {
            callbacks.passkeyCb(&RxPacket[CMD_POSITION_DATA+1]);
        }
        break;
    }

    case ProteusIII_SPI_CMD_DISPLAY_PASSKEY_IND:
    {
        if(callbacks.displayPasskeyCb != NULL)
        {
            callbacks.displayPasskeyCb((ProteusIII_SPI_DisplayPasskeyAction_t)RxPacket[CMD_POSITION_DATA],&RxPacket[CMD_POSITION_DATA+1],&RxPacket[CMD_POSITION_DATA+7]);
        }
        break;
    }

    case ProteusIII_SPI_CMD_PHYUPDATE_IND:
    {
        if(callbacks.phyUpdateCb != NULL)
        {
            callbacks.phyUpdateCb(&RxPacket[CMD_POSITION_DATA+3],(ProteusIII_SPI_Phy_t)RxPacket[CMD_POSITION_DATA+1],(ProteusIII_SPI_Phy_t)RxPacket[CMD_POSITION_DATA+2]);
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

/* function that waits for the return value of ProteusIII_SPI (*_CNF), when a command (*_REQ) was sent before */
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

/* function to initialize the chip select pin for SPI */
static bool SPI_CS_Init()
{
    if(false == InitPin(ProteusIII_SPI_PIN_SPI_CS))
    {
        Debug_out("SPI_CS_Init: Could not init pin", false);
        return false;
    }

    if(false == SPI_CS_SetInactive())
    {
        Debug_out("SPI_CS_Init: Could not clear pin", false);
        return false;
    }

    return true;
}

/* function to deinitialize the chip select pin for SPI */
static bool SPI_CS_Deinit()
{
    if(false == SetPin(ProteusIII_SPI_PIN_SPI_CS, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_Low))
    {
        Debug_out("SPI_CS_Deinit: Could not set cs pin to input low", false);
        return false;
    }

    if(false == DeinitPin(ProteusIII_SPI_PIN_SPI_CS))
    {
        Debug_out("SPI_CS_Deinit: Could not deinit cs pin", false);
        return false;
    }

    return true;
}

/* function to set the chip select pin for SPI to active (low)*/
static bool SPI_CS_SetActive()
{
    if(false == SetPin(ProteusIII_SPI_PIN_SPI_CS, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low))
    {
        Debug_out("SPI_CS_SetActive: Could not set pin to active", false);
        return false;
    }

    return true;
}

/* function to set the chip select pin for SPI to inactive (high)*/
static bool SPI_CS_SetInactive()
{
    if(false == SetPin(ProteusIII_SPI_PIN_SPI_CS, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High))
    {
        Debug_out("SPI_CS_SetInactive: Could not set pin to inactive", false);
        return false;
    }

    return true;
}

/* function to send the command to ProteusIII_SPI. The function will handle the chip select pin. */
static bool SendCommand(uint8_t *pCmd, uint16_t cmdLength)
{
    if(NULL == pCmd)
    {
        return false;
    }

    if(false == SPI_CS_SetActive())
    {
        return false;
    }

    delay_us(CS_CLK_DELAY_US);

    SendBytes(pCmd, cmdLength);

    if(false == SPI_CS_SetInactive())
    {
        return false;
    }

    return true;

}
/**************************************
 *         Global functions           *
 **************************************/
bool BytesAvailable()
{
    /* Get current level of interrupt pin */
    SetPin_Out_t pinLevel = GetPinLevel(spi_int_wakeup_pin);

    /* Check if level indicates available data */
    return (spi_int_active_level == pinLevel);
}

/*
 *Initialize the ProteusIII_SPI interface for serial interface
 *
 *input:
 * -index:          defines the SPI mode
 * -datarate_bit:   data rate of the interface in bit/s
 * -rp:             Reset Pin
 * -wp:             Wake-up and SPI_Int Pin
 * -bp:             Boot Pin
 * -callbackConfig  Function pointers for callbacks
 *
 *Caution: the parameter baudrate must match the configured UserSettings of the ProteusIII_SPI
 *         -the baudrate parameter must match to perform a successful FTDI communication
 *          *updating this parameter during runtime may lead to communication errors
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_Init(ProteusIII_SPI_Config_t index, int datarate_bit, int rp, int wp, int bp, ProteusIII_SPI_CallbackConfig_t callbackConfig)
{
    if (false == InitSerial())
    {
        /* error */
        return false;
    }

    Serial_Mode_t serial_mode = Serial_Mode_2;
    switch(index)
    {
        case ProteusIII_SPI_ConfigIndex_Mode0_activehigh:
        {
            spi_int_active_level = SetPin_Out_High;
            serial_mode = Serial_Mode_0;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode1_activehigh:
        {
            spi_int_active_level = SetPin_Out_High;
            serial_mode = Serial_Mode_1;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode2_activehigh:
        {
            spi_int_active_level = SetPin_Out_High;
            serial_mode = Serial_Mode_2;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode3_activehigh:
        {
            spi_int_active_level = SetPin_Out_High;
            serial_mode = Serial_Mode_3;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode0_activelow:
        {
            spi_int_active_level = SetPin_Out_Low;
            serial_mode = Serial_Mode_0;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode1_activelow:
        {
            spi_int_active_level = SetPin_Out_Low;
            serial_mode = Serial_Mode_1;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode2_activelow:
        {
            spi_int_active_level = SetPin_Out_Low;
            serial_mode = Serial_Mode_2;
        }
        break;
        case ProteusIII_SPI_ConfigIndex_Mode3_activelow:
        {
            spi_int_active_level = SetPin_Out_Low;
            serial_mode = Serial_Mode_3;
        }
        break;
        default:
        {
            return false;
        }
    }


    if (false == OpenSPI(datarate_bit, serial_mode))
    {
        /* error */
        return false;
    }

    /* initialize the SPI chip select pin */
    if (false == SPI_CS_Init())
    {
        /* error */
        return false;
    }

    /* initialize the boot pin */
    if (false == InitPin(bp))
    {
        /* error */
        return false;
    }
    boot_pin = bp;
    SetPin(boot_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);

    /* initialize the wakeup pin /spi interrupt pin */
    if (false == InitPin(wp))
    {
        /* error */
        return false;
    }
    spi_int_wakeup_pin = wp;
    /* set pin to input to detect the SPI interrupt function */
    SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_Low);

    /* Init */

    /* initialize the reset pin,
     * we have to define it as input/pull-up such that the reset button
     * can pull the pin level down if the button is pressed
     */
    if (false == InitPin(rp))
    {
        /* error */
        return false;
    }
    reset_pin = rp;
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);
    delay(10);

    /* init the driver and module*/
    return InitDriver(callbackConfig);
}

/*
 *Deinitialize the ProteusIII_SPI interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_Deinit()
{
    /* close the communication interface to the module */
    CloseSerial();
    DeinitSerial();
    if(false == SPI_CS_Deinit())
    {
        Debug_out("CloseSerial: Could not deinit cs pin", false);
    }
    /* abort RX thread */
    AbortSpiRxThread = true;

    /* deinit pins */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    DeinitPin(reset_pin);
    SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    DeinitPin(spi_int_wakeup_pin);
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
 *Wakeup the ProteusIII_SPI from sleep by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_PinWakeup()
{
    int i = 0;

    for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
    {
        cmdConfirmation_array[i].status = CMD_Status_Invalid;
        cmdConfirmation_array[i].cmd = CNFINVALID;
    }
    /* wake-up */
    if(spi_int_active_level == SetPin_Out_High)
    {
        SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
    }
    else
    {
        SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    }

    delay (5);

    /* set pin to input to detect the SPI interrupt function */
    SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_Low);

    /* wait for cnf */
    return Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GETSTATE_CNF, CMD_Status_NoStatus, false);
}

/*
 *Reset the ProteusIII_SPI by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_PinReset()
{
    /* set to output mode */
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
    delay (5);
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    ResetSpiRxThread = true;
    delay (5);
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);

    /* set to input mode again */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);

    /* wait for cnf */
    return Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true);
}

/*
 *Reset the ProteusIII_SPI by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_Reset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_RESET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        return Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true);
    }
    return ret;
}

/*
 *Disconnect the ProteusIII_SPI connection if open
 *
 *return true if disconnection succeeded
 *       false otherwise
 */
bool
ProteusIII_SPI_Disconnect()
{
    bool ret = false;
    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_DISCONNECT_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* Confirmation is sent before perfoming the disconnect. After disconnect, module sends disconnect indication */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_DISCONNECT_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Put the ProteusIII_SPI into sleep mode
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIII_SPI_Sleep()
{
    bool ret = false;
    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_SLEEP_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand( CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_SLEEP_CNF, CMD_Status_Success, true);
    }

    /* set pin to output to run as wake-up pin */
    if(spi_int_active_level == SetPin_Out_High)
    {
        /* Alternatively set pin to input and pull_down */
        SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    }
    else
    {
        /* Alternatively set pin to input and pull_up */
        SetPin(spi_int_wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
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
ProteusIII_SPI_Transmit(uint8_t *PayloadP, uint16_t length)
{
    bool ret = false;
    if ((length < MAX_PAYLOAD_LENGTH)&&(ProteusIII_SPI_State_BLE_Channel_Open == ProteusIII_SPI_GetDriverState()))
    {
        CMD_Array[CMD_POSITION_STX] = CMD_STX;
        CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_DATA_REQ;
        CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t) (length >> 0);
        CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t) (length >> 8);

        memcpy(&CMD_Array[CMD_POSITION_DATA], PayloadP, length);

        if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
        {
            SendCommand( CMD_Array, CMD_ARRAY_SIZE());
            ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_TXCOMPLETE_RSP, CMD_Status_Success, true);
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
ProteusIII_SPI_FactoryReset()
{
    bool ret = false;
    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_FACTORYRESET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for reset after factory reset */
        return Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true);
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
ProteusIII_SPI_Set(ProteusIII_SPI_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_SET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t) (1 + length);
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_DATA] = userSetting;
    memcpy(&CMD_Array[CMD_POSITION_DATA + 1], ValueP, length);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        return Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_SET_CNF, CMD_Status_Success, true);
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
ProteusIII_SPI_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_DEVICE_NAME, deviceNameP, nameLength);
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
ProteusIII_SPI_SetAdvertisingTimeout(uint16_t advTimeout)
{
    uint8_t help[2];
    memcpy(help,(uint8_t*)&advTimeout,2);
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT, help, 2);
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
ProteusIII_SPI_SetCFGFlags(uint16_t cfgflags)
{
    uint8_t help[2];
    memcpy(help,(uint8_t*)&cfgflags,2);
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_CFGFLAGS, help, 2);
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
ProteusIII_SPI_SetConnectionTiming(ProteusIII_SPI_ConnectionTiming_t connectionTiming)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_CONNECTION_TIMING, (uint8_t*)&connectionTiming, 1);
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
ProteusIII_SPI_SetScanTiming(ProteusIII_SPI_ScanTiming_t scanTiming)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_SCAN_TIMING, (uint8_t*)&scanTiming, 1);
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
ProteusIII_SPI_SetTXPower(ProteusIII_SPI_TXPower_t txpower)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_TX_POWER, (uint8_t*)&txpower, 1);
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
ProteusIII_SPI_SetSecFlags(ProteusIII_SPI_SecFlags_t secflags)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_SEC_FLAGS, (uint8_t*)&secflags, 1);
}

/*
 *Set the SPI config index
 *
 *input:
 * -index: SPI config index
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_SetBaudrateIndex(ProteusIII_SPI_Config_t index)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_SPI_CONFIGINDEX, (uint8_t*)&index, 1);
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
ProteusIII_SPI_SetStaticPasskey(uint8_t *staticPasskeyP)
{
    return ProteusIII_SPI_Set(ProteusIII_SPI_USERSETTING_POSITION_RF_STATIC_PASSKEY, staticPasskeyP, 6);
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
ProteusIII_SPI_Get(ProteusIII_SPI_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GET_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)1;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_DATA] = userSetting;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GET_CNF, CMD_Status_Success, true))
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
ProteusIII_SPI_GetFWVersion(uint8_t *versionP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_FS_FWVersion, versionP, &length);
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
ProteusIII_SPI_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP)
{
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_DEVICE_NAME, deviceNameP, nameLengthP);
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
ProteusIII_SPI_GetMAC(uint8_t *MacP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_FS_MAC, MacP, &length);
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
ProteusIII_SPI_GetBTMAC(uint8_t *BTMacP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_FS_BTMAC, BTMacP, &length);
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
ProteusIII_SPI_GetAdvertisingTimeout(uint16_t *advTimeoutP)
{
    uint16_t length;
    bool ret = false;
    uint8_t help[2];

    ret = ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT, help, &length);
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
ProteusIII_SPI_GetConnectionTiming(ProteusIII_SPI_ConnectionTiming_t *connectionTimingP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_CONNECTION_TIMING, (uint8_t*)connectionTimingP, &length);
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
ProteusIII_SPI_GetScanTiming(ProteusIII_SPI_ScanTiming_t *scanTimingP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_SCAN_TIMING, (uint8_t*)scanTimingP, &length);
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
ProteusIII_SPI_GetTXPower(ProteusIII_SPI_TXPower_t *txpowerP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_TX_POWER, (uint8_t*)txpowerP, &length);
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
ProteusIII_SPI_GetSecFlags(ProteusIII_SPI_SecFlags_t *secflagsP)
{
    uint16_t length;
    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_SEC_FLAGS, (uint8_t*)secflagsP, &length);
}

/*
 *Request the SPI config index
 *
 *output:
 * -indexP: pointer to the SPI config index
 *
 *return true if request succeeded
*       false otherwise
*/
bool ProteusIII_SPI_GetSPIConfigIndex(ProteusIII_SPI_Config_t *indexP)
{
    uint16_t length;

    return ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_SPI_CONFIGINDEX, (uint8_t*)&indexP, &length);
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
ProteusIII_SPI_GetCFGFlags(uint16_t *cfgflags)
{
    uint16_t length;
    bool ret = false;
    uint8_t help[2];

    ret = ProteusIII_SPI_Get(ProteusIII_SPI_USERSETTING_POSITION_RF_CFGFLAGS, help, &length);
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
ProteusIII_SPI_GetState(ProteusIII_SPI_BLE_Role_t *BLE_roleP, ProteusIII_SPI_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GETSTATE_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());
        askedForState = true;
        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GETSTATE_CNF, CMD_Status_NoStatus, true))
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
ProteusIII_SPI_States_t ProteusIII_SPI_GetDriverState()
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
ProteusIII_SPI_Scanstart()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_SCANSTART_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_SCANSTART_CNF, CMD_Status_Success, true);
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
ProteusIII_SPI_Scanstop()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_SCANSTOP_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_SCANSTOP_CNF, CMD_Status_Success, true);
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
ProteusIII_SPI_GetDevices(ProteusIII_SPI_GetDevices_t* devicesP)
{
    bool ret = false;

    ProteusIII_SPI_GetDevicesP = devicesP;
    if (ProteusIII_SPI_GetDevicesP != NULL)
    {
        ProteusIII_SPI_GetDevicesP->numberofdevices = 0;
    }

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GETDEVICES_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GETDEVICES_CNF, CMD_Status_Success, true);
    }

    ProteusIII_SPI_GetDevicesP = NULL;

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
bool ProteusIII_SPI_Connect(uint8_t *btmac)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_CONNECT_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)6;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    memcpy(&CMD_Array[CMD_POSITION_DATA], btmac, 6);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(3000, ProteusIII_SPI_CMD_CONNECT_CNF, CMD_Status_Success, true);
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
bool ProteusIII_SPI_Passkey(uint8_t* passkey)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_PASSKEY_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)6;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    memcpy(&CMD_Array[CMD_POSITION_DATA], passkey, 6);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_PASSKEY_CNF, CMD_Status_Success, true);
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
bool ProteusIII_SPI_NumericCompareConfirm(bool keyIsOk)
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
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_NUMERIC_COMP_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)1;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_DATA] = lescStatus;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_NUMERIC_COMP_CNF, CMD_Status_Success, true);
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
bool ProteusIII_SPI_PhyUpdate(ProteusIII_SPI_Phy_t phy)
{
    bool ret = false;

    if (ProteusIII_SPI_State_BLE_Channel_Open == ProteusIII_SPI_GetDriverState())
    {
        /* fill CMD_ARRAY packet */
        CMD_Array[CMD_POSITION_STX] = CMD_STX;
        CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_PHYUPDATE_REQ;
        CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)1;
        CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;
        CMD_Array[CMD_POSITION_DATA] = (uint8_t)phy;

        if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
        {
            /* now send CMD_ARRAY */
            SendCommand(CMD_Array, CMD_ARRAY_SIZE());

            /* wait for cnf */
            ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_PHYUPDATE_CNF, CMD_Status_Success, true);
        }
    }
    return ret;
}

/*
 *Configure the local GPIO of the module
 *
 *input:
 * -configP: pointer to one or more pin configurations
 * -number_of_configs: number of entries in configP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIOLocalWriteConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t number_of_configs)
{
    bool ret = false;
	uint16_t length = 0;

	for (uint16_t i=0; i < number_of_configs; i++)
	{
		switch(configP->function)
		{
			case ProteusIII_SPI_GPIO_IO_Disconnected:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 3;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				CMD_Array[CMD_POSITION_DATA + length + 3] = 0x00;
				length += 4;
			}
			break;
			case ProteusIII_SPI_GPIO_IO_Input:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 3;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				CMD_Array[CMD_POSITION_DATA + length + 3] = configP->value.input;
				length += 4;
			}
			break;
			case ProteusIII_SPI_GPIO_IO_Output:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 3;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				CMD_Array[CMD_POSITION_DATA + length + 3] = configP->value.output;
				length += 4;
				}
			break;
			case ProteusIII_SPI_GPIO_IO_PWM:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 5;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				memcpy(&CMD_Array[CMD_POSITION_DATA + length + 3], &configP->value.pwm.period, 2);
				CMD_Array[CMD_POSITION_DATA + length + 5] = configP->value.pwm.ratio;
				length += 6;
			}
			break;
			default:
			{
			}
			break;
		}

		/* Move pointer to next element. configP is increased by sizeof(ProteusIII_SPI_GPIOConfigBlock_t)*/
		configP++;
	}

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (length & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (length & 0xFF00) >> 8;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_LOCAL_WRITECONFIG_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the local GPIO configuration of the module
 *
 *output:
 * -configP: pointer to one or more pin configurations
 * -number_of_configsP: pointer to number of entries in configP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIOLocalReadConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t *number_of_configsP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= 0;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_LOCAL_READCONFIG_CNF, CMD_Status_Success, true);

		if(ret == true)
		{
			uint16_t length = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8);

			*number_of_configsP = 0;
			uint8_t* uartP = &RxPacket[CMD_POSITION_DATA+1];
			ProteusIII_SPI_GPIOConfigBlock_t* configP_running = configP;
			while(uartP < &RxPacket[CMD_POSITION_DATA+length])
			{
				switch(*(uartP + 2))
				{
					case ProteusIII_SPI_GPIO_IO_Disconnected:
					{
						if(*uartP == 3)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					case ProteusIII_SPI_GPIO_IO_Input:
					{
						if(*uartP == 3)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);
							configP_running->value.input = *(uartP + 3);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					case ProteusIII_SPI_GPIO_IO_Output:
					{
						if(*uartP == 3)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);
							configP_running->value.output = *(uartP + 3);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					case ProteusIII_SPI_GPIO_IO_PWM:
					{
						if(*uartP == 5)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);
							memcpy(&configP_running->value.pwm.period, (uartP + 3), 2);
							configP_running->value.pwm.ratio = *(uartP + 5);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					default:
					{

					}
					break;
				}

				uartP += *uartP + 1;
			}
		}

    }

    return ret;
}

/*
 *Set the output value of the local pin. Pin has to be configured first.
 *See ProteusIII_SPI_GPIOLocalWriteConfig
 *
 *input:
 * -controlP: pointer to one or more pin controls
 * -number_of_controls: number of entries in controlP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIOLocalWrite(ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t number_of_controls)
{
    bool ret = false;
	uint16_t length = 0;

	for (uint16_t i=0; i < number_of_controls; i++)
	{
		CMD_Array[CMD_POSITION_DATA + length] = 2;
		CMD_Array[CMD_POSITION_DATA + length + 1] = controlP->GPIO_ID;
		CMD_Array[CMD_POSITION_DATA + length + 2] = controlP->value.output;
		length += 3;

        /* Move pointer to next element. configP is increased by sizeof(ProteusIII_SPI_GPIOControlBlock_t)*/
		controlP++;
	}

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (length & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (length & 0xFF00) >> 8;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_LOCAL_WRITE_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the input of the pin. Pin has to be configured first.
 *See ProteusIII_SPI_GPIOLocalWriteConfig
 *
 *input:
 * -GPIOToReadP: One or more pins to read.
 * -amountGPIOToRead: amount of pins to read and therefore length of GPIOToRead
 *output:
 * -controlP: Pointer to controlBlock
 * -number_of_controlsP: pointer to number of entries in controlP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIOLocalRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t* number_of_controlsP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_LOCAL_READ_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= amountGPIOToRead + 1;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;
    CMD_Array[CMD_POSITION_DATA] = amountGPIOToRead;
    memcpy(&CMD_Array[CMD_POSITION_DATA + 1], GPIOToReadP, amountGPIOToRead);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_LOCAL_READ_CNF, CMD_Status_Success, true);

		if(ret)
		{
			uint16_t length = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8);

			*number_of_controlsP = 0;
			uint8_t* uartP = &RxPacket[CMD_POSITION_DATA+1];
			ProteusIII_SPI_GPIOControlBlock_t* controlP_running = controlP;
			while(uartP < &RxPacket[CMD_POSITION_DATA+length])
			{
			    /* each ControlBlock starts with length field which is currently fixed to "2" */
				if(*uartP == 2)
				{
					controlP_running->GPIO_ID = *(uartP + 1);
					controlP_running->value.output = *(uartP + 2);

                    /* Move pointer to next element. configP is increased by sizeof(ProteusIII_SPI_GPIOControlBlock_t)*/
					controlP_running++;
					*number_of_controlsP += 1;
				}

				/* uartP points to length field of control block. So increase address by value of length + 1 */
				uartP += *uartP + 1;
			}
		}
    }

    return ret;
}

/*
 *Configure the remote GPIO of the module
 *
 *input:
 * -configP: pointer to one or more pin configurations
 * -number_of_configs: number of entries in configP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIORemoteWriteConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t number_of_configs)
{
    bool ret = false;
	uint16_t length = 0;

	for (uint16_t i=0; i < number_of_configs; i++)
	{
		switch(configP->function)
		{
			case ProteusIII_SPI_GPIO_IO_Disconnected:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 3;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				CMD_Array[CMD_POSITION_DATA + length + 3] = 0x00;
				length += 4;
			}
			break;
			case ProteusIII_SPI_GPIO_IO_Input:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 3;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				CMD_Array[CMD_POSITION_DATA + length + 3] = configP->value.input;
				length += 4;
			}
			break;
			case ProteusIII_SPI_GPIO_IO_Output:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 3;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				CMD_Array[CMD_POSITION_DATA + length + 3] = configP->value.output;
				length += 4;
				}
			break;
			case ProteusIII_SPI_GPIO_IO_PWM:
			{
				CMD_Array[CMD_POSITION_DATA + length] = 5;
				CMD_Array[CMD_POSITION_DATA + length + 1] = configP->GPIO_ID;
				CMD_Array[CMD_POSITION_DATA + length + 2] = configP->function;
				memcpy(&CMD_Array[CMD_POSITION_DATA + length + 3], &configP->value.pwm.period, 2);
				CMD_Array[CMD_POSITION_DATA + length + 5] = configP->value.pwm.ratio;
				length += 6;
			}
			break;
			default:
			{
			}
			break;
		}
		configP++;
	}

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (length & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (length & 0xFF00) >> 8;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_REMOTE_WRITECONFIG_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the remote GPIO configuration of the module
 *
 *output:
 * -configP: pointer to one or more pin configurations
 * -number_of_configsP: pointer to number of entries in configP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIORemoteReadConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t *number_of_configsP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= 0;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_REMOTE_READCONFIG_CNF, CMD_Status_Success, true);

		if(ret == true)
		{
			uint16_t length = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8);

			*number_of_configsP = 0;
			uint8_t* uartP = &RxPacket[CMD_POSITION_DATA+1];
			ProteusIII_SPI_GPIOConfigBlock_t* configP_running = configP;
			while(uartP < &RxPacket[CMD_POSITION_DATA+length])
			{
				switch(*(uartP + 2))
				{
					case ProteusIII_SPI_GPIO_IO_Disconnected:
					{
						if(*uartP == 3)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					case ProteusIII_SPI_GPIO_IO_Input:
					{
						if(*uartP == 3)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);
							configP_running->value.input = *(uartP + 3);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					case ProteusIII_SPI_GPIO_IO_Output:
					{
						if(*uartP == 3)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);
							configP_running->value.output = *(uartP + 3);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					case ProteusIII_SPI_GPIO_IO_PWM:
					{
						if(*uartP == 5)
						{
							configP_running->GPIO_ID = *(uartP + 1);
							configP_running->function = *(uartP + 2);
							memcpy(&configP_running->value.pwm.period, (uartP + 3), 2);
							configP_running->value.pwm.ratio = *(uartP + 5);

							configP_running++;
							*number_of_configsP += 1;
						}
					}
					break;
					default:
					{

					}
					break;
				}
				uartP += *uartP + 1;
			}
		}
    }

    return ret;
}

/*
 *Set the output value of the remote pin. Pin has to be configured first.
 *See ProteusIII_SPI_GPIORemoteWriteConfig
 *
 *input:
 * -controlP: pointer to one or more pin controls
 * -number_of_controls: number of entries in controlP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIORemoteWrite(ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t number_of_controls)
{
    bool ret = false;
	uint16_t length = 0;

	for (uint16_t i=0; i < number_of_controls; i++)
	{
		CMD_Array[CMD_POSITION_DATA + length] = 2;
		CMD_Array[CMD_POSITION_DATA + length + 1] = controlP->GPIO_ID;
		CMD_Array[CMD_POSITION_DATA + length + 2] = controlP->value.ratio;
		length += 3;
		controlP++;
	}

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= (length & 0x00FF);
    CMD_Array[CMD_POSITION_LENGTH_MSB]= (length & 0xFF00) >> 8;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_REMOTE_WRITE_CNF, CMD_Status_Success, true);
    }

    return ret;
}

/*
 *Read the input of the pins. Pin has to be configured first.
 *See ProteusIII_SPI_GPIORemoteWriteConfig
 *
 *input:
 * -GPIOToReadP: One or more pins to read.
 * -amountGPIOToRead: amount of pins to read and therefore length of GPIOToReadP
 *output:
 * -controlP: Pointer to controlBlock
 * -number_of_controlsP: pointer to number of entries in controlP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIII_SPI_GPIORemoteRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t* number_of_controlsP)
{
    bool ret = false;

    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_GPIO_REMOTE_READ_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB]= amountGPIOToRead + 1;
    CMD_Array[CMD_POSITION_LENGTH_MSB]= 0;
    CMD_Array[CMD_POSITION_DATA] = amountGPIOToRead;
    memcpy(&CMD_Array[CMD_POSITION_DATA + 1], GPIOToReadP, amountGPIOToRead);

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_GPIO_REMOTE_READ_CNF, CMD_Status_Success, true);

		if(ret)
		{
			uint16_t length = ((uint16_t) RxPacket[CMD_POSITION_LENGTH_LSB] << 0) + ((uint16_t) RxPacket[CMD_POSITION_LENGTH_MSB] << 8);

			*number_of_controlsP = 0;
			uint8_t* uartP = &RxPacket[CMD_POSITION_DATA+1];
			ProteusIII_SPI_GPIOControlBlock_t* controlP_running = controlP;
			while(uartP < &RxPacket[CMD_POSITION_DATA+length])
			{
				if(*uartP == 2)
				{
					controlP_running->GPIO_ID = *(uartP + 1);
					controlP_running->value.ratio   = *(uartP + 2);

					controlP_running++;
					*number_of_controlsP += 1;
				}
				uartP += *uartP + 1;
			}
		}
    }

    return ret;
}

/*
 *Temporarily allow unbonded connections, in case only bonded connections have been configured
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIII_SPI_Allowunbondedconnections()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    CMD_Array[CMD_POSITION_STX] = CMD_STX;
    CMD_Array[CMD_POSITION_CMD] = ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS_REQ;
    CMD_Array[CMD_POSITION_LENGTH_LSB] = (uint8_t)0;
    CMD_Array[CMD_POSITION_LENGTH_MSB] = (uint8_t)0;

    if (FillChecksum(CMD_Array, CMD_ARRAY_SIZE()))
    {
        /* now send CMD_ARRAY */
        SendCommand(CMD_Array, CMD_ARRAY_SIZE());

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ProteusIII_SPI_CMD_ALLOWUNBONDEDCONNECTIONS_CNF, CMD_Status_Success, true);
    }
    return ret;
}

