/* ###########################################################################
* Includes:
*/

#include "ATDevice.h"
#include "../Calypso.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/


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

static const char *ATDevice_ATGetIdStrings[] =
{
    ATDEVICE_ID(GENERATE_STRING)
};

static const char *ATDevice_ATGetStatusStrings[] =
{
    ATDEVICE_STATUS(GENERATE_STRING)
};

static const char *ATDevice_ATGetGeneralStrings[] =
{
    ATDEVICE_GENERAL(GENERATE_STRING)
};

static const char *ATDevice_ATGetIotStrings[] =
{
    ATDEVICE_IOT(GENERATE_STRING)
};

static const char *ATDevice_ATGetUartStrings[] =
{
    ATDEVICE_UART(GENERATE_STRING)
};


/*
* Static Globals.
* ########################## */

/* #############################
* Static Functions:
*/

static bool ATDevice_isInputValidATget(uint8_t id, uint8_t option);
static bool ATDevice_isInputValidATset(uint8_t id, uint8_t option);
static bool ATDevice_addArgumentsATget(char *pAtCommand, uint8_t id, uint8_t option);
static bool ATDevice_addArgumentsATset(char *pAtCommand, uint8_t id, uint8_t option, void *pValues);
static bool ATDevice_parseResponseATget(uint8_t id, uint8_t option, char *pAtCommand, void *pValues, size_t *pValuesSize);

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
 * Sends the AT+test command
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_test()
{
    bool ret = false;

    Calypso_sendRequest("AT+test\r\n");
    ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);

    return ret;
}

/**
 * Sends the AT+start command
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_start()
{
    bool ret = false;

    Calypso_sendRequest("AT+start\r\n");
    ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);

    return ret;
}

/**
 * Sends the AT+stop=[timeout] command
 *
 * input:
 *  - timeoutInMS   Timeout in milliseconds.
 *
 * return true if command was sent successful, false otherwise
 */

bool ATDevice_stop(uint32_t timeoutInMS)
{

    bool ret = false;

    if(timeoutInMS <= AT_STOP_MAX_TIMEOUT)
    {

        char cmdToSend[32] = "AT+stop=";

            ret = Calypso_parseInt(&cmdToSend[strlen(cmdToSend)], timeoutInMS, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));


        if(ret)
        {
            Calypso_appendArgumentString(cmdToSend, CRLF,STRING_TERMINATE);
        }

        if(ret)
        {
            Calypso_sendRequest(cmdToSend);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
        }
    }

    return ret;
}

/**
 * Sends the AT+reboot command
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_reboot()
{
    bool ret = false;

    Calypso_sendRequest("AT+reboot\r\n");
    ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);

    return ret;
}

/**
 * Sends the AT+factoryreset command
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_factoryReset()
{
    bool ret = false;

    Calypso_sendRequest("AT+factoryreset\r\n");
    ret = Calypso_Wait4CNF(30000, Calypso_CNFStatus_Success, true, NULL);

    return ret;
}

/**
 * Sends the AT+start command
 *
 * input:
 *  - timeoutInSeconds    Timeout, until the module wakes up again. Has to be 0 < timeoutInSeconds < 86400
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_sleep(uint32_t timeoutInSeconds)
{

    bool ret = false;

    if((timeoutInSeconds >= AT_SLEEP_MIN_TIMEOUT) && (timeoutInSeconds <= AT_SLEEP_MAX_TIMEOUT))
    {

        char cmdToSend[32] = "AT+sleep=";

        ret = Calypso_parseInt(&cmdToSend[strlen(cmdToSend)], timeoutInSeconds, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));

        if(ret)
        {
            Calypso_appendArgumentString(cmdToSend, CRLF,STRING_TERMINATE);
        }


        if(ret)
        {
            Calypso_sendRequest(cmdToSend);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
        }
    }

    return ret;
}

/**
 * Sends a AT+get=status,option command
 *
 * input:
 *  - id      ID to get. Value has to be ATDeviceId_t
 *  - option  Option to get. Valid values depend on id
 *
 * output:
 *  - pValues      values get from module
 *  - pValuesSize  length of pValues in bytes
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_get(uint8_t id, uint8_t option, void *pValues, size_t *pValuesSize)
{

    bool ret = false;

    if(ATDevice_isInputValidATget(id, option))
    {
        char cmdToSend[32] = "AT+get=";
        char response[128];

        ret = ATDevice_addArgumentsATget(&cmdToSend[strlen(cmdToSend)], id, option);


        if(ret)
        {
            Calypso_sendRequest(cmdToSend);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, &response[0]);
        }

        if(ret)
        {
            ret = ATDevice_parseResponseATget(id, option, &response[0], pValues, pValuesSize);

        }

    }

    return ret;
}



/**
 * Sends a AT+set=status,option command
 *
 * input:
 * -   id      ID to get. Valid ids are general and UART
 * -   option  Option to get. Valid values depend on id
 *
 * output:
 * -   pValues values for the specific id/option
 *
 * return true if command was sent successful, false otherwise
 */
bool ATDevice_set(uint8_t id, uint8_t option, void *pValues)
{

    bool ret = false;

    if(ATDevice_isInputValidATset(id, option))
    {
        char cmdToSend[64] = "AT+set=";


        ret = ATDevice_addArgumentsATset(&cmdToSend[strlen(cmdToSend)], id, option,pValues);


        if(ret)
        {
            Calypso_sendRequest(cmdToSend);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
        }
    }

    return ret;
}
/*
* Exported Functions.
* ########################## */

/* #############################
* Static Functions:
*/


/**
 * Checks if parameters are valid for the command
 *
 * input:
 * -  id           the id to add
 * -  option       the option to add
 *
 * return true if arguments are valid, false otherwise
 */
static bool ATDevice_isInputValidATget(uint8_t id, uint8_t option)
{
    bool ret = false;

    if(id < ATDEVICE_ID_NumberOfValues)
    {
        switch(id)
        {
        case ATDEVICE_ID_status:
        {
            ret = (option < ATDEVICE_STATUS_NumberOfValues);
            break;
        }
        case ATDEVICE_ID_general:
        {
            ret  = (option < ATDEVICE_GENERAL_NumberOfValues);
            break;
        }
        case ATDEVICE_ID_IOT:
        {
            ret = (option < ATDEVICE_IOT_NumberOfValues);
            break;
        }
        case ATDEVICE_ID_UART:
        {
            ret = (option < ATDEVICE_UART_NumberOfValues);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    return ret;
}


/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  id           the id to add
 * -  option       the option to add
 *
 * output:
 *  - pAtCommand   the request command string to add the arguments to
 * return true if arguments were added successful, false otherwise
 */
static bool ATDevice_addArgumentsATget(char *pAtCommand, uint8_t id, uint8_t option)
{
    bool ret = false;


    ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetIdStrings[id], ARGUMENT_DELIM);
    if(ret)
    {
        switch(id)
        {
        case ATDEVICE_ID_status:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetStatusStrings[option], STRING_TERMINATE);

            break;
        }
        case ATDEVICE_ID_general:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetGeneralStrings[option], STRING_TERMINATE);

            break;
        }
        case ATDEVICE_ID_IOT:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetIotStrings[option], STRING_TERMINATE);

            break;
        }
        case ATDEVICE_ID_UART:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetUartStrings[option], STRING_TERMINATE);

            break;
        }
        default:
        {
            break;
        }
        }
    }

    if(ret)
    {
        Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 *  Parses the response
 *
 * input:
 * - id           the id to get
 * - option       the option to get
 * - pAtCommand    the string reponse by the module
 *
 * output:
 *  - pValues      values get from module
 *  - pValuesSize  length of pValues in bytes
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATDevice_parseResponseATget(uint8_t id, uint8_t option, char *pAtCommand, void *pValues, size_t *pValuesSize)
{
    bool ret = false;
    const char *cmd = "+get:";
    const size_t cmdLength = strlen(cmd);

    /* check if reponse is for get*/
    ret = (0 == strncmp(pAtCommand, cmd, cmdLength));
    if(ret)
    {
        pAtCommand += cmdLength;
        switch(id)
        {
        case ATDEVICE_ID_status:
        {
            /* All options return a bitmask */
            char tempString[12];

            ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],ARGUMENT_DELIM);
            ret = Calypso_StringToInt(pValues, &tempString[0], INTFLAGS_SIZE32);

            *pValuesSize = INTFLAGS_SIZE32;
            ret = true;
            break;
        }

        case ATDEVICE_ID_general:
        {
            switch(option)
            {
            case ATDEVICE_GENERAL_version:
            {
                ATDevice_Version_t *version = (ATDevice_Version_t *)pValues;

                ret = Calypso_getNextArgumentString(&pAtCommand, &(version->chipId[0]), ARGUMENT_DELIM);

                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &(version->FWVersion[0]), ARGUMENT_DELIM);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &(version->PHYVersion[0]), ARGUMENT_DELIM);
                }
                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &(version->NWPVersion[0]), ARGUMENT_DELIM);
                }
                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &(version->ROMVersion[0]), STRING_TERMINATE);
                }
                if(ret)
                {
                    *pValuesSize = sizeof(ATDevice_Version_t);
                }
                break;
            }
            case ATDEVICE_GENERAL_time:
            {
                ATDevice_Time_t *time = (ATDevice_Time_t *)pValues;
                char tempString[6];

                ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],ARGUMENT_DELIM);
                ret = Calypso_StringToInt(&(time->hour), &tempString[0], INTFLAGS_SIZE8);


                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],ARGUMENT_DELIM);
                    ret = Calypso_StringToInt(&(time->minute), &tempString[0], INTFLAGS_SIZE8);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],ARGUMENT_DELIM);
                    ret = Calypso_StringToInt(&(time->second), &tempString[0], INTFLAGS_SIZE8);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],ARGUMENT_DELIM);
                    ret = Calypso_StringToInt(&(time->day), &tempString[0], INTFLAGS_SIZE8);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],ARGUMENT_DELIM);
                    ret = Calypso_StringToInt(&(time->month), &tempString[0], INTFLAGS_SIZE8);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],STRING_TERMINATE);
                    ret = Calypso_StringToInt(&(time->year), &tempString[0], INTFLAGS_SIZE16);
                }

                if(ret)
                {
                    *pValuesSize = sizeof(ATDevice_Time_t);
                }
                break;
            }
            case ATDEVICE_GENERAL_persistent:
            {
                char tempString[6];
                ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],STRING_TERMINATE);
                ret = Calypso_StringToInt(pValues, &tempString[0], INTFLAGS_SIZE8);
                *pValuesSize = INTFLAGS_SIZE8;
                ret = true;
                break;

            }
            default:
            {
                break;
            }
            }
            break;
        }

        case ATDEVICE_ID_IOT:
        {
            char tempString[12];
            ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],STRING_TERMINATE);
            ret = Calypso_StringToInt(pValues, &tempString[0], INTFLAGS_SIZE16);
            *pValuesSize = INTFLAGS_SIZE16;
            ret = true;
            break;
        }

        case ATDEVICE_ID_UART:
        {
            switch(option)
            {
            case ATDEVICE_UART_baudrate:
            {
                char tempString[12];
                ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],STRING_TERMINATE);
                ret = Calypso_StringToInt(pValues, &tempString[0], INTFLAGS_SIZE32);

                *pValuesSize = INTFLAGS_SIZE32;
                ret = true;
                break;
            }
            case ATDEVICE_UART_parity:
            {
                char tempString[6];
                ret = Calypso_getNextArgumentString(&pAtCommand, &tempString[0],STRING_TERMINATE);
                ret = Calypso_StringToInt(pValues, &tempString[0], INTFLAGS_SIZE8);
                *pValuesSize = INTFLAGS_SIZE8;
                ret = true;
                break;
            }
            }
        }

        default:
        {
            break;
        }
        }
    }

    return ret;
}

/**
 * Checks if parameters are valid for the command
 *
 * input:
 * -  id           the id to add
 * -  option       the option to add
 *
 * return true if arguments are valid, false otherwise
 */
static bool ATDevice_isInputValidATset(uint8_t id, uint8_t option)
{
    bool ret = false;

    switch(id)
    {
    case ATDEVICE_ID_general:
    {
        /* only option time and persistent can be written*/
        ret  = ((ATDEVICE_GENERAL_persistent == option) || (ATDEVICE_GENERAL_time == option));
        break;
    }

    case ATDEVICE_ID_UART:
    {
        ret = (option < ATDEVICE_UART_NumberOfValues);
        break;
    }
    default:
    {
        break;
    }
    }


    return ret;
}

/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  id           the id to add
 * -  option       the option to add
 *
 * output:
 *  - pAtCommand   the request command string to add the arguments to
 * return true if arguments were added successful, false otherwise
 */
static bool ATDevice_addArgumentsATset(char *pAtCommand, uint8_t id, uint8_t option, void *pValues)
{
    bool ret = false;

    /* add id*/
    ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetIdStrings[id], ARGUMENT_DELIM);

    switch(id)
    {
    case ATDEVICE_ID_general:
    {
        /* add option*/
        if(ret)
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetGeneralStrings[option], ARGUMENT_DELIM);
        }

        if(ATDEVICE_GENERAL_persistent == option)
        {
            char tempString[4];
            uint32_t *persistent = (uint32_t*)pValues;
            Calypso_parseInt(&tempString[0], *persistent, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
            ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], STRING_TERMINATE);
        }
        else if(ATDEVICE_GENERAL_time == option)
        {
            ATDevice_Time_t *time = pValues;
            char tempString[4];

            if(ret)
            {
                Calypso_parseInt(&tempString[0], time->hour, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
                ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], ARGUMENT_DELIM);
            }
            if(ret)
            {
                Calypso_parseInt(&tempString[0], time->minute, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
                ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], ARGUMENT_DELIM);
            }
            if(ret)
            {
                Calypso_parseInt(&tempString[0], time->second, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
                ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], ARGUMENT_DELIM);
            }
            if(ret)
            {
                Calypso_parseInt(&tempString[0], time->day, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
                ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], ARGUMENT_DELIM);
            }
            if(ret)
            {
                Calypso_parseInt(&tempString[0], time->month, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
                ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], ARGUMENT_DELIM);
            }
            if(ret)
            {
                Calypso_parseInt(&tempString[0], time->year, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
                ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], STRING_TERMINATE);
            }
        }

        break;
    }

    case ATDEVICE_ID_UART:
    {
        char tempString[12];
        uint32_t *baudrate = (uint32_t *)pValues;

        ret = Calypso_appendArgumentString(pAtCommand, ATDevice_ATGetUartStrings[option], ARGUMENT_DELIM);

        Calypso_parseInt(&tempString[0], *baudrate, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));
        ret = Calypso_appendArgumentString(pAtCommand, &tempString[0], STRING_TERMINATE);

        break;
    }
    default:
    {
        break;
    }
    }

    if(ret)
    {
        Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}
/*
* Static Functions.
* ########################## */

/*
* Definitions.
* ######################################################################### */
