/* ###########################################################################
* Includes:
*/

#include "ATSocket.h"
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
#define ATSOCKET_MAX_BUFFER_SIZE 2048
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

static const char *ATSocketFamilyString[] =
{
    "INET",
    "INET6",
};

static const char *ATSocketTypeString[] =
{
    "STREAM",
    "DGRAM",
};

static const char *ATSocketProcotolString[] =
{
    "TCP",
    "UDP",
    "SEC",
};


static const char *ATSocketSockOptString[] =
{
    "SOCKET",
    "IP",
};

static const char *ATSocketSockOptSocketString[] =
{
    "KEEPALIVE",
    "KEEPALIVETIME",
    "RX_NO_IP_BOUNDARY",
    "RCVTIMEO",
    "RCVBUF",
    "NONBLOCKING",
    "SECMETHOD",
    "SECURE_MASK",
    "SECURE_FILE_CA_FILE_NAME",
    "SECURE_FILES_PRIVATE_KEY_FILE_NAME",
    "SECURE_FILES_CERTIFICATE_FILE_NAME",
    "SECURE_FILES_DH_KEY_FILE_NAME",
    "SECURE_DOMAIN_NAME_VERIFICATION",
};

static const char *ATSocketSockOptIDString[] =
{
    "MULTICAST_TTL",
    "ADD_MEMBERSHIP",
    "DROP_MEMBERSHIP",
};

static const char *AtSocketSockOptSecMethod[] =
{
    "SSLV3",
    "TLSV1",
    "TLSV1_1",
    "TLSV1_2",
    "SSLV3_TLSV1_2",
};


char responseBuffer[ATSOCKET_MAX_BUFFER_SIZE];
char requestBuffer[ATSOCKET_MAX_BUFFER_SIZE];

char *pRequestCommand;
char *pRespondCommand;
/*
* Static Globals.
* ########################## */

/* #############################
* Static Functions:
*/

static bool ATSocket_addArgumentsFromSocketDescriptor(char *pAtCommand, ATSocket_Descriptor_t socket, char lastDelim);
static bool ATSocket_addArgumentsCreate(char *pAtCommand, ATSocket_Family_t family, ATSocket_Type_t type, ATSocket_Protocol_t protocol);
static bool ATSocket_addArgumentsBindConnect(char *pAtCommand, uint8_t socketID, ATSocket_Descriptor_t socket);
static bool ATSocket_addArgumentsRecv(char *pAtCommand, uint8_t socketID, Calypso_DataFormat_t format, uint16_t length);
static bool ATSocket_addArgumentsRecvFrom(char *pAtCommand, uint8_t socketID, ATSocket_Descriptor_t socketFrom, Calypso_DataFormat_t format, uint16_t length);
static bool ATSocket_addArgumentsSend(char *pAtCommand, uint8_t socketID, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData);
static bool ATSocket_addArgumentsSendTo(char *pAtCommand, uint8_t socketID, ATSocket_Descriptor_t socketTo, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData);
static bool ATSocket_addArgumentsSetSockOpt(char *pAtCommand, uint8_t socketID, uint8_t level, uint8_t option, void *pData);


static bool ATSocket_parseResponseCreate(char **pAtCommand, uint8_t *pOutSocketID);

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
 * Sends the AT+Socket=[family],[type],[protocol] command
 *
 * input:
 * - family        Family of the socket. See ATSocket_Family_t
 * - type          Type of the socket. See ATSocket_Type_t
 * - protocol      Protocol of the socket. See ATSocket_Protocol_t
 * output:
 * - pOutSocketID  Id of the socket within the module. Can be used to access socket from other functions
 *
 * return true if command was sent successful, false otherwise
 */

bool ATSocket_create(ATSocket_Family_t family, ATSocket_Type_t type, ATSocket_Protocol_t protocol, uint8_t *pOutSocketID)
{
    bool ret = false;


    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+socket=");

    pRespondCommand = &responseBuffer[0];

    ret = ATSocket_addArgumentsCreate(pRequestCommand, family, type, protocol);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
    }

    if(ret)
    {
        ret = ATSocket_parseResponseCreate(&pRespondCommand, pOutSocketID);
    }

    return ret;
}

/**
 *  Sends the AT+close=[socketID] command
 *
 * input:
 * - socketID  Id of the socket to be closed
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_close(uint8_t socketID)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+close=");

    ret = Calypso_appendArgumentInt(pRequestCommand,socketID,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);

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
 *  Sends the AT+bind=[socketID],[family], [localPort], [localAddress] command
 *
 * input:
 *  - socketID  Id of the socket to be bind
 *  - socket    Struct contains family, localPort and localAddress. See ATSocket_Descriptor_t
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_bind(uint8_t socketID, ATSocket_Descriptor_t socket)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+bind=");


    ret = ATSocket_addArgumentsBindConnect(pRequestCommand, socketID, socket);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 *  Sends the AT+listen=[socketID], [backlog] command
 *
 * input:
 *  - socketID  Id of the socket to listen
 *  - socket    Struct contains family, localPort and localAddress. See ATSocket_Descriptor_t
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_listen(uint8_t socketID, uint16_t backlog)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+listen=");


    ret = Calypso_appendArgumentInt(pRequestCommand,socketID,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pRequestCommand,backlog,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
    }

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
 *  Sends the AT+connect=[socketID],[family], [remotePort], [remoteAddress] command
 *
 * input:
 *  - socketID  Id of the socket to establish connection
 *  - socket    Struct contains family, remotePort and remoteAddress. See ATSocket_Descriptor_t
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_connect(uint8_t socketID, ATSocket_Descriptor_t socket)
{
    bool ret = false;


    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+connect=");


    ret = ATSocket_addArgumentsBindConnect(pRequestCommand, socketID, socket);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 *  Sends the AT+accept=[socketID],[family] command
 *
 * input:
 *  - socketID   Id of the socket to accept connections
 *  - family     family if the socket to accept
 *
 *  return true if command was sent successful, false otherwise
 */
bool ATSocket_accept(uint8_t socketID, ATSocket_Family_t family)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+accept=");

    ret = Calypso_appendArgumentInt(pRequestCommand,socketID,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentString(pRequestCommand, ATSocketFamilyString[family], STRING_TERMINATE);
    }

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
 * Sends the AT+recv=[socketID],[format], [length] command
 *
 * input:
 *  - socketID     Id of the socket which should receive
 *  - format       format in which the data is to be output. The module will encode the data and will ouput the data in this format.
 *  - length       max number of bytes to receive
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_recv(uint8_t socketID, Calypso_DataFormat_t format, uint16_t length)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+recv=");

    ret = ATSocket_addArgumentsRecv(pRequestCommand, socketID, format, length);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 *  Sends the AT+recvFrom=[socketID],[family],[remotePort],[remoteAddress],[format],[length] command
 *
 * input:
 * -  socketID     Id of the socket which should receive
 * -  socket       socket from which should be received
 * -  format       format in which the data is to be output. The module will encode the data and will ouput the data in this format.
 * -  length       max number of bytes to receive
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_recvFrom(uint8_t socketID, ATSocket_Descriptor_t socket, Calypso_DataFormat_t format, uint16_t length)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+recvFrom=");

    ret = ATSocket_addArgumentsRecvFrom(pRequestCommand, socketID, socket, format, length);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *  Sends the AT+send=[socketID],[format],[length],[data] command
 *
 * input:
 * -  socketID     Id of the socket which should receive
 * -  format       format in which the data should be present
 * -  length       bytes to send
 * -  pData        data to send
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_send(uint8_t socketID, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+send=");

    ret = ATSocket_addArgumentsSend(pRequestCommand, socketID, format, encodeToBase64, length, pData);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 * Sends the AT+sendTo=[socketID],[family],[remotePort],[remoteAddress],[format],[length],[data] command
 *
 * input:
 * -  socketID        Id of the socket which should send the data
 * -  socket          socket to which to send
 * -  format          format in which the data is present. If Base64 encoding is selected, the modul will decode the data befor transmiting.
 * -  encodeToBase64  if true, data will be encoded to Base64 prio to sending to the module.
 * -  length          bytes to send
 * -  pData           data to send
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_sendTo(uint8_t socketID, ATSocket_Descriptor_t socket, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+sendTo=");

    ret = ATSocket_addArgumentsSendTo(pRequestCommand, socketID, socket, format, encodeToBase64, length, pData);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/**
 *  Sends the AT+setSockOpt=[socketID],[level],[option],[pValues] command
 *
 * input:
 * -  socketID   Id of the socket which should receive
 * -  level      level of opt
 * -  option     option of opt
 * -  pValues    value of opt
 *
 * return true if command was sent successful, false otherwise
 */
bool ATSocket_setSockOpt(uint8_t socketID, uint8_t level, uint8_t option, void *pValues)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATSOCKET_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+setSockOpt=");


    ret = ATSocket_addArgumentsSetSockOpt(pRequestCommand, socketID, level, option, pValues);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }


    return ret;
}

/*
extern bool ATSocket_select();
extern bool ATSocket_getSockOpt(uint8_t socketID, uint8_t level, uint8_t option, void *poutValues);
*/

/*
* Exported Functions.
* ########################## */

/* #############################
* Static Functions:
*/
/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  socket      Struct contains family, localPort and localAddress. See ATSocket_Descriptor_t
 * -  lastDelim   delimeter to append after the last argument
 *
 * output:
 * - pAtCommand  the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsFromSocketDescriptor(char *pAtCommand, ATSocket_Descriptor_t socket, char lastDelim)
{

    bool ret = false;
    if(socket.family  < ATSocket_Family_NumberOfValues)
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATSocketFamilyString[socket.family], ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, socket.port, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, socket.address,lastDelim);
    }

    return ret;
}

/**
 * Adds the arguments to the request command string
 *
 * input:
 * -  family       Family of the socket. See ATSocket_Family_t
 * -  type         Type of the socket. See ATSocket_Type_t
 * -  protocol     Protocol of the socket. See ATSocket_Protocol_t
 *
 * output:
 * - pAtCommand   the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsCreate(char *pAtCommand, ATSocket_Family_t family, ATSocket_Type_t type, ATSocket_Protocol_t protocol)
{
    bool ret = false;

    if(family < ATSocket_Family_NumberOfValues)
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATSocketFamilyString[family], ARGUMENT_DELIM);
    }

    if(ret && (type < ATSocket_Type_NumberOfValues))
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATSocketTypeString[type], ARGUMENT_DELIM);
    }

    if(ret && (protocol < ATSocket_Protocol_NumberOfValues))
    {
        ret = Calypso_appendArgumentString(pAtCommand, ATSocketProcotolString[protocol], STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 * Adds the arguments to the request command string
 *
 * input:
 * -  socketID    Id of the socket to be bind
 * -  socket      Struct contains family, localPort and localAddress. See ATSocket_Descriptor_t
 *
 * output:
 * - pAtCommand  the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsBindConnect(char *pAtCommand, uint8_t socketID, ATSocket_Descriptor_t socket)
{
    bool ret = false;

    ret = Calypso_appendArgumentInt(pAtCommand, socketID, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret)
    {
        ret = ATSocket_addArgumentsFromSocketDescriptor(pAtCommand, socket, STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  socketID    Id of the socket to receive
 * -  format      format in which the data should be formated
 * -  length      max number of bytes to receive
 *
 * output:
 * - pAtCommand  the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsRecv(char *pAtCommand, uint8_t socketID, Calypso_DataFormat_t format, uint16_t length)
{
    bool ret = false;


    ret = Calypso_appendArgumentInt(pAtCommand, socketID, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);


    if(ret && (format < Calypso_DataFormat_NumberOfValues))
    {
        ret = Calypso_appendArgumentInt(pAtCommand, format, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, length, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  socketID    Id of the socket to receive
 * -  socket      socket from which should be received
 * -  format      format in which the data should be formated
 * -  length      max number of bytes to receive
 *
 * output:
 * - pAtCommand  the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsRecvFrom(char *pAtCommand, uint8_t socketID, ATSocket_Descriptor_t socketFrom, Calypso_DataFormat_t format, uint16_t length)
{
    bool ret = false;


    ret = Calypso_appendArgumentInt(pAtCommand, socketID, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);


    if(ret)
    {
        ret = ATSocket_addArgumentsFromSocketDescriptor(pAtCommand, socketFrom, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, format, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, length, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  socketID        Id of the socket to receive
 * -  format          format in which the data is to be output. The module will encode the data and will ouput the data in this format.
 * -  encodeToBase64  if true, data will be encoded to Base64 prio to sending to the module.
 * -  length          max number of bytes to receive
 * -  pData           Data to sent
 *
 * output:
 * - pAtCommand      the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsSend(char *pAtCommand, uint8_t socketID, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData)
{
    bool ret = false;

    ret = (format < Calypso_DataFormat_NumberOfValues) && (length == strlen(pData));

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, socketID, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, format, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(encodeToBase64)
    {

        uint32_t lengthEncoded;
        lengthEncoded = Calypso_getBase64EncBufSize(length);

        char base64Buffer[lengthEncoded];
        Calypso_encodeBase64((uint8_t *)pData, length, (uint8_t *)base64Buffer, &lengthEncoded);

        if(ret)
        {
            ret = Calypso_appendArgumentInt(pAtCommand, lengthEncoded, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pAtCommand, base64Buffer, STRING_TERMINATE);
        }
    }
    else
    {

        if(ret)
        {
            ret = Calypso_appendArgumentInt(pAtCommand, length, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
        }

        if(ret)
        {
            Calypso_appendArgumentString(pAtCommand, pData, STRING_TERMINATE);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
        }
    }

    return ret;
}

/**
 *  Adds the arguments to the request command string
 *
 * input:
 * -  socketID        Id of the socket to receive
 * -  socket          socket from which should be received
 * -  format          format in which the data should be formated
 * -  encodeToBase64  if true, data will be encoded to Base64 prio to sending to the module.
 * -  length          max number of bytes to receive
 *
 * output:
 * - pAtCommand      the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsSendTo(char *pAtCommand, uint8_t socketID, ATSocket_Descriptor_t socketTo, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData)
{
    bool ret = false;


    ret = (format < Calypso_DataFormat_NumberOfValues) && (length == strlen(pData));

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, socketID, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = ATSocket_addArgumentsFromSocketDescriptor(pAtCommand, socketTo, ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, format, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(encodeToBase64)
    {

        uint32_t lengthEncoded;
        lengthEncoded = Calypso_getBase64EncBufSize(length);

        char base64Buffer[lengthEncoded];
        Calypso_encodeBase64((uint8_t *)pData, length, (uint8_t *)base64Buffer, &lengthEncoded);

        if(ret)
        {
            ret = Calypso_appendArgumentInt(pAtCommand, lengthEncoded, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pAtCommand, base64Buffer, STRING_TERMINATE);
        }
    }
    else
    {
        if(ret)
        {
            ret = Calypso_appendArgumentInt(pAtCommand, length, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pAtCommand, pData, STRING_TERMINATE);
        }
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 * Adds the arguments to the request command string
 *
 * input:
 * -  socketID    Id of the socket to receive
 * -  level       level of the opt
 * -  option      option if the opt
 * -  pValues     value of the opt
 *
 * output:
 * - pAtCommand  the request command string to add the arguments to
 *
 * return true if arguments were added successful, false otherwise
 */
static bool ATSocket_addArgumentsSetSockOpt(char *pAtCommand, uint8_t socketID, uint8_t level, uint8_t option, void *pValues)
{
    bool ret = false;


    ret = Calypso_appendArgumentInt(pAtCommand, socketID, (INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);


    if(ret)
    {
        if(ATSocket_SockOpt_Socket == level)
        {
            ret = Calypso_appendArgumentString(pAtCommand, ATSocketSockOptString[level], ARGUMENT_DELIM);
            switch(option)
            {
            case ATSocket_SockOptSocket_KeepAlive:
            case ATSocket_SockOptSocket_KeepAliveTime:
            case ATSocket_SockOptSocket_RXNoIPBoundary:
            case ATSocket_SockOptSocket_RCVBuf:
            case ATSocket_SockOptSocket_NonBlocking:
            {
                if(ret)
                {
                    ret = Calypso_appendArgumentString(pAtCommand, ATSocketSockOptSocketString[option], ARGUMENT_DELIM);
                }

                if(ret)
                {
                    uint32_t *pInt32Value = pValues;
                    ret = Calypso_appendArgumentInt(pAtCommand, *pInt32Value, (INTFLAGS_NOTATION_DEC|INTFLAGS_UNSIGNED), STRING_TERMINATE);
                }
                break;
            }

            /* Strings to append*/
            case ATSocket_SockOptSocket_SecureFilesCAFileName:
            case ATSocket_SockOptSocket_SecureFilesCertificateFileName:
            case ATSocket_SockOptSocket_SecureFilesDHKeyFileName:
            case ATSocket_SockOptSocket_SecureFilesPrivateKeyFileName:
            case ATSocket_SockOptSocket_SecureDomainNameVerification:
            {
                if(ret)
                {
                    ret = Calypso_appendArgumentString(pAtCommand, ATSocketSockOptSocketString[option], ARGUMENT_DELIM);
                }

                if(ret)
                {
                    ret = Calypso_appendArgumentString(pAtCommand, pValues, STRING_TERMINATE);
                }
                break;
            }
            case ATSocket_SockOptSocket_RCVTimeO:
            {
                if(ret)
                {
                    ret = Calypso_appendArgumentString(pAtCommand, ATSocketSockOptSocketString[option], ARGUMENT_DELIM);
                }

                uint32_t *pInt32Value = pValues;
                if(ret)
                {
                    ret = Calypso_appendArgumentInt(pAtCommand, *pInt32Value, (INTFLAGS_NOTATION_DEC|INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
                }

                if(ret)
                {
                    pInt32Value++;
                    ret = Calypso_appendArgumentInt(pAtCommand, *pInt32Value, (INTFLAGS_NOTATION_DEC|INTFLAGS_UNSIGNED), STRING_TERMINATE);
                }

                break;
            }
            case ATSocket_SockOptSocket_SecMethod:
            {
                if(ret)
                {
                    ret = Calypso_appendArgumentString(pAtCommand, ATSocketSockOptSocketString[option], ARGUMENT_DELIM);
                }

                if(ret)
                {
                    uint8_t *pIndex = pValues;
                    ret = Calypso_appendArgumentString(pAtCommand, AtSocketSockOptSecMethod[*pIndex], STRING_TERMINATE);
                }
                break;
            }
            /* Invalid option or not implemented */
            default:
            {
                ret = false;
                break;
            }
            }
        }
        else
        {
            ret = false;
        }
    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF,STRING_TERMINATE);
    }

    return ret;
}

/**
 * Parses the response of a AT+socket command
 *
 * input:
 * -      pAtCommand      the string reponse by the module
 *
 * output:
 * -     pOutSocketID    the parses socketID
 *
 * return true if response was parsed succesful, false otherwise
 */
static bool ATSocket_parseResponseCreate(char **pAtCommand, uint8_t *pOutSocketID)
{
    bool ret = false;
    const char *cmd = "+socket:";
    const size_t cmdLength = strlen(cmd);

    /* check if reponse is for get*/
    ret = (0 == strncmp(*pAtCommand, cmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        ret = Calypso_getNextArgumentInt(pAtCommand, pOutSocketID, INTFLAGS_SIZE8, STRING_TERMINATE);
    }

    return ret;

}


/**
 *  Parses a string to ATSocket_Family_t
 *
 * input:
 * -   familyString   string representing the socket family
 *
 * output:
 * -  pOutFamily     ATSocket_Family_t representing the socket family
 *
 * return true if parsed succesful, false otherwise
 */
bool ATSocket_parseSocketFamily(const char *familyString, ATSocket_Family_t *pOutFamily)
{
    for(int i = 0; i < ATSocket_Family_NumberOfValues; i++)
    {
        if(0 == strcasecmp(familyString, ATSocketFamilyString[i]))
        {
            *pOutFamily = (ATSocket_Family_t)i;
            return true;
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
