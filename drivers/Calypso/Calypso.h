#ifndef CALYPSO_H_INCLUDED
#define CALYPSO_H_INCLUDED


/* ###########################################################################
* Includes:
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../global/global.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/


#define COMMAND_PREFIX  "AT+"
#define COMMAND_DELIM   (char)'='
#define CONFIRM_PREFIX  (char)'+'
#define CONFIRM_DELIM   (char)':'
#define EVENT_PREFIX    (char)'+'
#define EVENT_DELIM     (char)':'
#define ARGUMENT_DELIM  (char)','
#define BITMASK_DELIM   (char)'|'
#define CRLF            "\r\n"

#define RESPONSE_OK     "OK"
#define RESPONSE_Error  "error"

#define STRING_TERMINATE '\0'
#define STRING_EMPTY     ""

#define INTFLAGS_SIZE   (uint16_t)(7)
#define INTFLAGS_SIZE8  (uint16_t)(1)
#define INTFLAGS_SIZE16 (uint16_t)(2)
#define INTFLAGS_SIZE32 (uint16_t)(4)

#define INTFLAGS_SIGN       (uint16_t)(0x18)
#define INTFLAGS_SIGNED     (uint16_t)(0x08)
#define INTFLAGS_UNSIGNED   (uint16_t)(0x10)

#define INTFLAGS_NOTATION        (uint16_t)(0x60)
#define INTFLAGS_NOTATION_HEX    (uint16_t)(0x20)
#define INTFLAGS_NOTATION_DEC    (uint16_t)(0x40)
/*
* Constant Macros.
* ########################### */

/* #############################
* Function Macros:
*/

#define GENERATE_ENUM(PREFIX, ENUM) PREFIX##ENUM,
#define GENERATE_STRING(PREFIX, STRING) #STRING,

/*
* Function Macros.
* ########################## */
/* #############################
* Typedefs:
*/

typedef enum Calypso_CNFStatus_t
{
    Calypso_CNFStatus_Success = 0x00,
    Calypso_CNFStatus_Error,
    Calypso_CNFStatus_Failed,
    Calypso_CNFStatus_Invalid,
} Calypso_CNFStatus_t;

typedef enum Calypso_DataFormat_t
{
    Calypso_DataFormat_Binary,
    Calypso_DataFormat_Base64,
    Calypso_DataFormat_NumberOfValues,
} Calypso_DataFormat_t;
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
* Exported Variables:
*/


/*
* Exported Variables.
* ########################## */

/* #############################
* Exported Functions:
*/

extern bool Calypso_Init(int baudrate, Serial_ParityBit_t parityBit, void(*evtCb)(char *));
extern bool Calypso_Deinit();

extern bool Calypso_startRxThread();
extern void Calypso_stopRxThread();
extern void Calypso_sendRequest(char *data);
extern bool Calypso_Wait4CNF(int max_time_ms, Calypso_CNFStatus_t expectedStatus, bool reset_confirmstate, char *pOutResponse);

extern uint32_t Calypso_getBase64DecBufSize(uint8_t *inputData, uint32_t inputLength);
extern uint32_t Calypso_getBase64EncBufSize(uint32_t inputLength);
extern bool Calypso_decodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength);
extern bool Calypso_encodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength);


extern bool Calypso_parseInt(char *pOutString, uint32_t pInInt, uint16_t intFlags);
extern bool Calypso_StringToInt(void *pOutInt, const char *pInString, uint16_t intFlags);

extern bool Calypso_appendArgumentString(char *pOutString, const char *pInArgument, char delimeter);
extern bool Calypso_appendArgumentInt(char *pOutString, uint32_t pInValue, uint16_t intflags, char delimeter);
extern bool Calypso_getNextArgumentString(char **pInArguments, char * pOutargument, char delim);
extern bool Calypso_getNextArgumentInt(char **pInArguments, void * pOutargument, uint16_t intflags, char delim);
extern bool Calypso_getCmdName(char **pInAtCmd, char *pCmdName, char delim);
/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */

#endif // CALYPSO_H_INCLUDED
