#ifndef _AT_SOCKET_DEFINED
#define _AT_SOCKET_DEFINED

/* ###########################################################################
* Includes:
*/

#include <stdbool.h>
#include <stdint.h>
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

typedef enum ATSocket_Family_t
{
    ATSocket_Family_INET,
    ATSocket_Family_INET6,
    ATSocket_Family_NumberOfValues,
} ATSocket_Family_t;

typedef enum ATSocket_Type_t
{
    ATSocket_Type_Stream,
    ATSocket_Type_Datagram,
    ATSocket_Type_NumberOfValues,
} ATSocket_Type_t;

typedef enum ATSocket_Protocol_t
{
    ATSocket_Protocol_TCP,
    ATSocket_Protocol_UDP,
    ATSocket_Protocol_SEC,
    ATSocket_Protocol_NumberOfValues,
} ATSocket_Protocol_t;

typedef struct ATSocket_Descriptor_t
{
    ATSocket_Family_t family;
    uint16_t port;
    char     address[32];
} ATSocket_Descriptor_t;


typedef enum ATSocket_SockOpt_t
{
    ATSocket_SockOpt_Socket,
    ATSocket_SockOpt_IP,
    ATSocket_SockOpt_NumberOfValues,
} ATSocket_SockOpt_t;

typedef enum ATSocket_SockOptSocket_t
{
    ATSocket_SockOptSocket_KeepAlive,
    ATSocket_SockOptSocket_KeepAliveTime,
    ATSocket_SockOptSocket_RXNoIPBoundary,
    ATSocket_SockOptSocket_RCVTimeO,
    ATSocket_SockOptSocket_RCVBuf,
    ATSocket_SockOptSocket_NonBlocking,
    ATSocket_SockOptSocket_SecMethod,
    ATSocket_SockOptSocket_SecureMask,
    ATSocket_SockOptSocket_SecureFilesCAFileName,
    ATSocket_SockOptSocket_SecureFilesPrivateKeyFileName,
    ATSocket_SockOptSocket_SecureFilesCertificateFileName,
    ATSocket_SockOptSocket_SecureFilesDHKeyFileName,
    ATSocket_SockOptSocket_SecureDomainNameVerification,
    ATSocket_SockOptSocket_NumberOfValues,
} ATSocket_SockOptSocket_t;

typedef enum ATSocket_SockOptIP_t
{
    ATSocket_SockOptIP_MulticastTTL,
    ATSocket_SockOptIP_AddMembership,
    ATSocket_SockOptIP_DropMembership,
    ATSocket_SockOptIP_NumberOfValues,
} ATSocket_SockOptIP_t;


typedef enum ATSocket_SockOptSecMethod_t
{
    ATSocket_SockOptSecMethod_SSLv3,
    ATSocket_SockOptSecMethod_TLSv1,
    ATSocket_SockOptSecMethod_TLSv1_1,
    ATSocket_SockOptSecMethod_TLSv1_2,
    ATSocket_SockOptSecMethod_SSLv3_TLSv1_2,
    ATSocket_SockOptSecMethod_NumberOfValues,
} ATSocket_SockOptSecMethod_t;
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
extern bool ATSocket_create(ATSocket_Family_t family, ATSocket_Type_t type, ATSocket_Protocol_t protocol, uint8_t *pOutSocketID);
extern bool ATSocket_close(uint8_t socketID);
extern bool ATSocket_bind(uint8_t socketID, ATSocket_Descriptor_t socket);
extern bool ATSocket_listen(uint8_t socketID, uint16_t backlog);
extern bool ATSocket_connect(uint8_t socketID, ATSocket_Descriptor_t socket);
extern bool ATSocket_accept(uint8_t socketID, ATSocket_Family_t family);
extern bool ATSocket_select();
extern bool ATSocket_setSockOpt(uint8_t socketID, uint8_t level, uint8_t option, void *pValues);
extern bool ATSocket_getSockOpt(uint8_t socketID, uint8_t level, uint8_t option, void *poutValues);
extern bool ATSocket_recv(uint8_t socketID, Calypso_DataFormat_t format, uint16_t length);
extern bool ATSocket_recvFrom(uint8_t socketID, ATSocket_Descriptor_t socket, Calypso_DataFormat_t format, uint16_t length);
extern bool ATSocket_send(uint8_t socketID, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData);
extern bool ATSocket_sendTo(uint8_t socketID, ATSocket_Descriptor_t socket, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t length, char *pData);


extern bool ATSocket_parseSocketFamily(const char *familyString, ATSocket_Family_t *pOutFamily);
/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */


#endif /* _AT_SOCKET_DEFINED*/
