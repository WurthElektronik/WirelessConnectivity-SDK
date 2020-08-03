#ifndef _AT_MQTT_DEFINED
#define _AT_MQTT_DEFINED

/* ###########################################################################
* Includes:
*/

#include <stdbool.h>
#include <stdint.h>
#include "../Calypso.h"
#include "ATFile.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/

#define MQTT_MAX_TOPIC_LENGTH 128
#define MQTT_MAX_MESSAGE_LENGTH 128

/*
* Constant Macros.
* ########################### */

/* #############################
* Function Macros:
*/

#define ATMQTT_SECURITY_METHOD(GENERATOR)       \
    GENERATOR(ATMQTT_SECURITY_METHOD_, SSLV3)  \
    GENERATOR(ATMQTT_SECURITY_METHOD_, TLSV1) \
    GENERATOR(ATMQTT_SECURITY_METHOD_, TLSV1_1) \
    GENERATOR(ATMQTT_SECURITY_METHOD_, TLSV1_2) \
    GENERATOR(ATMQTT_SECURITY_METHOD_, SSLV3_TLSV1_2) \
    GENERATOR(ATMQTT_SECURITY_METHOD_, NumberOfValues) \

#define ATMQTT_CIPHER(GENERATOR) \
    GENERATOR(ATMQTT_CIPHER_, SSL_RSA_WITH_RC4_128_SHA) \
    GENERATOR(ATMQTT_CIPHER_, SSL_RSA_WITH_RC4_128_MD5) \
    GENERATOR(ATMQTT_CIPHER_, TLS_RSA_WITH_AES_256_CBC_SHA) \
    GENERATOR(ATMQTT_CIPHER_, TLS_DHE_RSA_WITH_AES_256_CBC_SHA) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_RSA_WITH_RC4_256_CBC_SHA) \
    GENERATOR(ATMQTT_CIPHER_, TLS_RSA_WITH_AES_128_CBC_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_RSA_WITH_AES_256_CBC_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA) \
    GENERATOR(ATMQTT_CIPHER_, TLS_RSA_WITH_AES_128_GCM_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_RSA_WITH_AES_256_GCM_SHA384) \
    GENERATOR(ATMQTT_CIPHER_, TLS_DHE_RSA_WITH_AES_128_GCM_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_DHE_RSA_WITH_AES_256_GCM_SHA384) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256) \
    GENERATOR(ATMQTT_CIPHER_, NumberOfValues) \

#define ATMQTT_PROTOCOL(GENERATOR)       \
    GENERATOR(ATMQTT_PROTOCOL_, v3_1)  \
    GENERATOR(ATMQTT_PROTOCOL_, v3_1_1) \
    GENERATOR(ATMQTT_PROTOCOL_, NumberOfValues)    \

#define ATMQTT_QOS(GENERATOR)       \
    GENERATOR(ATMQTT_QOS_, 0)  \
    GENERATOR(ATMQTT_QOS_, 1) \
    GENERATOR(ATMQTT_QOS_, 2) \
    GENERATOR(ATMQTT_QOS_, NumberOfValues)    \

#define ATMQTT_SET_OPTION(GENERATOR)       \
    GENERATOR(ATMQTT_SET_OPTION_, user)  \
    GENERATOR(ATMQTT_SET_OPTION_, password) \
    GENERATOR(ATMQTT_SET_OPTION_, will) \
    GENERATOR(ATMQTT_SET_OPTION_, keepAlive) \
    GENERATOR(ATMQTT_SET_OPTION_, clean) \
    GENERATOR(ATMQTT_SET_OPTION_, NumberOfValues)    \


#define ATMQTT_CREATE_FLAGS_IP4                 (1<<0)
#define ATMQTT_CREATE_FLAGS_IP6                 (1<<1)
#define ATMQTT_CREATE_FLAGS_URL                 (1<<2)
#define ATMQTT_CREATE_FLAGS_SEC                 (1<<3)
#define ATMQTT_CREATE_FLAGS_SKIP_DOMAIN_VERIFY  (1<<4)
#define ATMQTT_CREATE_FLAGS_SKIP_CERT_VERIFY    (1<<5)
#define ATMQTT_CREATE_FLAGS_SKIP_DATE_VERIFY    (1<<6)
#define ATMQTT_CREATE_FLAGS_NUM_OF_BITS          7

/*
* Function Macros.
* ########################## */

/* #############################
* Typedefs:
*/
typedef enum ATMQTT_SecurityMethods_t
{
    ATMQTT_SECURITY_METHOD(GENERATE_ENUM)
} ATMQTT_SecurityMethods_t;

typedef enum ATMQTT_Ciphers_t
{
    ATMQTT_CIPHER(GENERATE_ENUM)
} ATMQTT_Ciphers_t;

typedef enum ATMQTT_ProtocolVersion_t
{
    ATMQTT_PROTOCOL(GENERATE_ENUM)
} ATMQTT_ProtocolVersion_t;

typedef enum ATMQTT_QoS_t
{
    ATMQTT_QOS(GENERATE_ENUM)
} ATMQTT_QoS_t;

typedef enum ATMQTT_SetOption_t
{
    ATMQTT_SET_OPTION(GENERATE_ENUM)
} ATMQTT_SetOption_t;


typedef struct ATMQTT_ServerInfo_t
{
    char address[128];
    uint16_t port;

} ATMQTT_ServerInfo_t;

typedef struct ATMQTT_securityParams_t
{
    ATMQTT_SecurityMethods_t securityMethod;
    ATMQTT_Ciphers_t cipher;
    char privateKeyFile[FILENAME_MAX_LENGTH+1];
    char certificateFile[FILENAME_MAX_LENGTH+1];
    char CAFile[FILENAME_MAX_LENGTH+1];
    char DHKey[FILENAME_MAX_LENGTH+1];
} ATMQTT_securityParams_t;

typedef struct ATMQTT_connectionParams_t
{
    ATMQTT_ProtocolVersion_t protocolVersion;
    uint8_t blockingSend;
    Calypso_DataFormat_t format;
} ATMQTT_connectionParams_t;

typedef struct ATMQTT_subscribeTopic_t
{
    char topicString[MQTT_MAX_TOPIC_LENGTH];
    ATMQTT_QoS_t QoS;
} ATMQTT_subscribeTopic_t;

typedef struct ATMQTT_setWillParams_t
{
    char topic[MQTT_MAX_TOPIC_LENGTH];
    ATMQTT_QoS_t QoS;
    uint8_t retain;
    uint16_t messageLength;
    char message[MQTT_MAX_MESSAGE_LENGTH];
} ATMQTT_setWillParams_t;
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

extern bool ATMQTT_create(char *clientID, uint32_t flags, ATMQTT_ServerInfo_t serverInfo, ATMQTT_securityParams_t securityParams, ATMQTT_connectionParams_t connectionParams, uint8_t *pIndex);
extern bool ATMQTT_delete(uint8_t index);
extern bool ATMQTT_connect(uint8_t index);
extern bool ATMQTT_disconnect(uint8_t index);
extern bool ATMQTT_publish(uint8_t index, char *topicString, ATMQTT_QoS_t QoS, uint8_t retain, uint16_t messageLength, char *pMessage);
extern bool ATMQTT_subscribe(uint8_t index, uint8_t numOfTopics, ATMQTT_subscribeTopic_t *pTopics);
extern bool ATMQTT_unsubscribe(uint8_t index, uint8_t numOfTopics, char *topic1, char *topic2, char *topic3, char *topic4);
extern bool ATMQTT_set(uint8_t index, uint8_t option, void *pValues);

/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */


#endif /* _AT_MQTT_DEFINED*/
