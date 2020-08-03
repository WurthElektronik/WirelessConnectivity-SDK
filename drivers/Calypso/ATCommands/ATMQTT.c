/* ###########################################################################
* Includes:
*/

#include "ATMQTT.h"
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

#define ATMQTT_MAX_BUFFER_SIZE 2048

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

static const char *ATMQTT_SecurityMethodsStrings[] =
{
    ATMQTT_SECURITY_METHOD(GENERATE_STRING)
};

static const char *ATMQTT_CipherStrings[] =
{
    ATMQTT_CIPHER(GENERATE_STRING)
};

static const char *ATMQTT_ProtocolStrings[] =
{
    ATMQTT_PROTOCOL(GENERATE_STRING)
};

static const char *ATMQTT_QoSStrings[] =
{
    ATMQTT_QOS(GENERATE_STRING)
};

static const char *ATMQTT_SetOptionStrings[] =
{
    ATMQTT_SET_OPTION(GENERATE_STRING)
};

static const char *ATMQTT_CreateFlagsStrings[] =
{
    "ip4", "ip6", "url", "sec", "skip_domain_verify", "skip_cert_verify", "skip_date_verify"
};

char responseBuffer[ATMQTT_MAX_BUFFER_SIZE];
char requestBuffer[ATMQTT_MAX_BUFFER_SIZE];

char *pRequestCommand;
char *pRespondCommand;


/*
* Static Globals.
* ########################## */

/* #############################
* Static Functions:
*/

static bool ATMQTT_addArgumentsCreate(char *pAtCommand, char *clientID, uint32_t flags, ATMQTT_ServerInfo_t serverInfo, ATMQTT_securityParams_t securityParams, ATMQTT_connectionParams_t connectionParams);
static bool ATMQTT_addArgumentsPublish(char *pAtCommand, uint8_t index, char *topicString, ATMQTT_QoS_t QoS, uint8_t retain, uint16_t messageLength, char *pMessage);
static bool ATMQTT_addArgumentsSubscribe(char *pAtCommand, uint8_t index, uint8_t numOfTopics, ATMQTT_subscribeTopic_t *pTopics);
static bool ATMQTT_addArgumentsUnsubscribe(char *pAtCommand, uint8_t index, uint8_t numOfTopics, char *topic1, char *topic2, char *topic3, char *topic4);
static bool ATMQTT_addArgumentsSet(char *pAtCommand, uint8_t index, uint8_t option, void *pValues);
static bool ATMQTT_parseResponseCreate(char **pAtCommand, uint8_t *pOutIndex);
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
 *Sends the AT+MQTTCreate=[clientID],[flags],[server address],[server port],[security Method],[cipher],[private key],[CA],[DHkey],[protocol],[blocking send],[data format] command
 *
 *input:
 * -clientID:           client ID of the new
 * -flags:
 * -serverInfo:         server address and server ip. See ATMQTT_ServerInfo_t.
 * -securityParams:     security parameters. See securityMethod.
 * -connectionParams:   connection parameters. See ATMQTT_connectionParams_t.
 *
 *output:
 * -pIndex:             Index of the created mqtt client. Used in other functions.
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_create(char *clientID, uint32_t flags, ATMQTT_ServerInfo_t serverInfo, ATMQTT_securityParams_t securityParams, ATMQTT_connectionParams_t connectionParams, uint8_t *pIndex)
{
    bool ret = false;

    pRespondCommand = &responseBuffer[0];

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttCreate=");

    ret = ATMQTT_addArgumentsCreate(pRequestCommand, clientID, flags, serverInfo, securityParams, connectionParams);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
    }

    if(ret)
    {
        ret = ATMQTT_parseResponseCreate(&pRespondCommand, pIndex);
    }

    return ret;
}
/**
 *Sends the AT+MQTTDelete=[index] command
 *
 *input:
 * -index   index of mqttclient to delete
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_delete(uint8_t index)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttDelete=");


    ret = Calypso_appendArgumentInt(pRequestCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
    if(ret)
    {
        ret = Calypso_appendArgumentString(pRequestCommand, CRLF, STRING_TERMINATE);
    }

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+MQTTConnect=[index] command
 *
 *input:
 * -index   index of mqttclient to connect to server. Server data is set with AT+MQTTCreate command
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_connect(uint8_t index)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttConnect=");


    ret = Calypso_appendArgumentInt(pRequestCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
    if(ret)
    {
        ret = Calypso_appendArgumentString(pRequestCommand, CRLF, STRING_TERMINATE);
    }

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+MQTTDisconnect=[index] command
 *
 *input:
 * -index   index of mqttclient to disconnect from server.
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_disconnect(uint8_t index)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttDisconnet=");


    ret = Calypso_appendArgumentInt(pRequestCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
    if(ret)
    {
        ret = Calypso_appendArgumentString(pRequestCommand, CRLF, STRING_TERMINATE);
    }

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+MQTTpublish=[index],[topic],[QOS],[retain],[messageLength],[message] command
 *
 *input:
 * -index           index of mqttclient to to publish.
 * -topicString     topic to publish to
 * -retain          retain the message(0) or not (1)
 * -messageLength   length of the message
 * -pMessage         message to publish
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_publish(uint8_t index, char *topicString, ATMQTT_QoS_t QoS, uint8_t retain, uint16_t messageLength, char *pMessage)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttPublish=");

    ret = ATMQTT_addArgumentsPublish(pRequestCommand, index, topicString, QoS, retain, messageLength, pMessage);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+MQTTpublish=[index],[numOfTopics],[topic1],[QoSn],[reserved]... command
 *
 *input:
 * -index           index of mqttclient to to subscribe.
 * -numOfTopics     number of topics to which subscribe to
 * -pTopics         Topics to subscribe to. See ATMQTT_subscribeTopic_t
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_subscribe(uint8_t index, uint8_t numOfTopics, ATMQTT_subscribeTopic_t *pTopics)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttSubscribe=");

    ret = ATMQTT_addArgumentsSubscribe(pRequestCommand, index, numOfTopics, pTopics);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+MQTTpublish=[index],[numOfTopics],[topic1],[reserved]... command
 *
 *input:
 * -index           index of mqttclient to to unsibscribe.
 * -numOfTopics     number of topics from which to unsubscribe
 * -topic1-topic4   Topics from which to unsubscribe.
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATMQTT_unsubscribe(uint8_t index, uint8_t numOfTopics, char *topic1, char *topic2, char *topic3, char *topic4)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttUnsubscribe=");

    ret = ATMQTT_addArgumentsUnsubscribe(pRequestCommand, index, numOfTopics, topic1, topic2, topic3, topic4);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+MQTTset=[index],[option],[pValues]... command
 *
 *input:
 * -index     index of mqttclient to set options
 * -option    option to set
 * -pValues   values to set
 *
 *return true  if succeeded
 *       false otherwise

 * CAUTION! Only use this command if firmware version of the Calypso module is 1.1.0 or later
 */
bool ATMQTT_set(uint8_t index, uint8_t option, void *pValues)
{

    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATMQTT_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+mqttSet=");

    ret = ATMQTT_addArgumentsSet(pRequestCommand, index, option, pValues);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;

}
/*
*/

/*
* Exported Functions.
* ########################## */

/* #############################
* Static Functions:
*/

/**
 *Adds the arguments to the request command string
 *
 *input:
 * -pAtCommand  the request command string to add the arguments to
 * -clientID:           client ID of the new
 * -flags:
 * -serverInfo:         server address and server ip. See ATMQTT_ServerInfo_t.
 * -securityParams:     security parameters. See securityMethod.
 * -connectionParams:   connection parameters. See ATMQTT_connectionParams_t.
*/
static bool ATMQTT_addArgumentsCreate(char *pAtCommand, char *clientID, uint32_t flags, ATMQTT_ServerInfo_t serverInfo, ATMQTT_securityParams_t securityParams, ATMQTT_connectionParams_t connectionParams)
{

    bool ret = false;

    ret = Calypso_appendArgumentString(pAtCommand, clientID, ARGUMENT_DELIM);

    if(ret)
    {
        char optionString[128];
        memset(optionString,0,128);

        uint32_t bitmask;
        for(int i = 0; i < ATMQTT_CREATE_FLAGS_NUM_OF_BITS; i++)
        {
            bitmask = (1 << i);
            if(0 != (bitmask & flags))
            {
                Calypso_appendArgumentString(optionString, ATMQTT_CreateFlagsStrings[i],BITMASK_DELIM);
            }
        }
        /* delete last '|' */
        optionString[strlen(optionString)-1] = '\0';
        ret = Calypso_appendArgumentString(pAtCommand, optionString, ARGUMENT_DELIM);
    }


    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, serverInfo.address, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, serverInfo.port, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATMQTT_SecurityMethodsStrings[securityParams.securityMethod], ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATMQTT_CipherStrings[securityParams.cipher], ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, securityParams.privateKeyFile, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, securityParams.certificateFile, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, securityParams.CAFile, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, securityParams.DHKey, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATMQTT_ProtocolStrings[connectionParams.protocolVersion], ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, connectionParams.blockingSend, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, connectionParams.format, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF, STRING_TERMINATE);
    }

    return ret;
}

/**
 *Adds the arguments to the request command string
 *
 *input:
 * -pAtCommand  the request command string to add the arguments to
 * -index           index of mqttclient to to publish.
 * -topicString     topic to publish to
 * -retain          retain the message(0) or not (1)
 * -messageLength   length of the message
 * -pMessage         message to publish
*/
static bool ATMQTT_addArgumentsPublish(char *pAtCommand, uint8_t index, char *topicString, ATMQTT_QoS_t QoS, uint8_t retain, uint16_t messageLength, char *pMessage)
{
    bool ret = false;

    ret = Calypso_appendArgumentInt(pAtCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, topicString, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATMQTT_QoSStrings[QoS], ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, retain, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, messageLength, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, pMessage, STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF, STRING_TERMINATE);
    }

    return ret;
}

/**
 *Adds the arguments to the request command string
 *
 *input:
 * -pAtCommand  the request command string to add the arguments to
 * -index           index of mqttclient to to subscribe.
 * -numOfTopics     number of topics to which subscribe to
 * -pTopics         Topics to subscribe to. See ATMQTT_subscribeTopic_t
*/
static bool ATMQTT_addArgumentsSubscribe(char *pAtCommand, uint8_t index, uint8_t numOfTopics, ATMQTT_subscribeTopic_t *pTopics)
{
    bool ret = false;

    ret = Calypso_appendArgumentInt(pAtCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);


    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, numOfTopics, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    for(int i = 0; i < numOfTopics; i++)
    {
        if(ret)
        {
            Calypso_appendArgumentString(pAtCommand, pTopics[i].topicString, ARGUMENT_DELIM);
        }

        if(ret)
        {
            Calypso_appendArgumentString(pAtCommand, ATMQTT_QoSStrings[pTopics[i].QoS], ARGUMENT_DELIM);
        }

        if(ret)
        {
            Calypso_appendArgumentString(pAtCommand, STRING_EMPTY, ARGUMENT_DELIM);
        }
    }

    /* add empty, unused topics*/
    for(int i = numOfTopics; i < 5;  i++)
    {
        if(ret)
        {
           Calypso_appendArgumentString(pAtCommand, ",,", ARGUMENT_DELIM);
        }
    }

    pAtCommand[strlen(pAtCommand)] = STRING_TERMINATE;

    if(ret)
    {
        Calypso_appendArgumentString(pAtCommand, CRLF, STRING_TERMINATE);
    }

    return ret;
}

/**
 *Adds the arguments to the request command string
 *
 *input:
 * -pAtCommand  the request command string to add the arguments to
 * -index           index of mqttclient to to unsibscribe.
 * -numOfTopics     number of topics from which to unsubscribe
 * -topic1-topic4   Topics from which to unsubscribe.
*/
static bool ATMQTT_addArgumentsUnsubscribe(char *pAtCommand, uint8_t index, uint8_t numOfTopics, char *topic1, char *topic2, char *topic3, char *topic4)
{
    bool ret = false;

    ret = Calypso_appendArgumentInt(pAtCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);


    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, numOfTopics, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }


    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, topic1, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, STRING_EMPTY, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, topic2, ARGUMENT_DELIM);
    }

        if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, STRING_EMPTY, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, topic3, ARGUMENT_DELIM);
    }

        if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, STRING_EMPTY, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, topic4, ARGUMENT_DELIM);
    }

        if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, STRING_EMPTY, STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF, STRING_TERMINATE);
    }
    return ret;
}

/**
 *Adds the arguments to the request command string
 *
 *input:
 * -index     index of mqttclient to set options
 * -option    option to set
 * -pValues   values to set
*/
static bool ATMQTT_addArgumentsSet(char *pAtCommand, uint8_t index, uint8_t option, void *pValues)
{
    bool ret = false;

    ret = Calypso_appendArgumentInt(pAtCommand, index, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret && (option < ATMQTT_SET_OPTION_NumberOfValues))
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATMQTT_SetOptionStrings[option], ARGUMENT_DELIM);
    }
    else
    {
        ret = false;
    }


    if(ret)
    {
        switch(option)
        {
        case ATMQTT_SET_OPTION_user:
        case ATMQTT_SET_OPTION_password:
        {
            ret = Calypso_appendArgumentString(pAtCommand, pValues, STRING_TERMINATE);
            break;
        }

        case ATMQTT_SET_OPTION_will:
        {
            ATMQTT_setWillParams_t *pWillValues = pValues;

            ret = Calypso_appendArgumentString(pAtCommand, pWillValues->topic, ARGUMENT_DELIM);

            if(ret)
            {
                ret = Calypso_appendArgumentString(pAtCommand, ATMQTT_QoSStrings[pWillValues->QoS], ARGUMENT_DELIM);
            }

            if(ret)
            {
                ret = Calypso_appendArgumentInt(pAtCommand, pWillValues->retain, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
            }

            if(ret)
            {
                ret = Calypso_appendArgumentInt(pAtCommand, pWillValues->messageLength, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED),ARGUMENT_DELIM);
            }

            if(ret)
            {
                ret = Calypso_appendArgumentString(pAtCommand, pWillValues->message, STRING_TERMINATE);
            }

            break;
        }

        case ATMQTT_SET_OPTION_keepAlive:
        {
            uint16_t *pKeepAliveValue = pValues;
            ret = Calypso_appendArgumentInt(pAtCommand, *pKeepAliveValue, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
            break;
        }

        case ATMQTT_SET_OPTION_clean:
        {
            uint8_t *pCleanValue = pValues;
            ret = Calypso_appendArgumentInt(pAtCommand, *pCleanValue, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);

            break;
        }

        default:
        {
            ret = false;
        }

        }
    }

    return ret;
}

/**
 *Adds the arguments to the request command string
 *
 *input:
 * -pAtCommand  the request command string to add the arguments to
 *
 *output:
 * -pOutIndex   Index of the MQTT client. Will be used in other MQTT functions.
*/
static bool ATMQTT_parseResponseCreate(char **pAtCommand, uint8_t *pOutIndex)
{
    bool ret = false;
    const char *cmd = "+mqttcreate:";
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


/*
* Static Functions.
* ########################## */

/*
* Definitions.
* ######################################################################### */
