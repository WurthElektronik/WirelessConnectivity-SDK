#ifndef ATWLAN_H_INCLUDED
#define ATWLAN_H_INCLUDED

/* ###########################################################################
* Includes:
*/


/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/

#define ATWLAN_BSSID_LENGTH                    (29)
#define ATWLAN_SSID_MAX_LENGTH                 (32)
#define ATWLAN_SECURITYKEY_LENGTH              (32)
/*
* Constant Macros.
* ########################### */

/* #############################
* Function Macros:
*/

#define ATWLAN_SET_MODE(GENERATOR)       \
    GENERATOR(ATWLAN_SET_MODE_, STA)  \
    GENERATOR(ATWLAN_SET_MODE_, AP) \
    GENERATOR(ATWLAN_SET_MODE_, P2P)   \
    GENERATOR(ATWLAN_SET_MODE_, NumberOfValues)    \


#define ATWLAN_SECURITY_TYPE(GENERATOR)       \
    GENERATOR(ATWLAN_SECURITY_TYPE_, OPEN)  \
    GENERATOR(ATWLAN_SECURITY_TYPE_, WEP) \
    GENERATOR(ATWLAN_SECURITY_TYPE_, WPA) \
    GENERATOR(ATWLAN_SECURITY_TYPE_, WPA2) \
    GENERATOR(ATWLAN_SECURITY_TYPE_, WPA_WPA2)   \
    GENERATOR(ATWLAN_SECURITY_TYPE_, NumberOfValues)    \

#define ATWLAN_SCAN_SECURITY_TYPE(GENERATOR)       \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, OPEN)  \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, WEP) \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, WEP_SHARED)   \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, WPA_WPA2)   \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, WPA_ENT)   \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, WPS_PBC)   \
    GENERATOR(ATWLAN_SCAN_SECURITY_TYPE_, NumberOfValues)    \

#define ATWLAN_SCAN_CIPHER_TYPE(GENERATOR)       \
    GENERATOR(ATWLAN_SCAN_CIPHER_TYPE_, NONE)  \
    GENERATOR(ATWLAN_SCAN_CIPHER_TYPE_, WEP40) \
    GENERATOR(ATWLAN_SCAN_CIPHER_TYPE_, WEP104) \
    GENERATOR(ATWLAN_SCAN_CIPHER_TYPE_, TKIP) \
    GENERATOR(ATWLAN_SCAN_CIPHER_TYPE_, CCMP) \
    GENERATOR(ATWLAN_SCAN_CIPHER_TYPE_, NumberOfValues)    \

#define ATWLAN_SCAN_KEYMGMNT_TYPE(GENERATOR)       \
    GENERATOR(ATWLAN_SCAN_KEYMGMNT_TYPE_, NONE)  \
    GENERATOR(ATWLAN_SCAN_KEYMGMNT_TYPE_, 802_1_x) \
    GENERATOR(ATWLAN_SCAN_KEYMGMNT_TYPE_, PSK) \
    GENERATOR(ATWLAN_SCAN_KEYMGMNT_TYPE_, NumberOfValues)    \

#define ATWLAN_SECURITY_EAP(GENERATOR)  \
    GENERATOR(ATWLAN_SECURITY_EAP_, TLS) \
    GENERATOR(ATWLAN_SECURITY_EAP_, TTLS_TLS) \
    GENERATOR(ATWLAN_SECURITY_EAP_, TTLS_MSCHAPv2) \
    GENERATOR(ATWLAN_SECURITY_EAP_, TTLS_PSK) \
    GENERATOR(ATWLAN_SECURITY_EAP_, PEAP0_TLS) \
    GENERATOR(ATWLAN_SECURITY_EAP_, PEAP0_MSCHAPv2) \
    GENERATOR(ATWLAN_SECURITY_EAP_, PEAP0_PSK) \
    GENERATOR(ATWLAN_SECURITY_EAP_, PEAP1_TLS) \
    GENERATOR(ATWLAN_SECURITY_EAP_, PEAP1_PSK) \
    GENERATOR(ATWLAN_SECURITY_EAP_, NumberOfValues) \


#define ATWLAN_SET_ID(GENERATOR)       \
    GENERATOR(ATWLAN_SET_ID_, general)  \
    GENERATOR(ATWLAN_SET_ID_, P2P) \
    GENERATOR(ATWLAN_SET_ID_, AP) \
    GENERATOR(ATWLAN_SET_ID_, NumberOfValues)    \

#define ATWLAN_SET_GENERAL(GENERATOR)       \
    GENERATOR(ATWLAN_SET_GENERAL_, COUNTRY_CODE)  \
    GENERATOR(ATWLAN_SET_GENERAL_, STA_TX_POWER) \
    GENERATOR(ATWLAN_SET_GENERAL_, AP_TX_POWER) \
    GENERATOR(ATWLAN_SET_GENERAL_, SCAN_PARAMS) \
    GENERATOR(ATWLAN_SET_GENERAL_, SUSPEND_PROFILES) \
    GENERATOR(ATWLAN_SET_GENERAL_, DISABLE_ENT_SERVER_AUTH) \
    GENERATOR(ATWLAN_SET_GENERAL_, NumberOfValues)    \

#define ATWLAN_SET_P2P(GENERATOR)       \
    GENERATOR(ATWLAN_SET_P2P_, CHANNEL_N_REGS)  \
    GENERATOR(ATWLAN_SET_P2P_, NumberOfValues)    \

#define ATWLAN_SET_AP(GENERATOR)       \
    GENERATOR(ATWLAN_SET_AP_, SSID)  \
    GENERATOR(ATWLAN_SET_AP_, CHANNEL) \
    GENERATOR(ATWLAN_SET_AP_, HIDDEN_SSID) \
    GENERATOR(ATWLAN_SET_AP_, SECURITY) \
    GENERATOR(ATWLAN_SET_AP_, PASSWORD) \
    GENERATOR(ATWLAN_SET_AP_, MAX_STATIONS) \
    GENERATOR(ATWLAN_SET_AP_, MAX_STA_AGING) \
    GENERATOR(ATWLAN_SET_AP_, NumberOfValues)

#define ATWLAN_POLICY_ID(GENERATOR)       \
    GENERATOR(ATWLAN_POLICY_ID_, connection)  \
    GENERATOR(ATWLAN_POLICY_ID_, scan) \
    GENERATOR(ATWLAN_POLICY_ID_, PM) \
    GENERATOR(ATWLAN_POLICY_ID_, P2P) \
    GENERATOR(ATWLAN_POLICY_ID_, NumberOfValues)

#define ATWLAN_POLICY_CONNECTION(GENERATOR)       \
    GENERATOR(ATWLAN_POLICY_CONNECTION_, auto)  \
    GENERATOR(ATWLAN_POLICY_CONNECTION_, fast) \
    GENERATOR(ATWLAN_POLICY_CONNECTION_, P2P) \
    GENERATOR(ATWLAN_POLICY_CONNECTION_, NumberOfValues)

#define ATWLAN_POLICY_SCAN(GENERATOR)       \
    GENERATOR(ATWLAN_POLICY_SCAN_, Hidden_SSID)  \
    GENERATOR(ATWLAN_POLICY_SCAN_, Disable_Scan) \
    GENERATOR(ATWLAN_POLICY_SCAN_, NumberOfValues)

#define ATWLAN_POLICY_PM(GENERATOR)       \
    GENERATOR(ATWLAN_POLICY_PM_, normal)  \
    GENERATOR(ATWLAN_POLICY_PM_, low_latency) \
    GENERATOR(ATWLAN_POLICY_PM_, low_power) \
    GENERATOR(ATWLAN_POLICY_PM_, long_sleep) \
    GENERATOR(ATWLAN_POLICY_PM_, NumberOfValues)

#define ATWLAN_POLICY_P2P(GENERATOR)       \
    GENERATOR(ATWLAN_POLICY_P2P_, CLIENT)  \
    GENERATOR(ATWLAN_POLICY_P2P_, GROUP_OWNER) \
    GENERATOR(ATWLAN_POLICY_P2P_, NEGOTIATE) \
    GENERATOR(ATWLAN_POLICY_P2P_, NumberOfValues)

#define ATWLAN_POLICY_P2P_VALUE(GENERATOR)       \
    GENERATOR(ATWLAN_POLICY_P2P_VALUE_, ACTIVE)  \
    GENERATOR(ATWLAN_POLICY_P2P_VALUE_, PASSOVE) \
    GENERATOR(ATWLAN_POLICY_P2P_VALUE_, RAND_BACKOFF) \
    GENERATOR(ATWLAN_POLICY_P2P_VALUE_, NumberOfValues)
/*
* Function Macros.
* ########################## */

/* #############################
* Typedefs:
*/

typedef enum ATWLAN_SetMode_t
{
    ATWLAN_SET_MODE(GENERATE_ENUM)
} ATWLAN_SetMode_t;

typedef enum ATWLAN_SetID_t
{
    ATWLAN_SET_ID(GENERATE_ENUM)
} ATWLAN_SetID_t;

typedef enum ATWLAN_SetGeneral_t
{
    ATWLAN_SET_GENERAL(GENERATE_ENUM)
} ATWLAN_SetGeneral_t;

typedef enum ATWLAN_SetP2P_t
{
    ATWLAN_SET_P2P(GENERATE_ENUM)
} ATWLAN_SetP2P_t;

typedef enum ATWLAN_SetAP_t
{
    ATWLAN_SET_AP(GENERATE_ENUM)
} ATWLAN_SetAP_t;

typedef enum ATWLAN_PolicyID_t
{
    ATWLAN_POLICY_ID(GENERATE_ENUM)
} ATWLAN_PolicyID_t;

typedef enum ATWLAN_PolicyConnection_t
{
    ATWLAN_POLICY_CONNECTION(GENERATE_ENUM)
} ATWLAN_PolicyConnection_t;

typedef enum ATWLAN_PolicyScan_t
{
    ATWLAN_POLICY_SCAN(GENERATE_ENUM)
} ATWLAN_PolicyScan_t;

typedef enum ATWLAN_PolicyPM_t
{
    ATWLAN_POLICY_PM(GENERATE_ENUM)
} ATWLAN_PolicyPM_t;

typedef enum ATWLAN_PolicyP2P_t
{
    ATWLAN_POLICY_P2P(GENERATE_ENUM)
} ATWLAN_PolicyP2P_t;

typedef enum ATWLAN_PolicyP2PValue_t
{
    ATWLAN_POLICY_P2P_VALUE(GENERATE_ENUM)
} ATWLAN_PolicyP2PValue_t;

typedef enum ATWLAN_SecurityType_t
{
    ATWLAN_SECURITY_TYPE(GENERATE_ENUM)
} ATWLAN_SecurityType_t;

typedef enum ATWLAN_ScanSecurityType_t
{
    ATWLAN_SCAN_SECURITY_TYPE(GENERATE_ENUM)
} ATWLAN_ScanSecurityType_t;

typedef enum ATWLAN_ScanCipher_t
{
    ATWLAN_SCAN_CIPHER_TYPE(GENERATE_ENUM)
} ATWLAN_ScanCipher_t;

typedef enum ATWLAN_ScanKeyMgmnt_t
{
    ATWLAN_SCAN_KEYMGMNT_TYPE(GENERATE_ENUM)
} ATWLAN_ScanKeyMgmnt_t;

typedef enum ATWLAN_Security_EAP_t
{
    ATWLAN_SECURITY_EAP(GENERATE_ENUM)
} ATWLAN_Security_EAP_t;




typedef struct ATWLAN_SecurityParams_t
{
    ATWLAN_SecurityType_t securityType;
    char securityKey[ATWLAN_SECURITYKEY_LENGTH];
} ATWLAN_SecurityParams_t;

typedef struct ATWLAN_SecurityEnterpriseParams_t
{
    char extUser[32];
    char extAnonUser[32];
    ATWLAN_Security_EAP_t eapMethod;
} ATWLAN_SecurityEnterpriseParams_t;

typedef struct ATWLAN_ScanEntry_t
{
    char SSID[ATWLAN_SSID_MAX_LENGTH];
    char BSSID[ATWLAN_BSSID_LENGTH];
    uint8_t channel;
    uint8_t hiddenSsidEnabled;
    int8_t RSSI;
    ATWLAN_ScanCipher_t cypher;
    ATWLAN_ScanKeyMgmnt_t keyManagmentMethod;
    ATWLAN_SecurityType_t securityType;
} ATWLAN_ScanEntry_t;


typedef struct ATWLAN_ConnectionArguments_t
{
    char SSID[ATWLAN_SSID_MAX_LENGTH];
    char BSSID[ATWLAN_BSSID_LENGTH];
    ATWLAN_SecurityParams_t securityParams;
    ATWLAN_SecurityEnterpriseParams_t securityExtParams;
} ATWLAN_ConnectionArguments_t;

typedef struct ATWLAN_Profile_t
{
    char SSID[ATWLAN_SSID_MAX_LENGTH];
    char BSSID[ATWLAN_BSSID_LENGTH];
    uint32_t priority;
    ATWLAN_SecurityParams_t securityParams;
    ATWLAN_SecurityEnterpriseParams_t securityExtParams;
} ATWLAN_Profile_t;

typedef struct ATWLAN_ScanParams_t
{
    uint32_t channelMask;
    int32_t rssiTreshold;
} ATWLAN_ScanParams_t;

typedef struct ATWLAN_P2PChannelNRegs_t
{
    uint8_t listenChannel;
    uint8_t listenRegularityClass;
    uint8_t operatingChannel;
    uint8_t operatingRegularityClass;
} ATWLAN_P2PChannelNRegs_t;
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

extern bool ATWLAN_setMode(ATWLAN_SetMode_t mode);
extern bool ATWLAN_scan(uint8_t index, uint8_t deviceCount, ATWLAN_ScanEntry_t *pValues, uint8_t *pNumEntries);
extern bool ATWLAN_connect(ATWLAN_ConnectionArguments_t connectionArgs);
extern bool ATWLAN_disconnect();
extern bool ATWLAN_profileAdd(ATWLAN_ConnectionArguments_t connectionArgs, uint8_t priority, uint8_t *pOutIndex);
extern bool ATWLAN_profileGet(uint8_t index, ATWLAN_Profile_t *pOutProfile);
extern bool ATWLAN_profileDel(uint8_t index);
extern bool ATWLAN_get(uint8_t id, uint8_t option, void *pValues, size_t *pValuesSize);
extern bool ATWLAN_set(uint8_t id, uint8_t option, void *pValues);
extern bool ATWLAN_policySet(uint8_t id, uint8_t option, void *pValues);
extern bool ATWLAN_policyGet(uint8_t type, uint8_t *pOutOption, void *pOutValue);
/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */


#endif // ATWLAN_H_INCLUDED
