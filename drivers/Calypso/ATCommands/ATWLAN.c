/* ###########################################################################
* Includes:
*/

#include "../Calypso.h"
#include "ATWLAN.h"


/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/
#define ATWLAN_MAX_BUFFER_SIZE 2048

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

static const char *ATWLAN_SetModeStrings[] =
{
    ATWLAN_SET_MODE(GENERATE_STRING)
};

static const char *ATWLAN_SetGetIDStrings[] =
{
    ATWLAN_SET_ID(GENERATE_STRING)
};

static const char *ATWLAN_SetGetGeneralStrings[] =
{
    ATWLAN_SET_GENERAL(GENERATE_STRING)
};

static const char *ATWLAN_SetGetP2PStrings[] =
{
    ATWLAN_SET_P2P(GENERATE_STRING)
};

static const char *ATWLAN_SetGetAPStrings[] =
{
    ATWLAN_SET_AP(GENERATE_STRING)
};

static const char *ATWLAN_PolicyStrings[] =
{
    ATWLAN_POLICY_ID(GENERATE_STRING)
};

static const char *ATWLAN_PolicyConnectionStrings[] =
{
    ATWLAN_POLICY_CONNECTION(GENERATE_STRING)
};

static const char *ATWLAN_PolicyScanStrings[] =
{
    ATWLAN_POLICY_SCAN(GENERATE_STRING)
};

static const char *ATWLAN_PolicyPMStrings[] =
{
    ATWLAN_POLICY_PM(GENERATE_STRING)
};

static const char *ATWLAN_PolicyP2PStrings[] =
{
    ATWLAN_POLICY_P2P(GENERATE_STRING)
};

static const char *ATWLAN_PolicyP2PValueStrings[] =
{
    ATWLAN_POLICY_P2P_VALUE(GENERATE_STRING)
};


static const char *ATWLAN_SecurityTypeStrings[] =
{
    ATWLAN_SECURITY_TYPE(GENERATE_STRING)
};

static const char *ATWLAN_ScanSecurityTypeStrings[] =
{
    ATWLAN_SCAN_SECURITY_TYPE(GENERATE_STRING)
};

static const char *ATWLAN_ScanCipherStrings[] =
{
    ATWLAN_SCAN_CIPHER_TYPE(GENERATE_STRING)
};

static const char *ATWLAN_ScanKeyMgmntStrings[] =
{
    ATWLAN_SCAN_KEYMGMNT_TYPE(GENERATE_STRING)
};

static const char *ATWLAN_SecurityEAPStrings[] =
{
    ATWLAN_SECURITY_EAP(GENERATE_STRING)
};

char responseBuffer[ATWLAN_MAX_BUFFER_SIZE];
char requestBuffer[ATWLAN_MAX_BUFFER_SIZE];

char *pRequestCommand;
char *pRespondCommand;

/*
* Static Globals.
* ########################## */

/* #############################
* Static Functions:
*/

static bool ATWLAN_isInputValidWlanGet(uint8_t id, uint8_t option);
static bool ATWLAN_isInputValidWlanSet(uint8_t id, uint8_t option);
static bool ATWLAN_isInputValidWlanPolicySet(uint8_t id, uint8_t option);

static bool ATWLAN_addConnectionArguments(char *pOutString, ATWLAN_ConnectionArguments_t connectionArgs, char lastDelim);
static bool ATWLAN_addArgumentsWlanGet(char *pAtCommand, uint8_t id, uint8_t option);
static bool ATWLAN_addArgumentsWlanSet(char *pAtCommand, uint8_t id, uint8_t option, void *pValues);
static bool ATWLAN_addArgumentsWlanPolicySet(char *pAtCommand, uint8_t id, uint8_t option, void *pValues);


static bool ATWLAN_parseResponseWlanScanEntry(char **pAtCommand, ATWLAN_ScanEntry_t *pOutScanEntry);
static bool ATWLAN_parseResponseWlanAddProfile(char **pAtCommand, uint8_t *pOutIndex);
static bool ATWLAN_parseResponseWlanGetProfile(char **pAtCommand, ATWLAN_Profile_t *pOutProfile);
static bool ATWLAN_parseResponseWlanGet(uint8_t id, uint8_t option, char **pAtCommand, void *pValues);
static bool ATWLAN_parseResponseWlanPolicyGet(uint8_t type, char **pAtCommand, uint8_t *pOption, void *pValues);

static bool ATWLAN_parseSetMode(const char *setModeString, ATWLAN_SetMode_t *pOutSetMode);
static bool ATWLAN_parseSecurityType(const char *securityTypeString, ATWLAN_SecurityType_t *pOutSecurityType);
static bool ATWLAN_parseScanSecurityType(const char *scansecurityTypeString, ATWLAN_ScanSecurityType_t *pOutScanSecurityType);
static bool ATWLAN_parseScanCipher(const char *scanCipherString, ATWLAN_ScanCipher_t *pOutScanCipher);
static bool ATWLAN_parseScanKeyMngmt(const char *scanKeyMgmntString, ATWLAN_ScanKeyMgmnt_t *pOutScanKeyMgmnt);
static bool ATWLAN_parseSecurityEAP(const char *securityEAPString, ATWLAN_Security_EAP_t *pOutSecurityEAP);
static bool ATWLAN_parsePolicyOption(uint8_t policyId, const char *policyOptionString, uint8_t *pOutPolicyOption);

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
 * Sends the AT+wlanSetMode=[mode] command
 *
 * input:
 * - mode    new mode of the module. See ATWLAN_SetMode_t for valid values
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_setMode(ATWLAN_SetMode_t mode)
{
    bool ret = false;

    if(mode < ATWLAN_SET_MODE_NumberOfValues)
    {
        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanSetMode=");

        ret = Calypso_appendArgumentString(pRequestCommand, ATWLAN_SetModeStrings[mode], (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC));


        if(ret)
        {
            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
        }
    }

    return ret;
}

/**
 * Sends the AT+wlanScan=[index],[deviceCount] command
 * first time scan is called, list will be initialized and module will return error
 *
 * input:
 * -   index           Index, from which on the entries should be get
 * -   deviceCound     Max number entries to get
 *
 * output:
 * -  pOutValues      The scan entries the module has returned
 * -  pOutNumEntries  Number of entries the module has returned
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_scan(uint8_t index, uint8_t deviceCount, ATWLAN_ScanEntry_t *pOutValues, uint8_t *pOutNumEntries)
{
    bool ret = false;

    if((index < 30) && (deviceCount < 30))
    {

        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanScan=");

        pRespondCommand = &responseBuffer[0];


        ret = Calypso_appendArgumentInt(pRequestCommand, index, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), ARGUMENT_DELIM);

        if(ret)
        {
            ret = Calypso_appendArgumentInt(pRequestCommand, deviceCount, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pRequestCommand, CRLF,STRING_TERMINATE);
        }

        if(ret)
        {
            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(1000, Calypso_CNFStatus_Success, true, pRespondCommand);
        }

        if(ret)
        {
            uint8_t NumEntries = 0;
            for(int i = 0; i < deviceCount; i++)
            {
                if(ATWLAN_parseResponseWlanScanEntry(&pRespondCommand, (ATWLAN_ScanEntry_t *)&pOutValues[NumEntries]))
                {
                    NumEntries++;
                }
            }

            *pOutNumEntries = NumEntries;
        }
    }

    return ret;
}

/**
 *  Sends the AT+wlanConnect=[SSID],[BSSID],[SecurityType],[SecurityKey],[SecurityExtUser],[SecurityExtAnonUsed],[SecurityExtEapMethod] command
 *
 * input:
 *  - connectArgs    connectArgs contains the parameters for the connect command. See ATWLAN_ConnectionArguments_t
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_connect(ATWLAN_ConnectionArguments_t connectArgs)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+wlanConnect=");

    ret = ATWLAN_addConnectionArguments(pRequestCommand, connectArgs, STRING_TERMINATE);

    if(ret)
    {
        ret = Calypso_appendArgumentString(pRequestCommand, CRLF,STRING_TERMINATE);
    }

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 * Sends the AT+disconnect command
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_disconnect()
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+wlanDisconnect\r\n");

    Calypso_sendRequest(pRequestCommand);
    ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);

    return ret;
}

/**
 *  Sends the AT+wlanProfileAdd=[SSID],[BSSID],[SecurityType],[SecurityKey],[SecurityExtUser],[SecurityExtAnonUsed],[SecurityExtEapMethod],[priority] command
 *
 * input:
 * -   connectArgs    connectArgs contains the parameters for the connect command. See ATWLAN_ConnectionArguments_t
 * -   priority       priority of the profile to be added
 *
 * output:
 * -  pOutIndex      the index of the profile within the module. Can be used to access the profile.
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_profileAdd(ATWLAN_ConnectionArguments_t connectionArgs, uint8_t priority, uint8_t *pOutIndex)
{

    bool ret = false;

    if((pOutIndex != NULL) && (priority < 16))
    {
        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanProfileAdd=");

        pRespondCommand = &responseBuffer[0];


        ret = ATWLAN_addConnectionArguments(pRequestCommand, connectionArgs, ARGUMENT_DELIM);

        if(ret)
        {
            ret = Calypso_appendArgumentInt(pRequestCommand, priority, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pRequestCommand, CRLF,STRING_TERMINATE);
        }

        if(ret)
        {

            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
        }

        if(ret)
        {
            ATWLAN_parseResponseWlanAddProfile(&pRespondCommand, pOutIndex);
        }
    }

    return ret;
}

/**
 *  Sends the AT+wlanProfileGet=[index] command
 *
 * input:
 *   - index           the index of the profile. Return value of ATWLAN_profileAdd.
 *
 * output:
 * -  pOutProfile     the profile as present in the module. See ATWLAN_Profile_t.
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_profileGet(uint8_t index, ATWLAN_Profile_t *pOutProfile)
{
    bool ret = false;

    if(NULL == pOutProfile)
    {
        return false;
    }

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+wlanProfileGet=");

    pRespondCommand = &responseBuffer[0];


    ret = Calypso_appendArgumentInt(pRequestCommand, index, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);

    if(ret)
    {
        Calypso_appendArgumentString(pRequestCommand, CRLF, STRING_TERMINATE);
    }

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
    }

    if(ret)
    {
        ret = ATWLAN_parseResponseWlanGetProfile(&pRespondCommand, pOutProfile);
    }

    return ret;
}


/**
 * Sends the AT+wlanProfileDel=[index] command
 *
 * input:
 *  - index  the index of the profile. Return value of ATWLAN_profileAdd.
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_profileDel(uint8_t index)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+wlanProfileDel=");

    ret = Calypso_appendArgumentInt(pRequestCommand, index, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);

    if(ret)
    {
        Calypso_appendArgumentString(pRequestCommand, CRLF, STRING_TERMINATE);
    }

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 * Sends the AT+wlanProfileGet=[id],[option] command
 *
 * input:
 * -   id           the id of the value to get
 * -   option       the option of the value to get
 *
 * output:
 * -  pValues      requested values from the module
 * -  pValuesSize  length(in bytes) of the returned values
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_get(uint8_t id, uint8_t option, void *pValues, size_t *pValuesSize)
{

    bool ret = false;

    if(ATWLAN_isInputValidWlanGet(id, option))
    {
        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanGet=");

        pRespondCommand = &responseBuffer[0];

        ret = ATWLAN_addArgumentsWlanGet(pRequestCommand, id, option);


        if(ret)
        {
            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
        }

        if(ret)
        {
            ret = ATWLAN_parseResponseWlanGet(id, option, &pRespondCommand, pValues);

        }

    }

    return ret;
}

/**
 *  Sends the AT+wlanProfileSet=[id],[option], [values1],...[values[x] command
 *
 * input:
 * -   id           the id of the value to set
 * -   option       the option of the value to set
 * -   pValues      values to set
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_set(uint8_t id, uint8_t option, void *pValues)
{
    bool ret = false;

    if(ATWLAN_isInputValidWlanSet(id, option))
    {
        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanSet=");

        ret = ATWLAN_addArgumentsWlanSet(pRequestCommand, id, option,pValues);


        if(ret)
        {
            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
        }
    }

    return ret;
}


/**
 * sends the AT+wlanPolicySet=[id],[option], [value] command
 *
 * input:
 * -   id           the id of the policy to set
 * -   option       the option of the policy to set
 * -   pValues      value to set
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_policySet(uint8_t id, uint8_t option, void *pValues)
{
    bool ret = false;

    if(ATWLAN_isInputValidWlanPolicySet(id, option))
    {
        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanPolicySet=");

        ret = ATWLAN_addArgumentsWlanPolicySet(pRequestCommand, id, option,pValues);


        if(ret)
        {
            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
        }
    }

    return ret;

}

/**
 *  Sends the AT+wlanPolicySet=[id],[option], [value] command
 *
 * input:
 * -   id           the id of the policy to set
 *
 * output:
 * -  pOutOption   the option of the policy to set
 * -  pOutValue    value to set
 *
 * return true if command was sent successful, false otherwise
 */
bool ATWLAN_policyGet(uint8_t type, uint8_t *pOutOption, void *pOutValue)
{

    bool ret = false;

    if(type < ATWLAN_POLICY_ID_NumberOfValues)
    {
        pRequestCommand = &requestBuffer[0];
        memset(pRequestCommand, 0, ATWLAN_MAX_BUFFER_SIZE);
        strcpy(pRequestCommand, "AT+wlanPolicyGet=");

        pRespondCommand = &responseBuffer[0];

        ret = Calypso_appendArgumentString(pRequestCommand, ATWLAN_PolicyStrings[type], STRING_TERMINATE);
        ret = Calypso_appendArgumentString(pRequestCommand, CRLF, STRING_TERMINATE);

        if(ret)
        {
            Calypso_sendRequest(pRequestCommand);
            ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
        }

        if(ret)
        {
            ret = ATWLAN_parseResponseWlanPolicyGet(type, &pRespondCommand, pOutOption, pOutValue);

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
 *  Checks of the id and option are valid for this command
 *
 * input:
 *  - id      the id of the command
 *  - option  the option of the command
 *
 * return true if id and option are valid, false otherwise
 */
static bool ATWLAN_isInputValidWlanGet(uint8_t id, uint8_t option)
{
    bool ret = false;

    if(id < ATWLAN_SET_ID_NumberOfValues)
    {
        switch(id)
        {
        case ATWLAN_SET_ID_general:
        {
            /* Only countrycode, sta_tx_power and scan_params are valid */
            ret = (option == ATWLAN_SET_GENERAL_COUNTRY_CODE) || (option == ATWLAN_SET_GENERAL_STA_TX_POWER) || (option == ATWLAN_SET_GENERAL_SCAN_PARAMS);
            break;
        }
        case ATWLAN_SET_ID_P2P:
        {
            /* All options valid */
            ret  = (option < ATWLAN_SET_P2P_NumberOfValues);
            break;
        }
        case ATWLAN_SET_ID_AP:
        {
            /* all option but MAX_STA_AGING are valid*/
            ret = (option < ATWLAN_SET_AP_NumberOfValues);
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
 *  Checks of the id and option are valid for this command
 *
 * input:
 *  - id           the id of the command
 *  - option       the option of the command
 *
 * return true if id and option are valid, false otherwise
 */
static bool ATWLAN_isInputValidWlanSet(uint8_t id, uint8_t option)
{
    bool ret = false;

    if(id < ATWLAN_SET_ID_NumberOfValues)
    {
        switch(id)
        {
        case ATWLAN_SET_ID_general:
        {
            /* Only countrycode, sta_tx_power and scan_params are valid */
            ret = (option < ATWLAN_SET_GENERAL_NumberOfValues);
            break;
        }
        case ATWLAN_SET_ID_P2P:
        {
            /* All options valid */
            ret  = (option < ATWLAN_SET_P2P_NumberOfValues);
            break;
        }
        case ATWLAN_SET_ID_AP:
        {
            /* all option but MAX_STA_AGING are valid*/
            ret = (option < ATWLAN_SET_AP_MAX_STA_AGING);
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
 * Checks of the id and option are valid for this command
 *
 * input:
 * -  id       the id of the command
 * -  option   the option of the command
 *
 * return true if id and option are vlaid, false otherwise
 */
static bool ATWLAN_isInputValidWlanPolicySet(uint8_t id, uint8_t option)
{
    bool ret = false;

    if(id < ATWLAN_POLICY_ID_NumberOfValues)
    {
        switch(id)
        {
        case ATWLAN_POLICY_ID_connection:
        {
            /* Only countrycode, sta_tx_power and scan_params are valid */
            ret = (option < ATWLAN_POLICY_CONNECTION_NumberOfValues);
            break;
        }
        case ATWLAN_POLICY_ID_scan:
        {
            /* All options valid */
            ret  = (option < ATWLAN_POLICY_SCAN_NumberOfValues);
            break;
        }
        case ATWLAN_POLICY_ID_PM:
        {
            /* all option but MAX_STA_AGING are valid*/
            ret = (option < ATWLAN_POLICY_PM_NumberOfValues);
            break;
        }

        case ATWLAN_POLICY_ID_P2P:
        {
            /* all option but MAX_STA_AGING are valid*/
            ret = (option < ATWLAN_POLICY_P2P_NumberOfValues);
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
 * -  connectionArgs   the arguments of the command. See ATWLAN_ConnectionArguments_t
 * -  lastDelim        the delimeter after the last argument.
 *
 * output:
 * - pOutString       the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATWLAN_addConnectionArguments(char *pOutString, ATWLAN_ConnectionArguments_t connectionArgs, char lastDelim)
{
    bool ret = false;

    ret = Calypso_appendArgumentString(pOutString, connectionArgs.SSID, ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentString(pOutString, connectionArgs.BSSID, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pOutString, ATWLAN_SecurityTypeStrings[connectionArgs.securityParams.securityType], ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pOutString, connectionArgs.securityParams.securityKey, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pOutString, connectionArgs.securityExtParams.extUser, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pOutString, connectionArgs.securityExtParams.extAnonUser, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pOutString, ATWLAN_SecurityEAPStrings[connectionArgs.securityExtParams.eapMethod], lastDelim);
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
 * - pOutString   the request command string to add the arguments to
 * return true if arguments were added successful, false otherwise
 */
static bool ATWLAN_addArgumentsWlanGet(char *pAtCommand, uint8_t id, uint8_t option)
{
    bool ret = false;


    ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetIDStrings[id], ARGUMENT_DELIM);
    if(ret)
    {
        switch(id)
        {
        case ATWLAN_SET_ID_general:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetGeneralStrings[option], STRING_TERMINATE);

            break;
        }
        case ATWLAN_SET_ID_P2P:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetP2PStrings[option], STRING_TERMINATE);

            break;
        }
        case ATWLAN_SET_ID_AP:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetAPStrings[option], STRING_TERMINATE);

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
 * Adds the arguments to the request command string
 *
 * input:
 * -  id           the id to add
 * -  option       the option to add
 * -  pValues      values to add
 *
 * output:
 * - pOutString   the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATWLAN_addArgumentsWlanSet(char *pAtCommand, uint8_t id, uint8_t option, void *pValues)
{
    bool ret = false;


    ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetIDStrings[id], ARGUMENT_DELIM);
    if(ret)
    {
        switch(id)
        {
        case ATWLAN_SET_ID_general:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetGeneralStrings[option], ARGUMENT_DELIM);
            switch(option)
            {
            case ATWLAN_SET_GENERAL_COUNTRY_CODE:
            {
                ret = Calypso_appendArgumentString(pAtCommand, pValues, STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_GENERAL_STA_TX_POWER:
            case ATWLAN_SET_GENERAL_AP_TX_POWER:
            case ATWLAN_SET_GENERAL_DISABLE_ENT_SERVER_AUTH:
            {
                uint8_t *pIntValues = pValues;
                ret = Calypso_appendArgumentInt(pAtCommand, *pIntValues, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_GENERAL_SCAN_PARAMS:
            {
                ATWLAN_ScanParams_t *scanParams = pValues;

                ret = Calypso_appendArgumentInt(pAtCommand, scanParams->channelMask, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), ARGUMENT_DELIM);
                if(ret)
                {
                    ret = Calypso_appendArgumentInt(pAtCommand, scanParams->rssiTreshold, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);
                }
                break;
            }

            case ATWLAN_SET_GENERAL_SUSPEND_PROFILES:
            {
                uint32_t *mask = pValues;
                ret = Calypso_appendArgumentInt(pAtCommand, *mask, (INTFLAGS_NOTATION_HEX | INTFLAGS_UNSIGNED), STRING_TERMINATE);
                break;
            }

            default:
            {
                ret = false;
                break;
            }

            }

            break;
        }
        case ATWLAN_SET_ID_P2P:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetP2PStrings[option], ARGUMENT_DELIM);
            if(ATWLAN_SET_P2P_CHANNEL_N_REGS == option)
            {
                ATWLAN_P2PChannelNRegs_t *pChannelNRegs = pValues;
                ret = Calypso_appendArgumentInt(pAtCommand, pChannelNRegs->listenChannel, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

                if(ret)
                {
                    ret = Calypso_appendArgumentInt(pAtCommand, pChannelNRegs->listenRegularityClass, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
                }

                if(ret)
                {
                    ret = Calypso_appendArgumentInt(pAtCommand, pChannelNRegs->operatingChannel, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
                }

                if(ret)
                {
                    ret = Calypso_appendArgumentInt(pAtCommand, pChannelNRegs->operatingRegularityClass, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
                }

            }
            else
            {
                ret = false;
            }
            break;
        }
        case ATWLAN_SET_ID_AP:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SetGetAPStrings[option], ARGUMENT_DELIM);
            switch(option)
            {
            case ATWLAN_SET_AP_SSID:
            case ATWLAN_SET_AP_PASSWORD:
            {
                ret = Calypso_appendArgumentString(pAtCommand,pValues, STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_AP_CHANNEL:
            case ATWLAN_SET_AP_HIDDEN_SSID:
            case ATWLAN_SET_AP_MAX_STATIONS:
            {
                uint32_t *pIntValue = pValues;
                ret = Calypso_appendArgumentInt(pAtCommand, *pIntValue, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);

                break;
            }

            case ATWLAN_SET_AP_SECURITY:
            {
                uint8_t *index = pValues;
                ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_SecurityTypeStrings[*index], STRING_TERMINATE);
                break;
            }

            default:
            {
                ret = false;
                break;
            }

            }


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
 *  Adds the arguments to the request command string
 *
 * input:
 * -  id           the id to add
 * -  option       the option to add
 * -  pValues      values to add
 *
 * output;
 * - pOutString   the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATWLAN_addArgumentsWlanPolicySet(char *pAtCommand, uint8_t id, uint8_t option, void *pValues)
{
    bool ret = false;


    ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyStrings[id], ARGUMENT_DELIM);
    if(ret)
    {
        switch(id)
        {
        case ATWLAN_POLICY_ID_connection:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyConnectionStrings[option], STRING_TERMINATE);
            break;
        }
        case ATWLAN_POLICY_ID_scan:
        {
            if(ATWLAN_POLICY_SCAN_Disable_Scan == option)
            {
                ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyScanStrings[option], STRING_TERMINATE);

            }
            else if(ATWLAN_POLICY_SCAN_Hidden_SSID == option)
            {
                ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyScanStrings[option], ARGUMENT_DELIM);
                if(ret)
                {
                    uint8_t *index = pValues;
                    ret = Calypso_appendArgumentInt(pAtCommand, *index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
                }

            }
            else
            {
                ret = false;
            }

            break;
        }
        case ATWLAN_POLICY_ID_PM:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyPMStrings[option], ARGUMENT_DELIM);
            if(ret)
            {
                uint8_t *pIntValue = pValues;
                ret = Calypso_appendArgumentInt(pAtCommand, *pIntValue, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
            }

            break;
        }

        case ATWLAN_POLICY_ID_P2P:
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyP2PStrings[option], ARGUMENT_DELIM);
            if(ret)
            {
                uint8_t *pIndex = pValues;
                ret = Calypso_appendArgumentString(pAtCommand, ATWLAN_PolicyP2PValueStrings[*pIndex], STRING_TERMINATE);
            }

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
    }
    return ret;
}

/**
 * Parses the response of a AT+wlanScan command
 *
 * input:
 * -      pAtCommand      the string reponse by the module
 *
 * output:
 * -     pOutScanEntry   the parsed response. See ATWLAN_ScanEntry_t
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATWLAN_parseResponseWlanScanEntry(char **pAtCommand, ATWLAN_ScanEntry_t *pOutScanEntry)
{
    bool ret = false;


    if((NULL == pAtCommand) || (NULL == pOutScanEntry))
    {
        return false;
    }

    const char *expectedCmd = "+wlanscan:";
    const size_t cmdLength = strlen(expectedCmd);
    char tempString[32];

    /* check if reponse is for get*/
    ret = (0 == strncmp(*pAtCommand, expectedCmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        ret = Calypso_getNextArgumentString(pAtCommand, &(pOutScanEntry->SSID)[0], ARGUMENT_DELIM);

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, &(pOutScanEntry->BSSID)[0], ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentInt(pAtCommand, &(pOutScanEntry->RSSI), INTFLAGS_SIZE8, ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentInt(pAtCommand, &(pOutScanEntry->channel), INTFLAGS_SIZE8, ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, tempString, ARGUMENT_DELIM);
            ATWLAN_parseSecurityType(tempString, &(pOutScanEntry->securityType));
        }


        if(ret)
        {
            ret = Calypso_getNextArgumentInt(pAtCommand, &(pOutScanEntry->hiddenSsidEnabled), INTFLAGS_SIZE8, ARGUMENT_DELIM);
        }
        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, tempString, ARGUMENT_DELIM);
            ATWLAN_parseScanCipher(tempString, &(pOutScanEntry->cypher));
        }


        ret = Calypso_getNextArgumentString(pAtCommand, tempString, STRING_TERMINATE);
        ATWLAN_parseScanKeyMngmt(tempString,&(pOutScanEntry->keyManagmentMethod));


    }

    return ret;
}


/**
 *  Parses the response of a AT+wlanScan command
 *
 * input:
 * -      pAtCommand      the string reponse by the module
 *
 * output:
 * -     pOutScanEntry   the parsed response. See ATWLAN_ScanEntry_t
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATWLAN_parseResponseWlanAddProfile(char **pAtCommand, uint8_t *pOutIndex)
{
    bool ret = false;
    const char *cmd = "+wlanprofileadd:";
    const size_t cmdLength = strlen(cmd);

    /* check if reponse is for get*/
    ret = (0 == strncmp(*pAtCommand, cmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        ret = Calypso_getNextArgumentInt(pAtCommand, pOutIndex, INTFLAGS_SIZE8, STRING_TERMINATE);
    }

    return ret;
}

/**
 * Parses the response of a AT+wlanProfileGet command
 *
 * input:
 * -   pAtCommand    the string reponse by the module.
 *
 * output:
 * -     pOutProfile   the parsed response. See ATWLAN_Profile_t
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATWLAN_parseResponseWlanGetProfile(char **pAtCommand, ATWLAN_Profile_t *pOutProfile)
{
    bool ret = false;

    const char *expectedCmd = "+wlanprofileget:";
    const size_t cmdLength = strlen(expectedCmd);

    char tempString[32];

    /* check if reponse is for profileGet*/
    ret = (0 == strncmp(*pAtCommand, expectedCmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        ret = Calypso_getNextArgumentString(pAtCommand, &(pOutProfile->SSID)[0], ARGUMENT_DELIM);

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, &(pOutProfile->BSSID)[0], ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, tempString, ARGUMENT_DELIM);
            ATWLAN_parseSecurityType(tempString, &(pOutProfile->securityParams.securityType));
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, pOutProfile->securityParams.securityKey, ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, pOutProfile->securityExtParams.extUser, ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, pOutProfile->securityExtParams.extAnonUser, ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, tempString, ARGUMENT_DELIM);
            ATWLAN_parseSecurityEAP(tempString, &(pOutProfile->securityExtParams.eapMethod));
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentInt(pAtCommand, &(pOutProfile->priority), INTFLAGS_SIZE32, STRING_TERMINATE);
        }
    }
    return ret;
}

/**
 * Parses the response of a AT+wlanGet command
 *
 * input:
 * -   id          id to get
 * -   option      option to get
 * -   pAtCommand  the string reponse by the module.
 *
 * output;
 * -  pValues     the parsed response. Depends on the id/option
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATWLAN_parseResponseWlanGet(uint8_t id, uint8_t option, char **pAtCommand, void *pValues)
{
    bool ret = false;

    const char *expectedCmd = "+wlanget:";
    const size_t cmdLength = strlen(expectedCmd);

    char tempString[32];

    /* check if reponse is for profileGet*/
    ret = (0 == strncmp(*pAtCommand, expectedCmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        switch(id)
        {
        case ATWLAN_SET_ID_general:
        {
            switch(option)
            {
            case ATWLAN_SET_GENERAL_COUNTRY_CODE:
            {
                ret = Calypso_getNextArgumentString(pAtCommand, pValues, STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_GENERAL_STA_TX_POWER:
            {
                ret = Calypso_getNextArgumentInt(pAtCommand, pValues, INTFLAGS_SIZE8, STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_GENERAL_SCAN_PARAMS:
            {
                ATWLAN_ScanParams_t *pScanParams = pValues;
                ret = Calypso_getNextArgumentInt(pAtCommand, &(pScanParams->channelMask), (INTFLAGS_SIZE32 | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

                if(ret)
                {
                    ret = Calypso_getNextArgumentInt(pAtCommand, &(pScanParams->rssiTreshold), (INTFLAGS_SIZE32 | INTFLAGS_SIGNED), STRING_TERMINATE);
                }
                break;
            }
            default:
            {
                ret = false;
            }
            }

            break;
        }
        case ATWLAN_SET_ID_P2P:
        {
            if(ATWLAN_SET_P2P_CHANNEL_N_REGS == option)
            {
                ATWLAN_P2PChannelNRegs_t *pChannelNRegs = pValues;

                ret = Calypso_getNextArgumentInt(pAtCommand, &(pChannelNRegs->listenChannel), (INTFLAGS_SIZE8 | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

                if(ret)
                {
                    ret = Calypso_getNextArgumentInt(pAtCommand, &(pChannelNRegs->listenRegularityClass), (INTFLAGS_SIZE8 | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentInt(pAtCommand, &(pChannelNRegs->operatingChannel), (INTFLAGS_SIZE8 | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
                }

                if(ret)
                {
                    ret = Calypso_getNextArgumentInt(pAtCommand, &(pChannelNRegs->operatingRegularityClass), (INTFLAGS_SIZE8 | INTFLAGS_UNSIGNED), STRING_TERMINATE);
                }

            }
            else
            {
                ret = false;
            }
            break;
        }
        case ATWLAN_SET_ID_AP:
        {
            switch(option)
            {
            case ATWLAN_SET_AP_SSID:
            case ATWLAN_SET_AP_PASSWORD:
            {
                ret = Calypso_getNextArgumentString(pAtCommand, pValues, STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_AP_CHANNEL:
            case ATWLAN_SET_AP_HIDDEN_SSID:
            case ATWLAN_SET_AP_MAX_STATIONS:
            case ATWLAN_SET_AP_MAX_STA_AGING:
            {
                ret = Calypso_getNextArgumentInt(pAtCommand, pValues, (INTFLAGS_SIZE8 | INTFLAGS_UNSIGNED), STRING_TERMINATE);
                break;
            }

            case ATWLAN_SET_AP_SECURITY:
            {
                ret = Calypso_getNextArgumentString(pAtCommand, tempString, STRING_TERMINATE);
                if(ret)
                {
                    ret = ATWLAN_parseSecurityType(tempString, pValues);
                }

                break;
            }

            default:
            {
                ret = false;
                break;
            }

            }

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
 * Parses the response of a AT+wlanPolicyGet command
 *
 * input:
 * -   type        type to get
 * -   pAtCommand  the string reponse by the module.
 *
 * output:
 * -  pOption     if type has an option, will be returned
 * -  pValues     the parsed value. Depends on the type
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATWLAN_parseResponseWlanPolicyGet(uint8_t type, char **pAtCommand, uint8_t *pOption, void *pValues)
{
    bool ret = false;

    const char *expectedCmd = "+wlanpolicyget:";
    const size_t cmdLength = strlen(expectedCmd);

    /* check if reponse is for profileGet*/
    ret = (0 == strncmp(*pAtCommand, expectedCmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;

        char temp[12];

        ret = Calypso_getNextArgumentString(pAtCommand, temp, ARGUMENT_DELIM);

        if(ret)
        {
            ret = ATWLAN_parsePolicyOption(type, temp, pOption);
        }

        if(ret)
        {
            if(ATWLAN_POLICY_ID_P2P == type)
            {
                ret = Calypso_getNextArgumentString(pAtCommand, pValues, STRING_TERMINATE);
            }
            else
            {
                ret = Calypso_getNextArgumentInt(pAtCommand, pValues, INTFLAGS_SIZE8, STRING_TERMINATE);
            }
        }
    }
    return ret;
}


/**
 * Parses a string to ATWLAN_SetMode_t
 *
 * input:
 * -   setModeString   string representing the mode
 *
 * output:
 * -  pOutSetMode     ATWLAN_SetMode_t representing the mode
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parseSetMode(const char *setModeString, ATWLAN_SetMode_t *pOutSetMode)
{
    for(int i = 0; i < ATWLAN_SET_MODE_NumberOfValues; i++)
    {
        if(0 == strcasecmp(setModeString, ATWLAN_SetModeStrings[i]))
        {
            *pOutSetMode = (ATWLAN_SetMode_t)i;
            return true;
        }
    }

    return false;
}

/**
 * Parses a string to ATWLAN_SecurityType_t
 *
 * input:
 * -   securityTypeString   string representing the security type
 *
 * output:
 * -  pOutSecurityType     ATWLAN_SecurityType_t representing the security type
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parseSecurityType(const char *securityTypeString, ATWLAN_SecurityType_t *pOutSecurityType)
{
    for(int i = 0; i < ATWLAN_SECURITY_TYPE_NumberOfValues; i++)
    {
        if(0 == strcasecmp(securityTypeString, ATWLAN_SecurityTypeStrings[i]))
        {
            *pOutSecurityType = (ATWLAN_SecurityType_t)i;
            return true;
        }
    }

    return false;
}

/**
 * Parses a string to ATWLAN_ScanSecurityType_t
 *
 * input:
 * -   scanSecurityTypeString   string representing the security type of scan
 *
 *output:
 * -  pOutScanSecurityType     ATWLAN_ScanSecurityType_t representing the security type of scan
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parseScanSecurityType(const char *scanSecurityTypeString, ATWLAN_ScanSecurityType_t *pOutScanSecurityType)
{
    for(int i = 0; i < ATWLAN_SCAN_SECURITY_TYPE_NumberOfValues; i++)
    {
        if(0 == strcasecmp(scanSecurityTypeString, ATWLAN_ScanSecurityTypeStrings[i]))
        {
            *pOutScanSecurityType = (ATWLAN_ScanSecurityType_t)i;
            return true;
        }
    }

    return false;
}

/**
 * Parses a string to ATWLAN_ScanCipher_t
 *
 * input:
 * -   scanCipherString   string representing the scan cipher
 *
 * output:
 * -  pOutScanCipher     ATWLAN_ScanCipher_t representing the scan cipher
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parseScanCipher(const char *scanCipherString, ATWLAN_ScanCipher_t *pOutScanCipher)
{
    for(int i = 0; i < ATWLAN_SECURITY_EAP_NumberOfValues; i++)
    {
        if(0 == strcasecmp(scanCipherString, ATWLAN_ScanCipherStrings[i]))
        {
            *pOutScanCipher = (ATWLAN_ScanCipher_t)i;
            return true;
        }
    }

    return false;
}

/**
 * Parses a string to ATWLAN_ScanKeyMgmnt_t
 *
* input:
 * -   scanKeyMgmntString   string representing the key managment
 *
 *output:
 * -  pOutScanKeyMgmnt     ATWLAN_ScanKeyMgmnt_t representing the key managment
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parseScanKeyMngmt(const char *scanKeyMgmntString, ATWLAN_ScanKeyMgmnt_t *pOutScanKeyMgmnt)
{
    for(int i = 0; i < ATWLAN_SECURITY_EAP_NumberOfValues; i++)
    {
        if(0 == strcasecmp(scanKeyMgmntString, ATWLAN_ScanKeyMgmntStrings[i]))
        {
            *pOutScanKeyMgmnt = (ATWLAN_ScanKeyMgmnt_t)i;
            return true;
        }
    }

    return false;
}

/**
 *  Parses a string to ATWLAN_Security_EAP_t
 *
 * input:
 * -   securityEAPString   string representing the security extensible authentication protocol
 *
 * output:
 * -  pOutSecurityEAP     ATWLAN_Security_EAP_t representing the security extensible authentication protocol
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parseSecurityEAP(const char *securityEAPString, ATWLAN_Security_EAP_t *pOutSecurityEAP)
{
    for(int i = 0; i < ATWLAN_SECURITY_EAP_NumberOfValues; i++)
    {
        if(0 == strcasecmp(securityEAPString, ATWLAN_SecurityEAPStrings[i]))
        {
            *pOutSecurityEAP = (ATWLAN_Security_EAP_t)i;
            return true;
        }
    }

    return false;
}

/**
 *  Parses a string of option to the respective struct type
 *
 * input:
 * -   policyId            id
 * -   policyOptionString  string of returned option
 *
 * output:
 * -  pOutPolicyOption    integer representing the mode. Can be casted to respective struct type
 *
 * return true if parsed succesful, false otherwise
 */
static bool ATWLAN_parsePolicyOption(uint8_t policyId, const char *policyOptionString, uint8_t *pOutPolicyOption)
{
    switch(policyId)
    {
    case ATWLAN_POLICY_ID_connection:
    {
        for(int i = 0; i < ATWLAN_POLICY_CONNECTION_NumberOfValues; i++)
        {
            if(0 == strcasecmp(policyOptionString, ATWLAN_PolicyConnectionStrings[i]))
            {
                *pOutPolicyOption = i;
                return true;
            }
        }
        break;
    }
    case ATWLAN_POLICY_ID_scan:
    {
        for(int i = 0; i < ATWLAN_POLICY_SCAN_NumberOfValues; i++)
        {
            if(0 == strcasecmp(policyOptionString, ATWLAN_PolicyScanStrings[i]))
            {
                *pOutPolicyOption = i;
                return true;
            }
        }
        break;
    }
    case ATWLAN_POLICY_ID_PM:
    {
        for(int i = 0; i < ATWLAN_POLICY_PM_NumberOfValues; i++)
        {
            if(0 == strcasecmp(policyOptionString, ATWLAN_PolicyPMStrings[i]))
            {
                *pOutPolicyOption = i;
                return true;
            }
        }
        break;
    }
    case ATWLAN_POLICY_ID_P2P:
    {
        for(int i = 0; i < ATWLAN_POLICY_P2P_NumberOfValues; i++)
        {
            if(0 == strcasecmp(policyOptionString, ATWLAN_PolicyP2PStrings[i]))
            {
                *pOutPolicyOption = i;
                return true;
            }
        }

    }
    default:
    {
        break;
    }
    }

    return false;
}

/*
* Static Functions.
* ########################## */

/*
* Definitions.
* ######################################################################### */
