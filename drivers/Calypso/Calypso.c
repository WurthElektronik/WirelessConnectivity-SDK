/* ###########################################################################
* Includes:
*/
#include <stdio.h>

#include "../WE-common.h"
#include  "../global/global.h"
#include "ATCommands/events.h"
#include "ATCommands/ATDevice.h"
#include "Calypso.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/

#define CALYPSO_LINE_MAX_SIZE        2048
#define CALYPSO_RESPONSE_BUFFER_SIZE 2048
#define CALYPSO_EVENT_BUFFER_SIZE    2048
#define CALYPSO_MAX_CMD_LENGTH               64
/*
* Constant Macros.
* ########################### */

/* #############################
* Function Macros:
*/

/*
* Function Macros.
* ########################## */

/* #############################
* Typedefs:
*/

/*
* Typedefs.
* ########################## */

/*
* Macros & Typedefs.
* ######################################################################## */

/* ##########################################################################
* Declarations:
*/

/* #############################
* Exported Globals:
*/


/*
* Exported Globals.
* ########################## */

/* #############################
* Static Globals:
*/

static uint8_t Calypso_base64EncTable[64] =  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                              'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                              'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                              'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                              'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                              'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                              'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                              '4', '5', '6', '7', '8', '9', '+', '/'
                                             };

static uint8_t Calypso_base64DecTable[123] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,
                                              52,53,54,55,56,57,58,59,60,61, /* 0-9 */
                                              0,0,0,0,0,0,0,
                                              0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25, /* A-Z */
                                              0,0,0,0,0,0,
                                              26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51
                                             }; /* a-z */


static bool ResetUartRxThread;
static bool AbortUartRxThread;
static bool requestPending;

static char Calypso_respndCmd[CALYPSO_LINE_MAX_SIZE];
static char Calypso_eventCmd[CALYPSO_LINE_MAX_SIZE];
static size_t lengthResponse;


static Calypso_CNFStatus_t cmdConfirmation;

static char RxBuffer[CALYPSO_LINE_MAX_SIZE]; /* data buffer for RX */

pthread_t thread_read;

/*
* Static Globals.
* ########################## */

/* #############################
* Static Functions:
*/

void(*eventCallback)(char *); /* callback function for events*/

static void HandleRxLine(char *rxPacket, uint16_t rxLength);
void *rx_thread();

/*
* Static Functions.
* ########################## */

/*
* Declarations.
* ######################################################################## */

/* ###########################################################################
* Definitions:
*/

/* #############################
* Exported Functions:
*/

/**
 *  Initializes the serial communication with the module
 *
 * input:
 * - baudrate   baudrate of the serial communication
 * - parityBit  parity bit of serial communication
 * - evtCb      function pointer to event handler

 * return true if succesful, false otherwise
 */
bool Calypso_Init(int baudrate, Serial_ParityBit_t parityBit, void(*evtCb)(char *))
{
    ResetUartRxThread = false;
    AbortUartRxThread = false;
    requestPending = false;

    eventCallback = evtCb;
    OpenSerialWithParity(baudrate, parityBit); /* calypso default: 921600 Baud 8e1 */

    /* start rx thread */
    if(0 != pthread_create(&thread_read, NULL, rx_thread, NULL))
    {

    }
    return true;
}


/**
 *  Deinitializes the serial communication with the module
 *
 * return true if succesful, false otherwise
 */
bool Calypso_Deinit()
{
    AbortUartRxThread = true;
    eventCallback = NULL;
    CloseSerial();

    pthread_cancel(thread_read);
    return true;
}

/**
 *  Sends the at+command request to the module
 *
 * input:
 *  - data: at command to send
 *
 */
void Calypso_sendRequest(char *data)
{

    requestPending = true;
    lengthResponse = 0;
#ifdef DEBUG
    printf("> %s", data);
#endif
    SendBytes((uint8_t *)data, strlen(data));

}

/**
 * Waits for the response from the module after a request
 *
 * input:
 * - max_time_ms        maximum wait time in milliseconds
 * - expectedStatus     status to wait for
 * - reset_confirmstate resets the
 *
 * output:
 * - pOutResponse
 *
 * return true if succesful, false otherwise
 */
bool Calypso_Wait4CNF(int max_time_ms, Calypso_CNFStatus_t expectedStatus, bool reset_confirmstate, char *pOutResponse)
{
    int count = 0;
    int time_step_ms = 5; /* 5ms */
    int max_count = max_time_ms / time_step_ms;

    if(reset_confirmstate)
    {
        cmdConfirmation = Calypso_CNFStatus_Invalid;
    }


    while (1)
    {
        if(Calypso_CNFStatus_Invalid != cmdConfirmation)
        {
            requestPending = false;
            if(cmdConfirmation == expectedStatus)
            {
                if(NULL != pOutResponse)
                {
                    /* copy response for further pocessign*/
                    memcpy(pOutResponse, Calypso_respndCmd, lengthResponse);
                }
                return true;
            }
            else
            {
                return false;
            }

        }


        if (count >= max_count)
        {
            /* received no correct response within timeout */
            requestPending = false;
            return false;
        }

        /* wait */
        count++;
        delay(time_step_ms);
    }
    return false;
}

/**
 * Parses int to string
 *
 * input:
 * - pInInt     int value to parse
 * - intFlags   flags to determine how to parse
 *
 * output:
 * - pOutString parsed string
 *
 * return true if succesful, false otherwise
 */
bool Calypso_parseInt(char *pOutString, uint32_t pInInt, uint16_t intFlags)
{
    if((0 == (intFlags & INTFLAGS_SIGN)) || (0 == (intFlags & INTFLAGS_NOTATION)))
    {
        return false;
    }

    /* HEX*/
    if(INTFLAGS_NOTATION == (intFlags & INTFLAGS_NOTATION))
    {
        sprintf(pOutString, "0x%x", pInInt);
    }
    /* DEC */
    else
    {

        /* UNSIGNED */
        if(INTFLAGS_UNSIGNED == (intFlags & INTFLAGS_SIGN))
        {
            sprintf(pOutString, "%u", pInInt);
        }
        /* SIGNED */
        else
        {
            sprintf(pOutString, "%d", pInInt);
        }
    }

    return true;
}

/**
 * Parses int string to int
 *
 * input:
 * - pInString  int string to parse
 * - intFlags   flags to determine how to parse
 *
 * output:
 * - pOutInt parsed int value
 *
 * return true if succesful, false otherwise
 */
bool Calypso_StringToInt(void *pOutInt, const char *pInString, uint16_t intFlags)
{
    uint32_t resultUnsigned;
    int32_t resultSigned;
    uint8_t figure;
    size_t argumentLength = 0;
    char currentChar = '0';
    bool isNegative = false;

    if((NULL == pInString) || (NULL == pOutInt))
    {
        return false;
    }

    argumentLength = strlen(pInString);

    resultUnsigned = 0;
    resultSigned = 0;
    figure = 0;
    /* Hex notation starts with 0x */
    if(0 == strncmp(pInString, "0x", 2))
    {
        pInString+=2;
        argumentLength -=2 ;
        for(int i = 0; i < argumentLength; i++)
        {
            currentChar = pInString[i];

            if((currentChar >= '0') && (currentChar <= '9'))
            {
                figure = (0x0F & (uint8_t)(currentChar - '0'));
            }
            else if((currentChar >= 'A') && (currentChar <= 'F'))
            {
                figure = (0x0F & ((uint8_t)(currentChar - 'A') + 10));
            }
            else if((currentChar >= 'a') && (currentChar <= 'f'))
            {
                figure = (0x0F & ((uint8_t)(currentChar - 'a') + 10));
            }

            resultUnsigned = (resultUnsigned << 4) + figure;

        }

        memcpy(pOutInt, &resultUnsigned,(intFlags & INTFLAGS_SIZE));

    }
    else
    {
        if('-' == pInString[0])
        {
            isNegative = true;
            pInString++;
            argumentLength--;
        }

        for(int i = 0; i < argumentLength; i++)
        {
            figure = (uint8_t)(pInString[i] - '0');
            resultUnsigned = (resultUnsigned * 10) + figure;
        }

        if(isNegative)
        {
            resultSigned = 0 - resultUnsigned;
            memcpy(pOutInt, &resultSigned,(intFlags & INTFLAGS_SIZE));

        }
        else
        {
            memcpy(pOutInt, &resultUnsigned,(intFlags & INTFLAGS_SIZE));
        }
    }

    return true;

}

/**
 * Appends a string argument to the end of the at command
 *
 * input:
 * - pInArgument argument to add
 * - delimeter   delimeter to append after argument
 *
 * output:
 * - pOutString  at-command after appending argument
 *
 * return true if succesful, false otherwise
 */
bool Calypso_appendArgumentString(char *pOutString, const char *pInArgument, char delimeter)
{
    if((NULL == pOutString))
    {
        return false;
    }

    if(NULL != pInArgument)
    {
        strcpy(&pOutString[strlen(pOutString)], pInArgument);
    }
    pOutString[strlen(pOutString)] = delimeter;

    return true;
}

/**
 * Appends an int argument to the end of the at command
 *
 * input:
 * - pInValue  argument to add
 * - intFlags   flags to determine how to parse
 * - delimeter delimeter to append after argument
 *
 * output:
 * - pOutString  at-command after appending argument
 *
 * return true if succesful, false otherwise
 */
bool Calypso_appendArgumentInt(char *pOutString, uint32_t pInValue, uint16_t intflags, char delimeter)
{
    if((NULL == pOutString))
    {
        return false;
    }

    char tempString[12];

    if(Calypso_parseInt(tempString, pInValue, intflags))
    {

        return Calypso_appendArgumentString(pOutString, tempString, delimeter);
    }

    return false;
}


/**
 * Gets next string argument from at-command.
 *
 * input:
 * - pInArguments  at-command to get argument from
 * - delimeter     delimetet which occures after argument to get
 *
 * output:
 * - pOutargument  argument as string
 *
 * return true if succesful, false otherwise
 */
bool Calypso_getNextArgumentString(char **pInArguments, char * pOutargument, char delim)
{
    if((NULL == pInArguments) || (NULL == pOutargument))
    {
        return false;
    }

    size_t argumentLength = 0;
    size_t inputStringLength = strlen(*pInArguments);

    while(true)
    {
        if (((*pInArguments)[argumentLength] == delim))
        {
            /* if argument list not empty*/
            /* copy string to output arguments */
            memcpy(pOutargument, *pInArguments, argumentLength);
            memset(&pOutargument[argumentLength], '\0', 1);

            *pInArguments = &((*pInArguments)[argumentLength+1]);
            return true;
        }
        else if(argumentLength > inputStringLength)
        {
            return false;
        }
        argumentLength++;
    }

}

/**
 * Gets next int argument from at-command.
 *
 * input:
 * - pInArguments  at-command to get argument from
 * - intFlags   flags to determine how to parse
 * - delimeter     delimetet which occures after argument to get
 *
 * output:
 * - pOutargument  argument as parsed as int
 *
 * return true if succesful, false otherwise
 */
bool Calypso_getNextArgumentInt(char **pInArguments, void * pOutargument, uint16_t intflags, char delim)
{
    if((NULL == pInArguments) || (NULL == pOutargument))
    {
        return false;
    }

    char tempString[12];

    Calypso_getNextArgumentString(pInArguments, tempString, delim);
    return Calypso_StringToInt(pOutargument, tempString, intflags);

}

/**
 * Gets command name from at-command.
 *
 * input:
 * - pInArguments  at-command to get command name from
 * - delimeter     delimetet which occures after command name
 *
 * output:
 * - pCmdName      command name as string
 *
 * return true if succesful, false otherwise
 */
bool Calypso_getCmdName(char **pInAtCmd, char *pCmdName, char delim)
{
    if((NULL == pInAtCmd) || (NULL == pCmdName))
    {
        return false;
    }

    size_t argumentLength = 0;
    size_t inputStringLength = strlen(*pInAtCmd);

    while(true)
    {
        if (((*pInAtCmd)[argumentLength] == delim))
        {
            /* if argument list not empty*/
            /* copy string to output arguments */
            memcpy(pCmdName, *pInAtCmd, argumentLength);
            memset(&pCmdName[argumentLength], '\0', 1);

            *pInAtCmd = &((*pInAtCmd)[argumentLength+1]);
            return true;
        }
        else if(argumentLength > inputStringLength)
        {
            return false;
        }
        argumentLength++;
    }
}

/**
 * Get Base64 decoded buffer size
 *
 * This routine calculate the expected raw data buffer size
 * by given base64 buffer and buffer size
 *
 * input:
 * -   inputData       -  source buffer which hold the base64 data
 * -   inputLength     -  length of base64 data
 *
 * return         function shall return expected size.
*/
uint32_t Calypso_getBase64DecBufSize(uint8_t *inputData, uint32_t inputLength)
{
    uint32_t outputLength = inputLength / 4 * 3;

    if (outputLength == 0)
    {
        return 0;
    }
    if (inputData[inputLength - 1] == '=') outputLength--;
    if (inputData[inputLength - 2] == '=') outputLength--;

    return outputLength;
}

/**
 * Get Base64 encoded buffer size
 *
 * This routine calculate the expected Base64 buffer size
 * by given raw data size
 *
 * input:
 * - inputLength raw buffer size
 *
 * return function shall return expected size.
*/
uint32_t Calypso_getBase64EncBufSize(uint32_t inputLength)
{
    return (4 * ((inputLength + 2) / 3));
}

/**
 * Decode Base64 data to raw data
 *
 * This routine decode a given data in Base64 format to raw data,
 * and return it into a given buffer - outputData (which should be already allocated).
 * size of the outputData buffer also be returned
 *
 * input;
 *  - inputData    source buffer which hold the Base64 data
 *  - inputLength  source buffer size
 *
 * output:
 * - outputData    destination buffer which hold the raw data
 * - outputLength  destination buffer size
 *
 * return         true if successfull, false otherwise
*/
bool Calypso_decodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength)
{
    uint32_t decode_value;
    uint32_t nibble6_1, nibble6_2, nibble6_3, nibble6_4;
    uint32_t i, j;

    if (inputLength % 4 != 0)
    {
        return false;
    }

    if (outputData == NULL)
    {
        return false;
    }

    *outputLength = Calypso_getBase64DecBufSize(inputData, inputLength);

    for (i = 0, j = 0; i < inputLength;)
    {
        nibble6_1 = inputData[i] == '=' ? 0 & i++ : Calypso_base64DecTable[inputData[i++]];
        nibble6_2 = inputData[i] == '=' ? 0 & i++ : Calypso_base64DecTable[inputData[i++]];
        nibble6_3 = inputData[i] == '=' ? 0 & i++ : Calypso_base64DecTable[inputData[i++]];
        nibble6_4 = inputData[i] == '=' ? 0 & i++ : Calypso_base64DecTable[inputData[i++]];

        decode_value = (nibble6_1 << 3 * 6) + (nibble6_2 << 2 * 6)  + (nibble6_3 << 1 * 6) + (nibble6_4 << 0 * 6);

        if (j < *outputLength) outputData[j++] = (decode_value >> 2 * 8) & 0xFF;
        if (j < *outputLength) outputData[j++] = (decode_value >> 1 * 8) & 0xFF;
        if (j < *outputLength) outputData[j++] = (decode_value >> 0 * 8) & 0xFF;
    }
    outputData[j] = 0;

    return true;
}

/**
 * Encode raw data to Base64 format
 *
 *   This routine encode a given raw data to base64 format and return it into
 *   a given buffer - outputData (which should be already allocated).
 *   size of the outputData buffer also be returned
 *
 * input:
 * - inputData    source buffer which hold the raw data
 * - inputLength  source buffer size
 *
 * output:
 * - outputData   destination buffer which hold the base64 data
 * - outputLength destination buffer size
 *
 *return         true if successfull, false otherwise
 */
bool Calypso_encodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength)
{
    uint32_t encodeValue;
    uint32_t nibble6_1, nibble6_2, nibble6_3;
    uint32_t i, j;

    *outputLength = Calypso_getBase64EncBufSize(inputLength);

    if (outputData == NULL)
    {
        return false;;
    }

    for (i = 0, j = 0; i < inputLength;)
    {

        nibble6_1 = i < inputLength ? inputData[i++] : 0;
        nibble6_2 = i < inputLength ? inputData[i++] : 0;
        nibble6_3 = i < inputLength ? inputData[i++] : 0;

        encodeValue = (nibble6_1 << 0x10) + (nibble6_2 << 0x08) + nibble6_3;

        outputData[j++] = Calypso_base64EncTable[(encodeValue >> 3 * 6) & 0x3F];
        outputData[j++] = Calypso_base64EncTable[(encodeValue >> 2 * 6) & 0x3F];
        outputData[j++] = Calypso_base64EncTable[(encodeValue >> 1 * 6) & 0x3F];
        outputData[j++] = Calypso_base64EncTable[(encodeValue >> 0 * 6) & 0x3F];
    }

    if (inputLength % 3 >= 1) outputData[*outputLength - 1] = '=';
    if (inputLength % 3 == 1) outputData[*outputLength - 2] = '=';

    outputData[*outputLength] = 0;

    return true;
}
/*
* Exported Functions.
* ########################## */

/* #############################
* Static Functions:
*/

void *rx_thread()
{
    uint16_t RxByteCounter = 0;
    uint8_t readBuffer;
    bool returnFound = false;

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
                switch(RxByteCounter)
                {
                case 0:
                {
                    /* Possible responses: OK, Error, +[event], +[cmdResponse] */
                    if(('O' == readBuffer) || ('o' == readBuffer) || ('E' == readBuffer) || ('e' == readBuffer) || ('+' == readBuffer))
                    {
                        RxBuffer[RxByteCounter++] = readBuffer;
                    }
                    break;
                }
                default:
                {

                    if(RxByteCounter >= CALYPSO_LINE_MAX_SIZE)
                    {
                        ResetUartRxThread = true;
                    }

                    if(readBuffer == '\r')
                    {
//                        RxBuffer[RxByteCounter++] = readBuffer;
                        returnFound = true;
                    }
                    else if(returnFound && (readBuffer == '\n'))
                    {
//                        RxBuffer[RxByteCounter++] = readBuffer;
                        /* interprete it now */
                        RxBuffer[RxByteCounter++] = (uint8_t)'\0';
#ifdef DEBUG
                        printf("< %s\n", RxBuffer);
#endif
                        HandleRxLine(RxBuffer, RxByteCounter);
                        returnFound = false;
                        RxByteCounter = 0;
                    }
                    else
                    {
                        RxBuffer[RxByteCounter++] = readBuffer;
                    }


                    break;
                }
                }
            }
        }
    }

    return 0;
}


static void HandleRxLine(char *rxPacket, uint16_t rxLength)
{
    /* AT command was sent to module. Waiting fot response*/
    if(requestPending)
    {
        /* If starts with 'O', check if response is "OK\r\n" */
        if(('O' == rxPacket[0]) || ('o' == rxPacket[0]))
        {
            if(0 == strncasecmp(&rxPacket[0], RESPONSE_OK, strlen(RESPONSE_OK)))
            {
                cmdConfirmation = Calypso_CNFStatus_Success;
            }
            else
            {
                cmdConfirmation = Calypso_CNFStatus_Failed;
            }
        }
        /* If starts with 'E', check if response is "Error:[arguments]\r\n" */
        else if(('E' == rxPacket[0]) || ('e' == rxPacket[0]))
        {
            if(!(0 == strncasecmp(&rxPacket[0], RESPONSE_Error, strlen(RESPONSE_Error))))
            {

                cmdConfirmation = Calypso_CNFStatus_Success;
            }
            else
            {
                cmdConfirmation = Calypso_CNFStatus_Failed;

            }
        }
        else
        {
            if(rxLength < CALYPSO_RESPONSE_BUFFER_SIZE)
            {
                memcpy(&Calypso_respndCmd[lengthResponse], RxBuffer, rxLength);
                lengthResponse += rxLength;
            }
        }
    }
    else
        /* If no request is pending, an event was received*/
    {
        if(NULL != eventCallback)
        {
            memcpy(&Calypso_eventCmd[0], RxBuffer, rxLength);
            eventCallback(Calypso_eventCmd);
        }

    }

}
/*
* Static Functions.
* ########################## */

/*
* Definitions.
* ######################################################################### */
