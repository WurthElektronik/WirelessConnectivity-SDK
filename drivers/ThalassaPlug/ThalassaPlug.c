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
#include <pthread.h>

#include <errno.h>
#include "string.h"

#include "ThalassaPlug.h"
#include "../../drivers/WE-common.h"
#include "../global/global.h"

#define CMD_WAIT_TIME 500
#define MAX_PAYLOAD_LENGTH 128
#define TXPOWERINVALID -128
#define CHANNELINVALID -1
#define RSSIINVALID -128
#define CNFINVALID 255

#define CMD_STX 0x02
#define ThalassaPlug_CMD_TYPE_REQ (0 << 6)
#define ThalassaPlug_CMD_TYPE_CNF (1 << 6)
#define ThalassaPlug_CMD_TYPE_IND (2 << 6)
#define ThalassaPlug_CMD_TYPE_RSP (3 << 6)

#define ThalassaPlug_CMD_DATA 0x00
#define ThalassaPlug_CMD_DATA_REQ (ThalassaPlug_CMD_DATA | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_DATA_CNF (ThalassaPlug_CMD_DATA | ThalassaPlug_CMD_TYPE_CNF)
#define ThalassaPlug_CMD_REPEAT_IND (ThalassaPlug_CMD_DATA | ThalassaPlug_CMD_TYPE_IND)

#define ThalassaPlug_CMD_DATAEX 0x01
#define ThalassaPlug_CMD_DATAEX_REQ (ThalassaPlug_CMD_DATAEX | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_DATAEX_IND (ThalassaPlug_CMD_DATAEX | ThalassaPlug_CMD_TYPE_IND)

#define ThalassaPlug_CMD_SETMODE 0x04
#define ThalassaPlug_CMD_SETMODE_CNF (ThalassaPlug_CMD_SETMODE | ThalassaPlug_CMD_TYPE_CNF)
#define ThalassaPlug_CMD_SETMODE_IND (ThalassaPlug_CMD_SETMODE | ThalassaPlug_CMD_TYPE_IND)

#define ThalassaPlug_CMD_RESET 0x05
#define ThalassaPlug_CMD_RESET_REQ (ThalassaPlug_CMD_RESET | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_RESET_CNF (ThalassaPlug_CMD_RESET | ThalassaPlug_CMD_TYPE_CNF)
#define ThalassaPlug_CMD_RESET_IND (ThalassaPlug_CMD_RESET | ThalassaPlug_CMD_TYPE_IND)

#define ThalassaPlug_CMD_SET_CHANNEL 0x06
#define ThalassaPlug_CMD_SET_CHANNEL_REQ (ThalassaPlug_CMD_SET_CHANNEL | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_SET_CHANNEL_CNF (ThalassaPlug_CMD_SET_CHANNEL | ThalassaPlug_CMD_TYPE_CNF)

#define ThalassaPlug_CMD_SET_DESTNETID 0x07
#define ThalassaPlug_CMD_SET_DESTNETID_REQ (ThalassaPlug_CMD_SET_DESTNETID | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_SET_DESTNETID_CNF (ThalassaPlug_CMD_SET_DESTNETID | ThalassaPlug_CMD_TYPE_CNF)

#define ThalassaPlug_CMD_SET_DESTADDR 0x08
#define ThalassaPlug_CMD_SET_DESTADDR_REQ (ThalassaPlug_CMD_SET_DESTADDR | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_SET_DESTADDR_CNF (ThalassaPlug_CMD_SET_DESTADDR | ThalassaPlug_CMD_TYPE_CNF)

#define ThalassaPlug_CMD_SET 0x09
#define ThalassaPlug_CMD_SET_REQ (ThalassaPlug_CMD_SET | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_SET_CNF (ThalassaPlug_CMD_SET | ThalassaPlug_CMD_TYPE_CNF)

#define ThalassaPlug_CMD_GET 0x0A
#define ThalassaPlug_CMD_GET_REQ (ThalassaPlug_CMD_GET | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_GET_CNF (ThalassaPlug_CMD_GET | ThalassaPlug_CMD_TYPE_CNF)

#define ThalassaPlug_CMD_GET_SERIALNO 0x0B
#define ThalassaPlug_CMD_GET_SERIALNO_REQ (ThalassaPlug_CMD_GET_SERIALNO | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_GET_SERIALNO_CNF (ThalassaPlug_CMD_GET_SERIALNO| ThalassaPlug_CMD_TYPE_CNF)

#define ThalassaPlug_CMD_RSSI 0x0D
#define ThalassaPlug_CMD_RSSI_REQ (ThalassaPlug_CMD_RSSI | ThalassaPlug_CMD_TYPE_REQ)
#define ThalassaPlug_CMD_RSSI_CNF (ThalassaPlug_CMD_RSSI | ThalassaPlug_CMD_TYPE_CNF)

/* type used to check the response, when a command was sent to the ThalassaPlug */
typedef enum CMD_Status_t
{
    CMD_Status_Success = 0x00,
    CMD_Status_Failed,
    CMD_Status_Invalid,
    CMD_Status_Reset,
} CMD_Status_t;

/*type used to store received frames */
typedef struct
{
    uint8_t Stx;
    uint8_t Cmd;
    uint8_t Length;
    uint8_t Data[MAX_PAYLOAD_LENGTH + 1]; /* +1 for the CS */
} CMD_Frame_t;

typedef struct
{
    uint8_t cmd;                /* variable to check if correct CMD has been confirmed */
    CMD_Status_t status;        /* variable used to check the response (*_CNF), when a request (*_REQ) was sent to the TarvosIII */
} CMD_Confirmation_t;

typedef enum ThalassaPlug_OperationMode_t
{
    ThalassaPlug_OperationMode_Transparent = 0x00, /* invalid for operation with this driver */
    ThalassaPlug_OperationMode_Command = 0x10,
}
ThalassaPlug_OperationMode_t;

typedef struct
{
    uint8_t memoryPosition;                 /* memory position of requested usersetting */
    uint8_t lengthGetRequest;               /* length of one or more requested usersetting */
} US_Confirmation_t;

/**************************************
 *     Static function declarations   *
 **************************************/

static void HandleRxPacket(uint8_t* RxBuffer);                                                    /* RX packet interpreter */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate);   /* wait for response, when a command was sent to the ThalassaPlug */
static bool FillChecksum(uint8_t* array, uint8_t length);                                         /* add the CS needed to finalize the command */
static bool InitDriver(void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThalassaPlug_AddressMode_t addrmode);
                                                      /* read next Byte from interface */
static int8_t CalculateRSSIValue(uint8_t rxLevel);

/**************************************
 *          Static variables          *
 **************************************/

CMD_Frame_t RxPacket;                       /* data buffer for RX */
bool AbortUartRxThread;                     /* boolean to abort the UART RX thread */
bool ResetUartRxThread;                     /* boolean to reset the UART RX thread */

pthread_t thread_read;

int reset_pin;

#define CMDCONFIRMATIONARRAY_LENGTH 2

CMD_Confirmation_t cmdConfirmation_array[CMDCONFIRMATIONARRAY_LENGTH];
US_Confirmation_t usConfirmation;             /* variable used to check if GET function was successfull */
uint8_t channelVolatile = CHANNELINVALID;     /* variable used to check if setting the channel was successfull */
uint8_t powerVolatile = TXPOWERINVALID;       /* variable used to check if setting the TXPower was successfull */


void(*RXcallback)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t);       /* callback function */
ThalassaPlug_AddressMode_t addressmode = AddressMode_0;                        /* initial address mode */


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
        /* wait for 10ms, then check if new RX data is available */
        delay (10);

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

/*
 *Initialize the ThalassaPlug and driver
 *
 *input:
 * -RXcb:      RX callback function
 * -addrmode:  address mode of the ThalassaPlug
 *
 *Caution:  *the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
static bool InitDriver(void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThalassaPlug_AddressMode_t addrmode)
{
    /* set address mode */
    addressmode = addrmode;

    /* set RX callback function */
    RXcallback = RXcb;

    /* start RX thread */
    if(pthread_create(&thread_read, NULL, &rx_thread, NULL))
    {
        fprintf(stdout, "Failed to start rx_Thread\n");
        ThalassaPlug_Deinit();
        return false;
    }

    /* reset module*/
    if(ThalassaPlug_PinReset())
    {
        delay(50);
    }
    else
    {
        fprintf(stdout, "Pin Reset failed\n");
        ThalassaPlug_Deinit();
        return false;
    }
    return true;
}



/* interprete the valid received data packet */
static void HandleRxPacket(uint8_t*RxBuffer)
{
    CMD_Confirmation_t cmdConfirmation;
    cmdConfirmation.cmd = CNFINVALID;
    cmdConfirmation.status = CMD_Status_Invalid;

    uint cmd_length = RxBuffer[2];
    memcpy((uint8_t*)&RxPacket,RxBuffer,cmd_length + 4);

    switch (RxPacket.Cmd)
    {
    case ThalassaPlug_CMD_SETMODE_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == ThalassaPlug_OperationMode_Command)
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

    case ThalassaPlug_CMD_RESET_CNF:
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

    case ThalassaPlug_CMD_RESET_IND:
    {
        cmdConfirmation.status = CMD_Status_Success;
        cmdConfirmation.cmd = ThalassaPlug_CMD_RESET_IND;
    }
    break;

    case ThalassaPlug_CMD_DATA_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            /* transmission success, ACK received if enabled */
            cmdConfirmation.status = CMD_Status_Success;
        }
        else if (RxPacket.Data[0] == 0x01)
        {
            /* no ACK requested or not received if enabled */
            cmdConfirmation.status = CMD_Status_Failed;
        }
        else
        {
            /* Invaild channel was selected */
            cmdConfirmation.status = CMD_Status_Invalid;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case ThalassaPlug_CMD_GET_CNF:
    {
        /*Data[0] contains memory postion of usersetting
         *Data[1] contains length of parameter, which is depending on usersetting
         *On success mode responds with usersetting, length of parameter and paramter
         */

        /* check if correct memoryPosition and length was returned */
        if((usConfirmation.memoryPosition == RxPacket.Data[0]) && (usConfirmation.lengthGetRequest == RxPacket.Data[1]))
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

    case ThalassaPlug_CMD_SET_CNF:
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

    case ThalassaPlug_CMD_GET_SERIALNO_CNF:
    {
        /* check whether the module returns 4 byte serial number*/
        if (RxPacket.Length == 4)
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

    case ThalassaPlug_CMD_DATAEX_IND:
    {
        /* data received, give it to the RXcallback function */
        if (RXcallback != NULL)
        {
            switch (addressmode)
            {
            case AddressMode_0:
            {
                RXcallback(&RxPacket.Data[0], RxPacket.Length - 1, ThalassaPlug_BROADCASTADDRESS, ThalassaPlug_BROADCASTADDRESS, ThalassaPlug_BROADCASTADDRESS, CalculateRSSIValue(RxPacket.Data[RxPacket.Length-1]));
            }
            break;

            case AddressMode_1:
            {
                RXcallback(&RxPacket.Data[1], RxPacket.Length - 2, ThalassaPlug_BROADCASTADDRESS, RxPacket.Data[0], ThalassaPlug_BROADCASTADDRESS, CalculateRSSIValue(RxPacket.Data[RxPacket.Length-1]));
            }
            break;

            case AddressMode_2:
            {
                RXcallback(&RxPacket.Data[2], RxPacket.Length - 3, RxPacket.Data[0], RxPacket.Data[1], ThalassaPlug_BROADCASTADDRESS, CalculateRSSIValue(RxPacket.Data[RxPacket.Length-1]));
            }
            break;

            default:
                /* wrong address mode */
                break;
            }
        }
    }
    break;

    case ThalassaPlug_CMD_SET_CHANNEL_CNF:
    {
        /* check whether the module set value of channel as requested */
        if(RxPacket.Data[0] == channelVolatile)
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

    case ThalassaPlug_CMD_SET_DESTADDR_CNF:
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

    case ThalassaPlug_CMD_SET_DESTNETID_CNF:
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

/* function that waits for the return value of ThalassaPlug (*_CNF), when a command (*_REQ) was sent before */
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

/* function to calculate the rssi value from the rx level */
static int8_t CalculateRSSIValue(uint8_t rxLevel)
{
    /* 72 is offset for rssi level with data rate of 250kbps*/
    uint8_t offset = 72;
    int8_t rssi_value = RSSIINVALID;

    if(rxLevel < 128)
    {
        rssi_value = rxLevel/2 - offset;
    }
    else
    {
        rssi_value = (rxLevel - 256)/2 - offset;
    }
    return rssi_value;
}



/**************************************
 *         Global functions           *
 **************************************/
/*
 *Initialize the ThalassaPlug interface
 *
 *input:
 * -baudrate:  baudrate of the interface
 * -rp:        reset pin number
 * -RXcb:      RX callback function
 * -addrmode:  address mode of the ThalassaPlug
 *
 *Caution: the parameters baudrate and addrmode must match the configured UserSettings of the ThalassaPlug
 *         -the baudrate parameter must match to perform a successful FTDI communication
 *          *updating this parameter during runtime may lead to communication errors
 *         -the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in ThalassaPlug_Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool ThalassaPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThalassaPlug_AddressMode_t addrmode)
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

    /* initialize the reset pin,
     * we have to define it as input/pull-up such that the reset button
     * can pull the pin level down if the button is pressed
     */
    reset_pin = rp;
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);
    delay(10);

	/* empty the UART buffers */
    FlushSerial();

    /* init the driver and module*/
    return InitDriver(RXcb, addrmode);
}


/*
 *Deinitialize the ThalassaPlug interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ThalassaPlug_Deinit()
{
    /* close the communication interface to the module */
    CloseSerial();

    /* abort RX thread */
    AbortUartRxThread = true;

    /* deinit pins */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    DeinitPin(reset_pin);

    /* reset RX callback and address mode */
    RXcallback = NULL;
    addressmode = AddressMode_0;

    return true;
}

/*
 *Reset the ThalassaPlug by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ThalassaPlug_PinReset()
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
    return Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_SETMODE_CNF, CMD_Status_Success, true);
}

/*
 *Reset the ThalassaPlug by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ThalassaPlug_Reset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_RESET_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_RESET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Request the current ThalassaPlug settings
 *
 *input:
 * -us: user setting to be requested
 *
 *output:
 * -response: pointer of the memory to put the requested content
 * -response_length: length of the requested content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_Get(ThalassaPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[6];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_GET_REQ;
    CMD_ARRAY[2] = 0x02;
    CMD_ARRAY[3] = us;

    switch(us)
    {
    /* usersettings with value length 1 */
    case(ThalassaPlug_USERSETTING_MEMPOSITION_NUMRETRYS):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_DESTNETID):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_DESTADDRLSB):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_SOURCENETID):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_SOURCEADDRLSB):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_PAPOWER):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_DEFAULTCHANNEL):
    case(ThalassaPlug_USERSETTING_MEMPOSITION_OP_MODE):
    {
        CMD_ARRAY[4] = 0x01;
    }
    break;

    /* usersettings with value length 2 */
    case(ThalassaPlug_USERSETTING_MEMPOSITION_CFG_FLAGS):
    {
        CMD_ARRAY[4] = 0x02;
    }
    break;
    }
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        usConfirmation.memoryPosition = us;
        usConfirmation.lengthGetRequest = CMD_ARRAY[4];

        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_GET_CNF, CMD_Status_Success, true))
        {
            int length = RxPacket.Length - 2;
            memcpy(response,&RxPacket.Data[2],length);
            *response_length = length;
            ret = true;
        }
        usConfirmation.memoryPosition = -1;
        usConfirmation.lengthGetRequest = -1;
    }
    return ret;
}

/*
 *Request multiple of the current Metis settings
 *
 *input:
 * -startAddress: first usersetting to be read
 * -lengthToRead: Length of memory to be read
 *
 *output:
 * -response: pointer of the memory to put the request content
 * -response_length: length of the request content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_GetMultiple(uint8_t startAddress, uint8_t lengthToRead, uint8_t *response, uint8_t *response_length)
{
    bool ret = false;

    uint8_t CMD_ARRAY[6];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_GET_REQ;
    CMD_ARRAY[2] = 0x02;
    CMD_ARRAY[3] = startAddress;
    CMD_ARRAY[4] = lengthToRead;

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        usConfirmation.memoryPosition = startAddress;
        usConfirmation.lengthGetRequest = CMD_ARRAY[4];

        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_GET_CNF, CMD_Status_Success, true))
        {
            int length = RxPacket.Length - 2;
            memcpy(response,&RxPacket.Data[2],length);
            *response_length = length;
            ret = true;
        }
        usConfirmation.memoryPosition = -1;
        usConfirmation.lengthGetRequest = -1;
    }
    return ret;
}

/*
 *Set a special ThalassaPlug setting
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
bool ThalassaPlug_Set(ThalassaPlug_UserSettings_t us, uint8_t* value, uint8_t length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + 6];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_SET_REQ;
    CMD_ARRAY[2] = (2 + length);
    CMD_ARRAY[3] = us;
    CMD_ARRAY[4] = length;
    memcpy(&CMD_ARRAY[5],value,length);
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_SET_CNF, CMD_Status_Success, true);
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
bool ThalassaPlug_GetSerialNumber(uint8_t* sn)
{
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_GET_SERIALNO_REQ;
    CMD_ARRAY[2] = 0;

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_GET_SERIALNO_CNF, CMD_Status_Success, true))
        {
            memcpy(sn,&RxPacket.Data[0],RxPacket.Length);
            return true;
        }
    }
    return false;
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
bool ThalassaPlug_GetDefaultTXPower(int8_t* txpower)
{
    uint8_t length;

    if(ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_PAPOWER, (uint8_t*)txpower, &length))
    {
        return true;
    }
    else
    {
        *txpower = TXPOWERINVALID;
        return false;
    }
}

/*
 *Get the default destination address
 *
 *output:
 * -destaddr_lsb: LSB of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb)
{
    uint8_t length;

    if(ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_DESTADDRLSB, destaddr_lsb, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Get the default destination address
 *
 *output:
 * -destnetid: destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_GetDefaultDestNetID(uint8_t* destnetid)
{
    uint8_t length;

    if(ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_DESTNETID, destnetid, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Get the default source address
 *
 *output:
 * -srcaddr_lsb: LSB of the source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_GetSourceAddr(uint8_t* srcaddr_lsb)
{
    uint8_t length;

    if(ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_SOURCEADDRLSB, srcaddr_lsb, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Get the default source address
 *
 *output:
 * -srcnetid: source net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_GetSourceNetID(uint8_t* srcnetid)
{
    uint8_t length;

    if(ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_SOURCENETID, srcnetid, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
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
bool ThalassaPlug_GetDefaultRFChannel(uint8_t* channel)
{
    uint8_t length;

    if(ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_DEFAULTCHANNEL, channel, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Set the default TX power
 *
 *input:
 * -txpower: TXpower
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThalassaPlug_SetVolatile_TXPower for frequent adaption of the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetDefaultTXPower(int8_t txpower)
{
    /* check for invalid power */
    if((txpower < -11) || (txpower > 15))
    {
        /* invalid power */
        return false;
    }
    return ThalassaPlug_Set(ThalassaPlug_USERSETTING_MEMPOSITION_PAPOWER, (uint8_t*)&txpower, 1);
}

/*
 *Set the default destination address
 *
 *input:
 * -destaddr_lsb: LSB of the destination address
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThalassaPlug_SetVolatile_DestAddr for frequent adaption of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetDefaultDestAddr(uint8_t destaddr_lsb)
{
    /* check for invalid address */
    if(destaddr_lsb > 255)
    {
        /* address invalid */
        return false;
    }
    return ThalassaPlug_Set(ThalassaPlug_USERSETTING_MEMPOSITION_DESTADDRLSB, &destaddr_lsb, 1);
}

/*
 *Set the default destination address
 *
 *input:
 * -destnetid: destination net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThalassaPlug_SetVolatile_DestNetID for frequent adaption of the destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetDefaultDestNetID(uint8_t destnetid)
{
    /* check for invalid destnetid */
    if(destnetid > 255)
    {
        /* invalid destnetid */
        return false;
    }
    return ThalassaPlug_Set(ThalassaPlug_USERSETTING_MEMPOSITION_DESTNETID, &destnetid, 1);
}

/*
 *Set the default source address
 *
 *input:
 * -srcaddr_lsb: LSB of the source address
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetSourceAddr(uint8_t srcaddr_lsb)
{
    /*check for invalid address */
    if(srcaddr_lsb > 255)
    {
        /* address invalid */
        return false;
    }
    return ThalassaPlug_Set(ThalassaPlug_USERSETTING_MEMPOSITION_SOURCEADDRLSB, &srcaddr_lsb, 1);
}

/*
 *Set the default source net id
 *
 *input:
 * -srcnetid: source net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetSourceNetID(uint8_t srcnetid)
{
    /* check for invalid srcnetid */
    if(srcnetid > 254)
    {
        /* invalid srcnetid */
        return false;
    }
    return ThalassaPlug_Set(ThalassaPlug_USERSETTING_MEMPOSITION_SOURCENETID, &srcnetid, 1);
}

/*
 *Set the default RF channel
 *
 *input:
 * -channel: channel
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThalassaPlug_SetVolatile_RFChannel for frequent adaption of the channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetDefaultRFChannel(uint8_t channel)
{
    /* check for invalid channel */
    if((channel<100)||(channel>140))
    {
        /* invalid channel */
        return false;
    }
    return ThalassaPlug_Set(ThalassaPlug_USERSETTING_MEMPOSITION_DEFAULTCHANNEL, &channel, 1);
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
bool ThalassaPlug_SetVolatile_RFChannel(uint8_t channel)
{
    bool ret = false;

    /* check for invalid channel */
    if((channel<100)||(channel>140))
    {
        /* invalid channel */
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_SET_CHANNEL_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = channel;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /*
        *  module sends set value as response
        *  Wait4CNF checks if the channel in response euals requested channel
        */
        channelVolatile = channel;

        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_SET_CHANNEL_CNF, CMD_Status_Success, true);
        channelVolatile = CHANNELINVALID;
    }
    return ret;
}

/*
 *Set the volatile destination net ID
 *
 *input:
 * -destnetid: new destination net ID
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetVolatile_DestNetID(uint8_t destnetid)
{
    bool ret = false;
    if(destnetid > 255)
    {
        return false;
    }
    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_SET_DESTNETID_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = destnetid;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_SET_DESTNETID_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Set the volatile destination address
 *
 *input:
 * -destaddr_lsb: lsb of the new destination address value

 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb)
{
    bool ret = false;
    /* check of invalid destaddr_lsb */
    if(destaddr_lsb > 255)
    {
        return false;
    }

    uint8_t CMD_ARRAY[6];

    /* fill CMD_ARRAY packet */
    switch (addressmode)
    {
    case AddressMode_0:
    case AddressMode_1:
    case AddressMode_2:
    {
        CMD_ARRAY[0] = CMD_STX;
        CMD_ARRAY[1] = ThalassaPlug_CMD_SET_DESTADDR_REQ;
        CMD_ARRAY[2] = 0x01;
        CMD_ARRAY[3] = destaddr_lsb;
        ret = FillChecksum(CMD_ARRAY,5);
    }
    break;

    /* invalid address mode */
    default:
        return false;
    }

    if(ret == true)
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_SET_DESTADDR_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Transmit data using the configured settings
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_Transmit(uint8_t* payload, uint8_t length)
{
    bool ret = false;
    uint8_t numretries = -1;
    uint8_t responseLength = 1;

    if(length > MAX_PAYLOAD_LENGTH)
    {
        fprintf(stdout, "Data exceeds maximal payload length\n");
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + 4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_DATA_REQ;
    CMD_ARRAY[2] = length;
    memcpy(&CMD_ARRAY[3],payload,length);
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* check for value if numreties - if value == 0 no ack is expected */
        ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_NUMRETRYS, &numretries, &responseLength);

        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if(numretries > 0)
        {
            /* ack will be requested from receiver module. Once ack is received CMD_Status_Success will be sent */
            ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_DATA_CNF, CMD_Status_Success, true);
        }
        else
        {
            /* no ack will be requested from receiver module - In this case CMD_Status_Failed is sent immediately after transmission */
            ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_DATA_CNF, CMD_Status_Failed, true);
        }
    }
    return ret;
}

/*
 *Transmit data
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 * -channel: channel to be used
 * -dest_network_id: destination network ID
 * -dest_address_lsb: destination address lsb
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThalassaPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb)
{
    bool ret = false;
    uint8_t numretries = -1;
    uint8_t responseLength = 1;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + addressmode + 4 + 1];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = ThalassaPlug_CMD_DATAEX_REQ;

    switch (addressmode)
    {
    case AddressMode_0:
    {
        /* 1 byte isneeded for the channel */
        if(length > (MAX_PAYLOAD_LENGTH -1))
        {
            fprintf(stdout, "Data exceeds maximal payload length\n");
            return false;
        }

        CMD_ARRAY[2] = (length + 1);
        CMD_ARRAY[3] = channel;
        memcpy(&CMD_ARRAY[4],payload,length);
    }
    break;

    case AddressMode_1:
    {
        /* 2 bytes are needed for the channel and the destination address*/
        if(length > (MAX_PAYLOAD_LENGTH -2))
        {
            fprintf(stdout, "Data exceeds maximal payload length\n");
            return false;
        }

        CMD_ARRAY[2] = (length + 2);
        CMD_ARRAY[3] = channel;
        CMD_ARRAY[4] = dest_address_lsb;
        memcpy(&CMD_ARRAY[5],payload,length);
    }
    break;

    case AddressMode_2:
    {
        /* 3 bytes are needed for the channel, the destination address and the network id */
        if(length > (MAX_PAYLOAD_LENGTH -3))
        {
            fprintf(stdout, "Data exceeds maximal payload length\n");
            return false;
        }

        CMD_ARRAY[2] = (length + 3);
        CMD_ARRAY[3] = channel;
        CMD_ARRAY[4] = dest_network_id;
        CMD_ARRAY[5] = dest_address_lsb;
        memcpy(&CMD_ARRAY[6],payload,length);
    }
    break;

    default:
        /* wrong address mode */
        return false;
    }

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {

        ThalassaPlug_Get(ThalassaPlug_USERSETTING_MEMPOSITION_NUMRETRYS, &numretries, &responseLength);

        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if(numretries > 0)
        {
            /* ack will be requested from receiver module. Once ack is received CMD_Status_Success will be sent */
            ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_DATA_CNF, CMD_Status_Success, true);
        }
        else
        {
            /* no ack will be requested from receiver module - In this case CMD_Status_Failed is sent immediately after transmission */
            ret = Wait4CNF(CMD_WAIT_TIME, ThalassaPlug_CMD_DATA_CNF, CMD_Status_Failed, true);
        }
    }
    return ret;
}

/*
 *Configure the ThalassaPlug
 *
 *input:
 * -config: pointer to the configuration struct
 * -config_length: length of the configuration struct
 *
 *return true if request succeeded
 *       false otherwise
*/
bool ThalassaPlug_Configure(ThalassaPlug_Configuration_t* config, uint8_t config_length)
{
    int i = 0;
    uint8_t help_length;
    uint8_t help[MAX_USERSETTING_LENGTH];

    /* now check all settings and update them if necessary */
    for(i=0; i<config_length; i++)
    {
        /* read current value */
        if(false == ThalassaPlug_Get(config[i].usersetting, help, &help_length))
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
            if(false == ThalassaPlug_Set(config[i].usersetting, config[i].value, config[i].value_length))
            {
                /* error */
                return false;
            }
        }
        delay(200);
    }

    /* reset to take effect of the updated parameters */
    if(false == ThalassaPlug_PinReset())
    {
        return false;
    }
    return true;
}
