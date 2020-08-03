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

#include <errno.h>
#include "string.h"

#include "Triton.h"
#include "../../drivers/WE-common.h"
#include "../global/global.h"

#define CMD_WAIT_TIME 500
#define CNFINVALID 255
#define MAX_PAYLOAD_LENGTH 26
#define MAX_DATA_BUFFER 255

#define CMD_STX 0x02
#define Triton_CMD_TYPE_REQ (0 << 6)
#define Triton_CMD_TYPE_CNF (1 << 6)
#define Triton_CMD_TYPE_IND (2 << 6)
#define Triton_CMD_TYPE_RSP (3 << 6)

#define Triton_CMD_DATAEX 0x01
#define Triton_CMD_DATAEX_REQ (Triton_CMD_DATAEX | Triton_CMD_TYPE_REQ)
#define Triton_CMD_DATAEX_CNF (Triton_CMD_DATAEX | Triton_CMD_TYPE_CNF)
#define Triton_CMD_DATAEX_IND (Triton_CMD_DATAEX | Triton_CMD_TYPE_IND)

#define Triton_CMD_RESET 0x05
#define Triton_CMD_RESET_REQ (Triton_CMD_RESET | Triton_CMD_TYPE_REQ)
#define Triton_CMD_RESET_CNF (Triton_CMD_RESET | Triton_CMD_TYPE_CNF)
#define Triton_CMD_RESET_IND (Triton_CMD_RESET | Triton_CMD_TYPE_IND)

#define Triton_CMD_SET 0x09
#define Triton_CMD_SET_REQ (Triton_CMD_SET | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SET_CNF (Triton_CMD_SET | Triton_CMD_TYPE_CNF)

#define Triton_CMD_GET 0x0A
#define Triton_CMD_GET_REQ (Triton_CMD_GET | Triton_CMD_TYPE_REQ)
#define Triton_CMD_GET_CNF (Triton_CMD_GET | Triton_CMD_TYPE_CNF)

#define Triton_CMD_SERIALNO 0x0B
#define Triton_CMD_SERIALNO_REQ (Triton_CMD_SERIALNO | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SERIALNO_CNF (Triton_CMD_SERIALNO | Triton_CMD_TYPE_CNF)

#define Triton_CMD_FWVERSION 0x0C
#define Triton_CMD_FWVERSION_REQ (Triton_CMD_FWVERSION | Triton_CMD_TYPE_REQ)
#define Triton_CMD_FWVERSION_CNF (Triton_CMD_FWVERSION | Triton_CMD_TYPE_CNF)

#define Triton_CMD_RSSI 0x0D
#define Triton_CMD_RSSI_REQ (Triton_CMD_RSSI | Triton_CMD_TYPE_REQ)
#define Triton_CMD_RSSI_CNF (Triton_CMD_RSSI | Triton_CMD_TYPE_CNF)

#define Triton_CMD_FACTORY_RESET 0x11
#define Triton_CMD_FACTORY_RESET_REQ (Triton_CMD_FACTORY_RESET | Triton_CMD_TYPE_REQ)
#define Triton_CMD_FACTORY_RESET_CNF (Triton_CMD_FACTORY_RESET | Triton_CMD_TYPE_CNF)

#define Triton_CMD_BOOTLOADER 0x12
#define Triton_CMD_BOOTLOADER_REQ (Triton_CMD_BOOTLOADER | Triton_CMD_TYPE_REQ)
#define Triton_CMD_BOOTLOADER_CNF (Triton_CMD_BOOTLOADER | Triton_CMD_TYPE_CNF)

#define Triton_CMD_SET_TMP_CHANNEL 0x20
#define Triton_CMD_SET_TMP_CHANNEL_REQ (Triton_CMD_SET_TMP_CHANNEL | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SET_TMP_CHANNEL_CNF (Triton_CMD_SET_TMP_CHANNEL | Triton_CMD_TYPE_CNF)

#define Triton_CMD_SET_TMP_S_ADDR 0x21
#define Triton_CMD_SET_TMP_S_ADDR_REQ (Triton_CMD_SET_TMP_S_ADDR | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SET_TMP_S_ADDR_CNF (Triton_CMD_SET_TMP_S_ADDR | Triton_CMD_TYPE_CNF)

#define Triton_CMD_SET_TMP_SUBNET 0x22
#define Triton_CMD_SET_TMP_SUBNET_REQ (Triton_CMD_SET_TMP_SUBNET | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SET_TMP_SUBNET_CNF (Triton_CMD_SET_TMP_SUBNET | Triton_CMD_TYPE_CNF)

#define Triton_CMD_SET_TMP_PWRLVL 0x23
#define Triton_CMD_SET_TMP_PWRLVL_REQ (Triton_CMD_SET_TMP_PWRLVL | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SET_TMP_PWRLVL_CNF (Triton_CMD_SET_TMP_PWRLVL | Triton_CMD_TYPE_CNF)

#define Triton_CMD_SET_OPMODE 0x28
#define Triton_CMD_SET_OPMODE_REQ (Triton_CMD_SET_OPMODE | Triton_CMD_TYPE_REQ)
#define Triton_CMD_SET_OPMODE_CNF (Triton_CMD_SET_OPMODE | Triton_CMD_TYPE_CNF)

#define Triton_CMD_STATUS_IND 0xC0
#define Triton_CMD_DATAFRAGEX_IND 0x82

/* type used to check the response, when a command was sent to the Triton */
typedef enum CMD_Status_t
{
    CMD_Status_Success = 0x00,
    CMD_Status_Failed,
    CMD_Status_Invalid,
    CMD_Status_Reset,
} CMD_Status_t;

typedef struct
{
    uint8_t Stx;
    uint8_t Cmd;
    uint8_t Length;
    uint8_t Data[MAX_DATA_BUFFER + 1]; /* +1 for the CS */
} CMD_Frame_t;

typedef struct
{
    uint8_t cmd;                 /* variable to check if correct CMD has been confirmed */
    CMD_Status_t status;         /* variable used to check the response (*_CNF), when a request (*_REQ) was sent to the Triton */
} CMD_Confirmation_t;

/**************************************
 *     Static function declarations   *
 **************************************/

static void HandleRxPacket(uint8_t*RxBuffer);                                                      /* RX packet interpreter */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate);
static bool FillChecksum(uint8_t* array, uint8_t length);                                           /* add the CS needed to finalize the command */
static bool InitDriver(void(*RXcb)(uint8_t*,uint8_t,uint8_t,int8_t));

/**************************************
 *          Static variables          *
 **************************************/

int reset_pin;                             /* reset pin number */
int wakeup_pin;                            /* wakeup pin number */
CMD_Frame_t RxPacket;                      /* data buffer for RX */
bool AbortUartRxThread;                    /* boolean to abort the UART RX thread */
bool ResetUartRxThread;                    /* boolean to reset the UART RX thread */

pthread_t thread_read;

void(*RXcallback)(uint8_t*,uint8_t,uint8_t,int8_t); /* callback function */

#define CMDCONFIRMATIONARRAY_LENGTH 2
CMD_Confirmation_t cmdConfirmation_array[CMDCONFIRMATIONARRAY_LENGTH];

/**************************************
 *         Static functions           *
 **************************************/

/* thread function to receive bytes from interface */
void *rx_thread()
{
    uint8_t checksum = 0;
    uint8_t RxByteCounter = 0;
    uint8_t BytesToReceive = 0;
    uint8_t readBuffer;
    static uint8_t RxBuffer[sizeof(CMD_Frame_t)]; /* data buffer for RX */

    ResetUartRxThread = false;
    AbortUartRxThread = false;

    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_UARTRXTHREAD);

    while(1)
    {
        /* wait for 1ms, then check if new RX data is available */
        delay (1) ;

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
                    /* length field */
                    RxByteCounter++;
                    BytesToReceive = (RxBuffer[RxByteCounter - 1] + 4); /* len + crc + sfd + cmd */
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

/* interprete the valid received UART data packet */
static void HandleRxPacket(uint8_t*RxBuffer)
{
    CMD_Confirmation_t cmdConfirmation;
    cmdConfirmation.cmd = CNFINVALID;
    cmdConfirmation.status = CMD_Status_Invalid;

    uint cmd_length = RxBuffer[2];
    memcpy((uint8_t*)&RxPacket,RxBuffer,cmd_length + 4);

    switch (RxPacket.Cmd)
    {
    case Triton_CMD_FACTORY_RESET_CNF:
    {
        /* check whether the module returns success */
        if ((RxPacket.Data[0] == 0x00))
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_RESET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_STATUS_IND:
    {
		/* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SET_OPMODE_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x01 /*LPM*/)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_DATAEX_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            /* transmission success, ACK received if enabled */
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            /* transmission failed, no ACK received if enabled */
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SET_TMP_PWRLVL_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SET_TMP_CHANNEL_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SET_TMP_SUBNET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SET_TMP_S_ADDR_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_GET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Length > 1)
        {
			cmdConfirmation.status = CMD_Status_Success;
		}
		else
		{
			if (RxPacket.Data[0] == 0x00)
			{
				cmdConfirmation.status = CMD_Status_Success;
			}
			else
			{
				cmdConfirmation.status = CMD_Status_Failed;
			}
		}
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_FWVERSION_CNF:
    {
        cmdConfirmation.status = CMD_Status_Success;
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_SERIALNO_CNF:
    {
        cmdConfirmation.status = CMD_Status_Success;
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_RSSI_CNF:
    {
        cmdConfirmation.status = CMD_Status_Success;
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case Triton_CMD_DATAEX_IND:
    {
        /* data received, give it to the RXcallback function */
        if (RXcallback != NULL)
        {
            int8_t rssi = -120 + 8*RxPacket.Data[RxPacket.Length-1];
            RXcallback(&RxPacket.Data[1], RxPacket.Length - 2, RxPacket.Data[0], rssi);
        }
    }
    break;

    default:
        break;
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

/* function that waits for the return value of Triton (*_CNF), when a command (*_REQ) was sent before */
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
static bool FillChecksum(uint8_t* array, uint8_t length)
{
    bool ret = false;

    if ((length >= 4) && (array[0] == CMD_STX))
    {
        uint8_t checksum = 0;
        uint8_t payload_length = array[2];
        int i = 0;
        for (i = 0;
                i < payload_length + 3;
                i++)
        {
            checksum ^= array[i];
        }
        array[payload_length + 3] = checksum;
        ret = true;
    }
    return ret;
}

/*
 *Initialize the Triton and driver
 *
 *input:
 * -RXcb:      RX callback function
 *
 *Caution:  *the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
static bool InitDriver(void(*RXcb)(uint8_t*,uint8_t,uint8_t,int8_t))
{

    /* set RX callback function */
    RXcallback = RXcb;

    /* start RX thread */
    if(pthread_create(&thread_read, NULL, &rx_thread, NULL))
    {
        fprintf(stdout, "Failed to start rx_Thread\n");
        Triton_Deinit();
        return false;
    }

    /* reset module*/
    if(Triton_PinReset())
    {
        delay(300);
    }
    else
    {
        fprintf(stdout, "Pin Reset failed\n");
        Triton_Deinit();
        return false;
    }

    /* Read out the firmware version of the connected AMBER module */
    uint8_t response[3];
    if(Triton_GetFirmwareVersion(response))
    {
        fprintf (stdout, "Firmware version %d.%d.%d detected\n",response[0],response[1],response[2]);
    }
    else
    {
        fprintf (stdout, "Readout of the firmware version failed.\nProbably invalid UART configuration or module is shut down.\n");
        Triton_Deinit();
        return false;
    }
    return true;
}


/**************************************
 *         Global functions           *
 **************************************/

/*
 *Initialize the Triton for serial interface
 *
 *input:
 * -baudrate: baudrate of the serial interface
 * -rp:       reset pin number
 * -wp:       wakeup pin number
 * -RXcb:     RX callback function
 *
 *Caution: the parameters baudrate and addrmode must match the configured UserSettings of the Triton
 *         -the baudrate parameter must match to perform a successful UART communication
 *          *updating this parameter during runtime may lead to communication errors
 *         -the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in Triton_Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool Triton_Init(int baudrate, int rp, int wp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,int8_t))
{

    if (false == InitPin(rp))
    {
        /* error */
        return false ;
    }

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

    /* initialize the wakeup pin */
    wakeup_pin = wp;
    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    delay(10);

    /* initialize the reset pin,
     * we have to define it as input/pull-up such that the reset button
     * on the base PCB can pull the pin level down if the button is pressed */
    reset_pin = rp;
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);
    delay(10);

    /* empty the UART buffers */
    FlushSerial();

    return InitDriver(RXcb);
}

/*
 *Deinitialize the Triton interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool Triton_Deinit()
{
    /* close the communication interface to the module */
    CloseSerial();
    DeinitSerial();

    /* abort RX thread */
    AbortUartRxThread = true;

    /* deinit pins */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    SetPin(wakeup_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    DeinitPin(reset_pin);

    /* deinit RX callback and address mode */
    RXcallback = NULL;

    return true;
}

/*
 *Wakeup the Triton from sleep by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool Triton_PinWakeup()
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
    return Wait4CNF(CMD_WAIT_TIME, Triton_CMD_STATUS_IND, CMD_Status_Success, false);
}

/*
 *Reset the Triton by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool Triton_PinReset()
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
    return Wait4CNF(CMD_WAIT_TIME, Triton_CMD_STATUS_IND, CMD_Status_Success, true);
}

/*
 *Reset the Triton by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool Triton_Reset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_RESET_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_RESET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Factory reset the Triton
 *
 *note: use them only in rare cases, since flash can be updated only a limited number times
 *
 *return true if factory reset succeeded
 *       false otherwise
 */
bool Triton_FactoryReset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_FACTORY_RESET_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(1500, Triton_CMD_FACTORY_RESET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Switch the module to sleep mode
 *
 *return true if switching succeeded
 *       false otherwise
 */
bool Triton_Sleep()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SET_OPMODE_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = 0x01; /* LPM */
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SET_OPMODE_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Request the current Triton settings
 *
 *input:
 * -us: user setting to be requested
 *
 *output:
 * -response: pointer of the memory to put the request content
 * -response_length: length of the request content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_Get(Triton_UserSettings_t us, uint8_t* response, uint8_t* response_length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[10];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_GET_REQ;
    CMD_ARRAY[2] = 0x02;
    CMD_ARRAY[3] = us;

    switch(us)
    {
        /* usersettings with value length 1 */
        case(Triton_CMD_SETGET_OPTION_RFCHANNEL):
        case(Triton_CMD_SETGET_OPTION_RFRATE):
        case(Triton_CMD_SETGET_OPTION_SUBNETID):
        case(Triton_CMD_SETGET_OPTION_ADDRESSSOURCE):
        case(Triton_CMD_SETGET_OPTION_RFPOWERLEVEL):
        case(Triton_CMD_SETGET_OPTION_CFG_FLAGS):
        {
            CMD_ARRAY[4] = 0x01;
        }
        break;

        /* usersettings with value length 4 */
        case(Triton_CMD_SETGET_OPTION_UARTBAUDRATE):
        {
            CMD_ARRAY[4] = 0x04;
        }
        break;
        default:
            break;
    }

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, Triton_CMD_GET_CNF, CMD_Status_Success, true))
        {
            int length = RxPacket.Length - 1;
            memcpy(response,&RxPacket.Data[1],length);
            *response_length = length;
            ret = true;
        }
    }
    return ret;
}

/*
 *Set a special Triton setting
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
bool Triton_Set(Triton_UserSettings_t us, uint8_t* value, uint8_t length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + 5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SET_REQ;
    CMD_ARRAY[2] = (1 + length);
    CMD_ARRAY[3] = us;
    memcpy(&CMD_ARRAY[4],value,length);
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Request the 3 byte firmware version
 *
 *output:
 * -fw: pointer to the 3 byte firmware version
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetFirmwareVersion(uint8_t* fw)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_FWVERSION_REQ;
    CMD_ARRAY[2] = 0x00;

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, Triton_CMD_FWVERSION_CNF, CMD_Status_Success, true))
        {
            memcpy(fw,&RxPacket.Data[0],3);
            ret = true;
        }
    }
    return ret;
}

/*
 *Request the 4 byte serial number
 *
 *output:
 * -sn: pointer to the 4 byte serial number
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetSerialNumber(uint8_t* sn)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SERIALNO_REQ;
    CMD_ARRAY[2] = 0x00;

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SERIALNO_CNF, CMD_Status_Success, true))
        {
            memcpy(sn,&RxPacket.Data[0],4);
            ret = true;
        }
    }
    return ret;
}

/*
 *Request the default TX power
 *
 *output:
 * -txpower: pointer to the TXpower
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetDefaultTXPower(uint8_t* txpower)
{
    uint8_t length;
    return Triton_Get(Triton_CMD_SETGET_OPTION_RFPOWERLEVEL, txpower, &length);
}


/*
 *Get the default source address
 *
 *output:
 * -srcaddr: pointer to the source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetSourceAddr(uint8_t* srcaddr)
{
    uint8_t length;
    return Triton_Get(Triton_CMD_SETGET_OPTION_ADDRESSSOURCE, srcaddr, &length);
}

/*
 *Get the default source net id
 *
 *output:
 * -subnet: pointer to the source net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetSubnet(uint8_t* subnet)
{
    uint8_t length;
    return Triton_Get(Triton_CMD_SETGET_OPTION_SUBNETID, subnet, &length);
}


/*
 *Get the default RF channel
 *
 *output:
 * -channel: channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetDefaultRFChannel(uint8_t* channel)
{
    uint8_t length;
    return Triton_Get(Triton_CMD_SETGET_OPTION_RFCHANNEL, channel, &length);
}

/*
 *Get the default RF profile
 *
 *output:
 * -profile: RF profile
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetDefaultRFProfile(uint8_t* profile)
{
    uint8_t length;
    return Triton_Get(Triton_CMD_SETGET_OPTION_RFRATE, profile, &length);
}

/*
 *Get the RSSI of the last received RF packer
 *
 *output:
 * -rssi: pointer to the rssi
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_GetRSSI(int8_t* rssi)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_RSSI_REQ;
    CMD_ARRAY[2] = 0x00;

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, Triton_CMD_RSSI_CNF, CMD_Status_Success, true))
        {
            *rssi = -120 + 8*RxPacket.Data[0];
            ret = true;
        }
    }
    return ret;
}

/*
 *Set the default TX power
 *
 *input:
 * -txpower: TXpower
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use Triton_SetVolatile_TXPower for frequent adaption of the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetDefaultTXPower(int8_t txpower)
{
    if((txpower<-5)||(txpower>10))
    {
        return false;
    }
    return Triton_Set(Triton_CMD_SETGET_OPTION_RFPOWERLEVEL, (uint8_t*)&txpower, 1);
}

/*
 *Set the default source address
 *
 *input:
 * -srcaddr: the source address
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetSourceAddr(uint8_t srcaddr)
{
    if(srcaddr == Triton_BROADCASTADDRESS)
    {
        return false;
    }
    return Triton_Set(Triton_CMD_SETGET_OPTION_ADDRESSSOURCE, &srcaddr, 1);
}

/*
 *Set the default source net id
 *
 *input:
 * -subnet: the source net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 *
 */
bool Triton_SetSubnet(uint8_t subnet)
{
    if((subnet == 0)||(subnet == 48)||(subnet == 51)||(subnet == 99)||(subnet == 102)||(subnet == 146)||(subnet == 153)||(subnet == 204))
    {
        /* invalid net id */
        return false;
    }
    return Triton_Set(Triton_CMD_SETGET_OPTION_SUBNETID, &subnet, 1);
}

/*
 *Set the default RF channel
 *
 *input:
 * -channel: channel
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use Triton_SetVolatile_Channel for frequent adaption of the channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetDefaultRFChannel(uint8_t channel)
{

    /* check for valid channel */
    if((channel<0)||(channel>19))
    {
        /* invalid channel */
        return false;
    }
    return Triton_Set(Triton_CMD_SETGET_OPTION_RFCHANNEL, &channel, 1);
}

/*
 *Set the default RF profile
 *
 *input:
 * -profile: RF profile
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetDefaultRFProfile(uint8_t profile)
{
    if((profile < 1)||(profile > 7))
    {
        return false;
    }
    return Triton_Set(Triton_CMD_SETGET_OPTION_RFRATE, &profile, 1);
}

/*
 *Set the default baud rate
 *
 *input:
 * -baudrate: baud rate
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetDefaultBaudrate(uint32_t baudrate)
{
    if((baudrate < 300)||(baudrate > 115200))
    {
        return false;
    }
    uint8_t help[4];
    memcpy(help,(uint8_t*)&baudrate,4);
    return Triton_Set(Triton_CMD_SETGET_OPTION_UARTBAUDRATE, help, 4);
}

/*
 *Set the volatile TX power
 *
 *input:
 * -power: new TX power value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetVolatile_TXPower(int8_t txpower)
{
    bool ret = false;

    if((txpower<-5)||(txpower>10))
    {
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SET_TMP_PWRLVL_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = txpower;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SET_TMP_PWRLVL_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Set the volatile channel
 *
 *input:
 * -channel: new channel value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetVolatile_Channel(uint8_t channel)
{
    bool ret = false;

    /* check for valid channel */
    if((channel<0)||(channel>19))
    {
        /* invalid channel */
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SET_TMP_CHANNEL_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = channel;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SET_TMP_CHANNEL_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Set the volatile source address
 *
 *input:
 * -sourceaddr: new source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetVolatile_SourceAddress(uint8_t sourceaddr)
{
    bool ret = false;

    if(sourceaddr == Triton_BROADCASTADDRESS)
    {
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SET_TMP_S_ADDR_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = sourceaddr;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SET_TMP_S_ADDR_CNF, CMD_Status_Success, true);
    }
    return ret;
}


/*
 *Set the volatile source address
 *
 *input:
 * -sourceaddr: new source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_SetVolatile_Subnet(uint8_t subnet)
{
    bool ret = false;

    if((subnet == 0)||(subnet == 48)||(subnet == 51)||(subnet == 99)||(subnet == 102)||(subnet == 146)||(subnet == 153)||(subnet == 204))
    {
        /* invalid net id */
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_SET_TMP_SUBNET_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = subnet;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_SET_TMP_SUBNET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Transmit data
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 * -dest_address: destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool Triton_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t dest_address)
{
    bool ret = false;

    if(length > MAX_PAYLOAD_LENGTH)
    {
        fprintf(stdout, "Data exceeds maximal payload length\n");
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + 4 + 1];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = Triton_CMD_DATAEX_REQ;
    CMD_ARRAY[2] = (length + 1);
    CMD_ARRAY[3] = dest_address;
    memcpy(&CMD_ARRAY[4],payload,length);

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, Triton_CMD_DATAEX_CNF, CMD_Status_Success, true);
    }
    return ret;
}


/*
 *Configure the Triton
 *
 *input:
 * -config: pointer to the configuration struct
 * -config_length: length of the configuration struct
 * -factory_reset: apply a factory reset before or not
 *
 *return true if request succeeded
 *       false otherwise
*/
bool Triton_Configure(Triton_Configuration_t* config, uint8_t config_length, bool factory_reset)
{
    int i = 0;
    uint8_t help_length;
    uint8_t help[MAX_USERSETTING_LENGTH];

    if(factory_reset)
    {
        /* perform a factory reset */
        if(false == Triton_FactoryReset())
        {
            /* error */
            return false;
        }
    }
    delay(500);

    /* now check all settings and update them if necessary */
    for(i=0; i<config_length; i++)
    {
        /* read current value */
        if(false == Triton_Get(config[i].usersetting, help, &help_length))
        {
            /* error */
            return false;
        }
        delay(200);

        /* check the value read out */
        if(help_length != config[i].value_length)
        {
            /* error, length does not match */
            return false;
        }
        if(memcmp(help,config[i].value,config[i].value_length) != 0)
        {
            /* read value is not up to date, thus write the new value */
            if(false == Triton_Set(config[i].usersetting, config[i].value, config[i].value_length))
            {
                /* error */
                return false;
            }
        }
        delay(200);
    }

    /* reset to take effect of the updated parameters */
    if(false == Triton_PinReset())
    {
        return false;
    }
    return true;
}
