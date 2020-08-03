/* ###########################################################################
* Includes:
*/

#include "ATFile.h"
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

#define ATFILE_MAX_BUFFER_SIZE 2048 /* must be adopted to required file sizes. */

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

char responseBuffer[ATFILE_MAX_BUFFER_SIZE];
char requestBuffer[ATFILE_MAX_BUFFER_SIZE];

char *pRequestCommand;
char *pRespondCommand;


static const char *ATFile_OpenOptions_Strings[] =
{
    "CREATE", "READ", "WRITE", "OVERWRITE", "CREATE_FAILSAFE", "CREATE_SECURE", "CREATE_NOSIGNITURE", "CREATE_STATIC_TOKEN", "CREATE_VENDOR_TOKEN", "CREATE_PUBLIC_WRITE", "CREATE_PUBLIC_READ"
};

/*
* Static Globals.
* ########################## */

/* #############################
* Static Functions:
*/

static bool ATFile_AddArgumentsFileOpen(char *pAtCommand, const char *fileName, uint32_t options, uint16_t fileSize);
static bool ATFile_AddArgumentsFileClose(char *pAtCommand, uint32_t fileID, const char *certName, const char *signature);
static bool ATFile_AddArgumentsFileDel(char *pAtCommand, const char *fileName, uint32_t secureToken);
static bool ATFile_AddArgumentsFileRead(char *pAtCommand, uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, uint16_t bytesToRead);
static bool ATFile_AddArgumentsFileWrite(char *pAtCommand, uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t bytesToWrite, char *data);

static bool ATFile_ParseResponseFileOpen(char **pAtCommand, uint32_t *fileID, uint32_t *secureToken);
static bool ATFile_ParseResponseFileRead(char **pAtCommand, Calypso_DataFormat_t *pOutFormat, uint16_t *bytesRead, char *data);
static bool ATFile_ParseResponseFileWrite(char **pAtCommand, uint16_t *bytesWritten);
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
 *Sends the AT+FileOpen=[fileName],[options],[fileSize] command
 *
 *input:
 * -fileName: file to open
 * -options:  option flags for the file.
 * -fileSize: maximum size of the file. Will be allocate on create
 *
 *output:
 * -fileID:     ID of the opened file. Can be used for further file functions
 * -secureToken secure token of the opened file.
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATFile_open(const char *fileName, uint32_t options, uint16_t fileSize, uint32_t *fileID, uint32_t *secureToken)
{
    bool ret = false;


    pRespondCommand = &responseBuffer[0];

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATFILE_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+fileOpen=");

    ret = ATFile_AddArgumentsFileOpen(pRequestCommand, fileName, options, fileSize);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
    }

    if(ret)
    {
        ret = ATFile_ParseResponseFileOpen(&pRespondCommand, fileID, secureToken);
    }

    return ret;
}

/**
 *Sends the AT+FileClose=[fileID],[certificateFilename],[signature] command
 *
 *input:
 * -fileID:       file id of opened file. Is returned by ATFile_open
 * -certFileName: full path to certificate (optional). Can be NULL if not used.
 * -signature:    SHA1 signature (optional). Can be NULL if not used.
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATFile_close(uint32_t fileID, char *certFileName, char *signature)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATFILE_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+fileClose=");

    ret = ATFile_AddArgumentsFileClose(pRequestCommand, fileID, certFileName, signature);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+FileDel=[fileName],[secureToken] command
 *
 *input:
 * -fileName      name of the file to delete
 * -secureToken:  secure token returned by ATFile_open() (optional)
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATFile_del(const char *fileName, uint32_t secureToken)
{
    bool ret = false;

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATFILE_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+fileDel=");

    ret = ATFile_AddArgumentsFileDel(pRequestCommand, fileName, secureToken);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, NULL);
    }

    return ret;
}

/**
 *Sends the AT+FileRead=[fileID],[offset],[format],[length] command. The file has to be open prio to reading!
 *
 *input:
 * -fileID:      ID of file to read. Is returned by ATFile_open
 * -offset:      offset of the read operation
 * -format:      format of the output data.
 * -bytesToRead: number of bytes to read
 *
 *output:
 * -pOutFormat:  format of the output data
 * -byteRead:    number of bytes which had been read
 * -data         file content which had been read out
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATFile_read(uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, uint16_t bytesToRead, Calypso_DataFormat_t *pOutFormat, uint16_t *byteRead, char *data)
{
    bool ret = false;

    pRespondCommand = &responseBuffer[0];

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATFILE_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+fileRead=");

    ret = ATFile_AddArgumentsFileRead(pRequestCommand, fileID, offset, format, bytesToRead);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
    }

    if(ret)
    {
        ret = ATFile_ParseResponseFileRead(&pRespondCommand,pOutFormat, byteRead, data);
    }

    return ret;
}

/**
 *Sends the AT+FileWrite=[fileID],[offset],[format],[length],[data] command. The file has to be open prio to writing!
 *
 *input:
 * -fileID:       ID of file to write. Is returned by ATFile_open
 * -offset:       offset of the write operation
 * -format:       format of the input data.
 * -bytestoWrite: number of bytes to write
 *
 *output:
 * -writtenBytes: number of bytes which had been written
 *
 *return true  if succeeded
 *       false otherwise
 */
bool ATFile_write(uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t bytestoWrite, char *data, uint16_t *writtenBytes)
{
    bool ret = false;

    pRespondCommand = &responseBuffer[0];

    pRequestCommand = &requestBuffer[0];
    memset(pRequestCommand, 0, ATFILE_MAX_BUFFER_SIZE);
    strcpy(pRequestCommand, "AT+fileWrite=");

    ret = ATFile_AddArgumentsFileWrite(pRequestCommand, fileID, offset, format, encodeToBase64, bytestoWrite, data);

    if(ret)
    {
        Calypso_sendRequest(pRequestCommand);
        ret = Calypso_Wait4CNF(500, Calypso_CNFStatus_Success, true, pRespondCommand);
    }

    if(ret)
    {
        ret = ATFile_ParseResponseFileWrite(&pRespondCommand, writtenBytes);
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
 *Adds the arguments to the request command string
 *
 *input:
 * -pAtCommand  the request command string to add the arguments to
 * -fileName: file to open
 * -options:  option flags for the file.
 * -fileSize: maximum size of the file. Will be allocate on create
 *
 *return true if arguments were added successful
 *       false otherwise
 */
static bool ATFile_AddArgumentsFileOpen(char *pAtCommand, const char *fileName, uint32_t options, uint16_t fileSize)
{
    bool ret = false;

    if((NULL == pAtCommand) || (NULL == fileName))
    {
        return false;
    }


    if(strlen(fileName) <= FILENAME_MAX_LENGTH)
    {
        ret = Calypso_appendArgumentString(pAtCommand, fileName, ARGUMENT_DELIM);
    }


    if(ret)
    {
        char optionString[128];
        memset(optionString,0,128);

        uint32_t bitmask;
        for(int i = 0; i < ATFILE_OPEN_NUM_OF_BITS; i++)
        {
            bitmask = (1 << i);
            if(0 != (bitmask & options))
            {
                Calypso_appendArgumentString(optionString, ATFile_OpenOptions_Strings[i],BITMASK_DELIM);
            }
        }
        /* delete last '|' */
        optionString[strlen(optionString)-1] = '\0';
        ret = Calypso_appendArgumentString(pAtCommand, optionString, ARGUMENT_DELIM);
    }

    if(FILE_MIN_SIZE <= fileSize)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, fileSize, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), STRING_TERMINATE);
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
 * -fileID:       file id of opened file. Is returned by ATFile_open
 * -certFileName: full path to certificate (optional). Can be NULL if not used.
 * -signature:    SHA1 signature (optional). Can be NULL if not used.
 *
 *return true if arguments were added successful
 *       false otherwise
 */
static bool ATFile_AddArgumentsFileClose(char *pAtCommand, uint32_t fileID, const char *certName, const char *signature)
{
    bool ret = false;

    if(NULL == pAtCommand)
    {
        return false;
    }

    ret = Calypso_appendArgumentInt(pAtCommand, fileID, (INTFLAGS_UNSIGNED | INTFLAGS_NOTATION_DEC), ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, certName, ARGUMENT_DELIM);

    }

    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, signature, STRING_TERMINATE);
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
 * -fileName      name of the file to delete
 * -secureToken:  secure token returned by ATFile_open() (optional)
 *
 *return true if arguments were added successful
 *       false otherwise
 */
static bool ATFile_AddArgumentsFileDel(char *pAtCommand, const char *fileName, uint32_t secureToken)
{
    bool ret = false;

    if(NULL == pAtCommand)
    {
        return false;
    }

    ret = Calypso_appendArgumentString(pAtCommand, fileName, ARGUMENT_DELIM);


    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, secureToken,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
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
 * -fileID:      ID of file to read. Is returned by ATFile_open
 * -offset:      offset of the read operation
 * -format:      format of the output data.
 * -bytesToRead: number of bytes to read
 *
 *return true if arguments were added successful
 *       false otherwise
 */
static bool ATFile_AddArgumentsFileRead(char *pAtCommand, uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, uint16_t bytesToRead)
{
    bool ret = false;

    if(NULL == pAtCommand)
    {
        return false;
    }

    ret = Calypso_appendArgumentInt(pAtCommand, fileID,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, offset,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, format,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, bytesToRead,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), STRING_TERMINATE);
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
 * -fileID:       ID of file to write. Is returned by ATFile_open
 * -offset:       offset of the write operation
 * -format:       format of the input data.
 * -bytestoWrite: number of bytes to write
 *
 *return true if arguments were added successful
 *       false otherwise
 */
static bool ATFile_AddArgumentsFileWrite(char *pAtCommand, uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t bytesToWrite, char *pData)
{
    bool ret = false;

    if(NULL == pAtCommand)
    {
        return false;
    }

    ret = Calypso_appendArgumentInt(pAtCommand, fileID,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, offset,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }

    if(ret)
    {
        ret = Calypso_appendArgumentInt(pAtCommand, format,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
    }


    if(encodeToBase64)
    {

        uint32_t lengthEncoded;
        lengthEncoded = Calypso_getBase64EncBufSize(bytesToWrite);

        char base64Buffer[lengthEncoded];
        Calypso_encodeBase64((uint8_t *)pData, bytesToWrite, (uint8_t *)base64Buffer, &lengthEncoded);

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
            ret = Calypso_appendArgumentInt(pAtCommand, bytesToWrite,(INTFLAGS_NOTATION_DEC | INTFLAGS_UNSIGNED), ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_appendArgumentString(pAtCommand, pData, STRING_TERMINATE);
        }

    }
    if(ret)
    {
        ret = Calypso_appendArgumentString(pAtCommand, CRLF, STRING_TERMINATE);
    }

    return ret;
}

/**
*Parses the response of a AT+fileopen command
*
*input:
* -pAtCommand      the string reponse by the module
*
*output:
* -fileID:     ID of the opened file. Can be used for further file functions
* -secureToken secure token of the opened file.
*
*return true if arguments were added successful
*       false otherwise
*/
static bool ATFile_ParseResponseFileOpen(char **pAtCommand, uint32_t *fileID, uint32_t *secureToken)
{
    bool ret = false;
    const char *cmd = "+fileopen:";
    const size_t cmdLength = strlen(cmd);

    ret = (0 == strncmp(*pAtCommand, cmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        ret = Calypso_getNextArgumentInt(pAtCommand, fileID, INTFLAGS_SIZE32, ARGUMENT_DELIM);

        if(ret)
        {
            ret = Calypso_getNextArgumentInt(pAtCommand, secureToken, INTFLAGS_SIZE32, STRING_TERMINATE);
        }
    }

    return ret;
}

/**
*Parses the response of a AT+fileread command
*
*input:
* -pAtCommand      the string reponse by the module
*
*output:
* -pOutFormat:  format of the output data
* -byteRead:    number of bytes which had been read
* -data         file content which had been read out
*
*return true if arguments were added successful
*       false otherwise
*/
static bool ATFile_ParseResponseFileRead(char **pAtCommand, Calypso_DataFormat_t *pOutFormat, uint16_t *bytesRead, char *data)
{
    bool ret = false;
    const char *cmd = "+fileread:";
    const size_t cmdLength = strlen(cmd);

    ret = (0 == strncmp(*pAtCommand, cmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;

        ret = Calypso_getNextArgumentInt(pAtCommand, pOutFormat, INTFLAGS_SIZE8, ARGUMENT_DELIM);

        if(ret)
        {
            ret = Calypso_getNextArgumentInt(pAtCommand, bytesRead, INTFLAGS_SIZE16, ARGUMENT_DELIM);
        }

        if(ret)
        {
            ret = Calypso_getNextArgumentString(pAtCommand, data, STRING_TERMINATE);
        }
    }

    return ret;
}

/**
*Parses the response of a AT+fileread command
*
*input:
* -pAtCommand      the string reponse by the module
*
*output:
* -writtenBytes: number of bytes which had been written
*
*return true if arguments were added successful
*       false otherwise
*/
static bool ATFile_ParseResponseFileWrite(char **pAtCommand, uint16_t *bytesWritten)
{
    bool ret = false;
    const char *cmd = "+filewrite:";
    const size_t cmdLength = strlen(cmd);

    ret = (0 == strncmp(*pAtCommand, cmd, cmdLength));
    if(ret)
    {
        *pAtCommand += cmdLength;
        ret = Calypso_getNextArgumentInt(pAtCommand, bytesWritten, INTFLAGS_SIZE16, STRING_TERMINATE);
    }

    return ret;
}

/*
* Static Functions.
* ########################## */

/*
* Definitions.
* ######################################################################### */
