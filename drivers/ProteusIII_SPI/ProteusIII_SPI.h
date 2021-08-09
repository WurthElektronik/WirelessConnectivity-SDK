/**
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK:
 * https://www.we-online.com/wireless-connectivity
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2021 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ProteusIII_SPI_defined
#define _ProteusIII_SPI_defined

/* Pins */
/* Different pin numbers used for libgpio or wiringPi */
/* libgiod numbers */
#ifdef libgpiod_numbers
#define ProteusIII_SPI_PIN_RESET  4   // BCM_GPIO 4
#define ProteusIII_SPI_PIN_WAKEUP 17  // BCM_GPIO 17
#define ProteusIII_SPI_PIN_BOOT   23  // BCM_GPIO Rv1
#define ProteusIII_SPI_PIN_SPI_CS 25  /* BCM_GPIO 25 - CE0 pin will most likely not work, so unused gpio is used */

#elif defined wiringPi_numbers
/* wiring pi numbers */
#define ProteusIII_SPI_PIN_RESET  7   //  wiringPi (pin 7 - BCM_GPIO 4)
#define ProteusIII_SPI_PIN_WAKEUP 0   //  wiringPi (pin 0 - BCM_GPIO 17)
#define ProteusIII_SPI_PIN_BOOT   2   //  wiringPi (pin 2 - BCM_GPIO Rv1)
#define ProteusIII_SPI_PIN_SPI_CS 6   /*  wiringPi (pin 6 - BCM_GPIO 25) - CE0 pin will most likely not work, so unused gpio is used */

#else
#error "No pins defined"
#endif
/* timings */
#define ProteusIII_SPI_BOOT_DURATION (uint16_t)75

typedef struct ProteusIII_SPI_Device_t {
    uint8_t btmac[6];
    int8_t  rssi;
    int8_t  txpower;
    uint8_t devicenamelength;
    uint8_t devicename[32];
} ProteusIII_SPI_Device_t;

#define MAX_NUMBER_OF_DEVICES (uint8_t)10
typedef struct ProteusIII_SPI_GetDevices_t {
    uint8_t numberofdevices;
    ProteusIII_SPI_Device_t devices[MAX_NUMBER_OF_DEVICES];
} ProteusIII_SPI_GetDevices_t;

typedef enum ProteusIII_SPI_DisplayPasskeyAction_t{
    ProteusIII_SPI_DisplayPasskeyAction_NoAction      = (uint8_t)0x00,
    ProteusIII_SPI_DisplayPasskeyAction_PleaseConfirm = (uint8_t)0x01
} ProteusIII_SPI_DisplayPasskeyAction_t;

#define ProteusIII_SPI_AMOUNT_GPIO_PINS    6
typedef enum ProteusIII_SPI_GPIO_t
{
    ProteusIII_SPI_GPIO_1 = (uint8_t)0x01,
    ProteusIII_SPI_GPIO_2 = (uint8_t)0x02,
    ProteusIII_SPI_GPIO_3 = (uint8_t)0x03,
    ProteusIII_SPI_GPIO_4 = (uint8_t)0x04,
    ProteusIII_SPI_GPIO_5 = (uint8_t)0x05,
    ProteusIII_SPI_GPIO_6 = (uint8_t)0x06
} ProteusIII_SPI_GPIO_t;

typedef enum ProteusIII_SPI_GPIO_IO_t
{
    ProteusIII_SPI_GPIO_IO_Disconnected  = (uint8_t)0x00,
    ProteusIII_SPI_GPIO_IO_Input         = (uint8_t)0x01,
    ProteusIII_SPI_GPIO_IO_Output        = (uint8_t)0x02,
    ProteusIII_SPI_GPIO_IO_PWM           = (uint8_t)0x03
} ProteusIII_SPI_GPIO_IO_t;

typedef enum ProteusIII_SPI_GPIO_Output_t
{
    ProteusIII_SPI_GPIO_Output_Low  = (uint8_t)0x00,
    ProteusIII_SPI_GPIO_Output_High = (uint8_t)0x01
} ProteusIII_SPI_GPIO_Output_t;

typedef enum ProteusIII_SPI_GPIO_Input_t
{
    ProteusIII_SPI_GPIO_Input_NoPull   = (uint8_t)0x00,
    ProteusIII_SPI_GPIO_Input_PullDown = (uint8_t)0x01,
    ProteusIII_SPI_GPIO_Input_PullUp   = (uint8_t)0x02
} ProteusIII_SPI_GPIO_Input_t;

typedef struct ProteusIII_SPI_GPIO_PwmValue_t{
    uint16_t period; /* in ms */
    uint8_t  ratio;  /* 0-255 (0%-100%)*/
} ProteusIII_SPI_GPIO_PwmValue_t;

typedef struct ProteusIII_SPI_GPIOConfigBlock_t{
    ProteusIII_SPI_GPIO_t GPIO_ID;
    ProteusIII_SPI_GPIO_IO_t function;
    union /* 3Byte */
    {
        ProteusIII_SPI_GPIO_PwmValue_t pwm;
        ProteusIII_SPI_GPIO_Input_t input;
        ProteusIII_SPI_GPIO_Output_t output;
    } value;
} ProteusIII_SPI_GPIOConfigBlock_t;

typedef struct ProteusIII_SPI_GPIOControlBlock_t{
    ProteusIII_SPI_GPIO_t GPIO_ID;
    union /* 1Byte */
    {
        ProteusIII_SPI_GPIO_Output_t output;
		uint8_t ratio;  /* 0-255 (0%-100%)*/
    } value;
} ProteusIII_SPI_GPIOControlBlock_t;

typedef enum ProteusIII_SPI_States_t {
    ProteusIII_SPI_State_BLE_Invalid =         (uint8_t)0x00,
    ProteusIII_SPI_State_BLE_Connected =       (uint8_t)0x01,
    ProteusIII_SPI_State_BLE_Channel_Open =    (uint8_t)0x02,
} ProteusIII_SPI_States_t;

typedef enum ProteusIII_SPI_Security_t {
    ProteusIII_SPI_State_BLE_ReBonded = (uint8_t)0x00,
    ProteusIII_SPI_State_BLE_Bonded =   (uint8_t)0x01,
    ProteusIII_SPI_State_BLE_Paired =   (uint8_t)0x02,
} ProteusIII_SPI_Security_t;

typedef enum ProteusIII_SPI_BLE_Role_t {
    ProteusIII_SPI_BLE_Role_None =         (uint8_t)0x00,
    ProteusIII_SPI_BLE_Role_Pheripheral =  (uint8_t)0x01,
    ProteusIII_SPI_BLE_Role_Central =      (uint8_t)0x02,
    ProteusIII_SPI_BLE_Role_DTM =          (uint8_t)0x10,
} ProteusIII_SPI_BLE_Role_t;

typedef enum ProteusIII_SPI_BLE_Action_t {
    ProteusIII_SPI_BLE_Action_None =       (uint8_t)0x00,
    ProteusIII_SPI_BLE_Action_Idle =       (uint8_t)0x01,
    ProteusIII_SPI_BLE_Action_Scanning =   (uint8_t)0x02,
    ProteusIII_SPI_BLE_Action_Connected =  (uint8_t)0x03,
    ProteusIII_SPI_BLE_Action_Sleep =      (uint8_t)0x04,
    ProteusIII_SPI_BLE_Action_DTM =        (uint8_t)0x05,
} ProteusIII_SPI_BLE_Action_t;


/* user settings */
typedef enum ProteusIII_SPI_UserSettings_t {
    ProteusIII_SPI_USERSETTING_POSITION_FS_FWVersion =             (uint8_t)0x01,
    ProteusIII_SPI_USERSETTING_POSITION_RF_DEVICE_NAME =           (uint8_t)0x02,
    ProteusIII_SPI_USERSETTING_POSITION_FS_MAC =                   (uint8_t)0x03,
    ProteusIII_SPI_USERSETTING_POSITION_FS_BTMAC =                 (uint8_t)0x04,
    ProteusIII_SPI_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT =   (uint8_t)0x07,
    ProteusIII_SPI_USERSETTING_POSITION_RF_CONNECTION_TIMING =     (uint8_t)0x08,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SCAN_TIMING =           (uint8_t)0x09,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SCAN_FACTOR =           (uint8_t)0x0A,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SEC_FLAGS =             (uint8_t)0x0C,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SCAN_FLAGS =            (uint8_t)0x0D,
    ProteusIII_SPI_USERSETTING_POSITION_RF_BEACON_FLAGS =          (uint8_t)0x0E,
    ProteusIII_SPI_USERSETTING_POSITION_FS_DEVICE_INFO =           (uint8_t)0x0F,
    ProteusIII_SPI_USERSETTING_POSITION_FS_SERIAL_NUMBER =         (uint8_t)0x10,
    ProteusIII_SPI_USERSETTING_POSITION_RF_TX_POWER =              (uint8_t)0x11,
    ProteusIII_SPI_USERSETTING_POSITION_RF_STATIC_PASSKEY =        (uint8_t)0x12,
	ProteusIII_SPI_USERSETTING_POSITION_DIS_FLAGS =                (uint8_t)0x13,
	ProteusIII_SPI_USERSETTING_POSITION_DIS_MANUFACTURERNAME =     (uint8_t)0x14,
	ProteusIII_SPI_USERSETTING_POSITION_DIS_MODELNUMBER =          (uint8_t)0x15,
	ProteusIII_SPI_USERSETTING_POSITION_DIS_SERIALNUMBER =         (uint8_t)0x16,
	ProteusIII_SPI_USERSETTING_POSITION_DIS_HWVERSION =            (uint8_t)0x17,
	ProteusIII_SPI_USERSETTING_POSITION_DIS_SWVERSION =            (uint8_t)0x18,
	ProteusIII_SPI_USERSETTING_POSITION_RF_APPEARANCE =            (uint8_t)0x19,
	ProteusIII_SPI_USERSETTING_POSITION_RF_SPPBASEUUID =           (uint8_t)0x1A,
    ProteusIII_SPI_USERSETTING_POSITION_RF_CFGFLAGS =              (uint8_t)0x1C,
	ProteusIII_SPI_USERSETTING_POSITION_RF_ADVERTISING_FLAGS =     (uint8_t)0x1D,
	ProteusIII_SPI_USERSETTING_POSITION_SPI_CONFIGINDEX =          (uint8_t)0x1E,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SPPServiceUUID =        (uint8_t)0x20,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SPPRXUUID =             (uint8_t)0x21,
    ProteusIII_SPI_USERSETTING_POSITION_RF_SPPTXUUID =             (uint8_t)0x22,
} ProteusIII_SPI_UserSettings_t;

#define SEC_MODE_BONDING_ENABLE_MASK          (uint8_t)0x08
#define SEC_MODE_BONDEDCONNECTIONSONLY_ENABLE (uint8_t)0x10
typedef enum ProteusIII_SPI_SecFlags_t {
    ProteusIII_SPI_SecFlags_NONE =                       (uint8_t)0x00,
    ProteusIII_SPI_SecFlags_LescJustWorks =              (uint8_t)0x01,
    ProteusIII_SPI_SecFlags_JustWorks =                  (uint8_t)0x02,
    ProteusIII_SPI_SecFlags_StaticPassKey =              (uint8_t)0x03,
    ProteusIII_SPI_SecFlags_LescNumCompare =             (uint8_t)0x04,
    ProteusIII_SPI_SecFlags_LescPassKey =                (uint8_t)0x05,
    ProteusIII_SPI_SecFlags_LescJustWorks_Bonding =      (uint8_t)(ProteusIII_SPI_SecFlags_LescJustWorks | SEC_MODE_BONDING_ENABLE_MASK),
    ProteusIII_SPI_SecFlags_JustWorks_Bonding =          (uint8_t)(ProteusIII_SPI_SecFlags_JustWorks | SEC_MODE_BONDING_ENABLE_MASK),
    ProteusIII_SPI_SecFlags_StaticPassKey_Bonding =      (uint8_t)(ProteusIII_SPI_SecFlags_StaticPassKey | SEC_MODE_BONDING_ENABLE_MASK),
	ProteusIII_SPI_SecFlags_LescNumCompare_Bonding =     (uint8_t)(ProteusIII_SPI_SecFlags_LescNumCompare | SEC_MODE_BONDING_ENABLE_MASK),
    ProteusIII_SPI_SecFlags_LescPassKey_Bonding =        (uint8_t)(ProteusIII_SPI_SecFlags_LescPassKey | SEC_MODE_BONDING_ENABLE_MASK),
    ProteusIII_SPI_SecFlags_LescJustWorks_BondingOnly =  (uint8_t)(ProteusIII_SPI_SecFlags_LescJustWorks | SEC_MODE_BONDING_ENABLE_MASK | SEC_MODE_BONDEDCONNECTIONSONLY_ENABLE),
    ProteusIII_SPI_SecFlags_JustWorks_BondingOnly =      (uint8_t)(ProteusIII_SPI_SecFlags_JustWorks | SEC_MODE_BONDING_ENABLE_MASK | SEC_MODE_BONDEDCONNECTIONSONLY_ENABLE),
    ProteusIII_SPI_SecFlags_StaticPassKey_BondingOnly =  (uint8_t)(ProteusIII_SPI_SecFlags_StaticPassKey | SEC_MODE_BONDING_ENABLE_MASK | SEC_MODE_BONDEDCONNECTIONSONLY_ENABLE),
	ProteusIII_SPI_SecFlags_LescNumCompare_BondingOnly = (uint8_t)(ProteusIII_SPI_SecFlags_LescNumCompare | SEC_MODE_BONDING_ENABLE_MASK | SEC_MODE_BONDEDCONNECTIONSONLY_ENABLE),
    ProteusIII_SPI_SecFlags_LescPassKey_BondingOnly =    (uint8_t)(ProteusIII_SPI_SecFlags_LescPassKey | SEC_MODE_BONDING_ENABLE_MASK | SEC_MODE_BONDEDCONNECTIONSONLY_ENABLE),
} ProteusIII_SPI_SecFlags_t;

typedef enum ProteusIII_SPI_ConnectionTiming_t {
    ProteusIII_SPI_ConnectionTiming_0 = (uint8_t)0x00,
    ProteusIII_SPI_ConnectionTiming_1 = (uint8_t)0x01,
    ProteusIII_SPI_ConnectionTiming_2 = (uint8_t)0x02,
    ProteusIII_SPI_ConnectionTiming_3 = (uint8_t)0x03,
    ProteusIII_SPI_ConnectionTiming_4 = (uint8_t)0x04,
    ProteusIII_SPI_ConnectionTiming_5 = (uint8_t)0x05,
    ProteusIII_SPI_ConnectionTiming_6 = (uint8_t)0x06,
    ProteusIII_SPI_ConnectionTiming_7 = (uint8_t)0x07,
    ProteusIII_SPI_ConnectionTiming_8 = (uint8_t)0x08
} ProteusIII_SPI_ConnectionTiming_t;

typedef enum ProteusIII_SPI_ScanTiming_t {
    ProteusIII_SPI_ScanTiming_0  = (uint8_t)0x00,
    ProteusIII_SPI_ScanTiming_1  = (uint8_t)0x01,
    ProteusIII_SPI_ScanTiming_2  = (uint8_t)0x02,
    ProteusIII_SPI_ScanTiming_3  = (uint8_t)0x03,
    ProteusIII_SPI_ScanTiming_4  = (uint8_t)0x04,
    ProteusIII_SPI_ScanTiming_5  = (uint8_t)0x05,
    ProteusIII_SPI_ScanTiming_6  = (uint8_t)0x06,
    ProteusIII_SPI_ScanTiming_7  = (uint8_t)0x07,
    ProteusIII_SPI_ScanTiming_8  = (uint8_t)0x08,
    ProteusIII_SPI_ScanTiming_9  = (uint8_t)0x09,
    ProteusIII_SPI_ScanTiming_10 = (uint8_t)0x0A,
    ProteusIII_SPI_ScanTiming_11 = (uint8_t)0x0B
} ProteusIII_SPI_ScanTiming_t;

typedef enum ProteusIII_SPI_TXPower_t {
	ProteusIII_SPI_TXPower_8       = (int8_t) 8,
    ProteusIII_SPI_TXPower_7       = (int8_t) 7,
    ProteusIII_SPI_TXPower_6       = (int8_t) 6,
    ProteusIII_SPI_TXPower_5       = (int8_t) 5,
    ProteusIII_SPI_TXPower_4       = (int8_t) 4,
    ProteusIII_SPI_TXPower_3       = (int8_t) 3,
	ProteusIII_SPI_TXPower_2       = (int8_t) 2,
	ProteusIII_SPI_TXPower_0       = (int8_t) 0,
    ProteusIII_SPI_TXPower_minus4  = (int8_t)-4,
    ProteusIII_SPI_TXPower_minus8  = (int8_t)-8,
    ProteusIII_SPI_TXPower_minus12 = (int8_t)-12,
} ProteusIII_SPI_TXPower_t;

typedef enum ProteusIII_SPI_Config_t {
    ProteusIII_SPI_ConfigIndex_Mode0_activehigh    = (uint8_t)0,
    ProteusIII_SPI_ConfigIndex_Mode1_activehigh    = (uint8_t)1,
    ProteusIII_SPI_ConfigIndex_Mode2_activehigh    = (uint8_t)2,
    ProteusIII_SPI_ConfigIndex_Mode3_activehigh    = (uint8_t)3,
    ProteusIII_SPI_ConfigIndex_Mode0_activelow     = (uint8_t)4,
    ProteusIII_SPI_ConfigIndex_Mode1_activelow     = (uint8_t)5,
    ProteusIII_SPI_ConfigIndex_Mode2_activelow     = (uint8_t)6,
    ProteusIII_SPI_ConfigIndex_Mode3_activelow     = (uint8_t)7
} ProteusIII_SPI_Config_t;

typedef enum ProteusIII_SPI_Phy_t
{
    ProteusIII_SPI_Phy_1MBit = 0x01,
    ProteusIII_SPI_Phy_2MBit = 0x02,
	ProteusIII_SPI_Phy_125kBit_LECoded = 0x04,
} ProteusIII_SPI_Phy_t;

/* Callback definition */

typedef void (*RxCallback)(uint8_t* payload, uint16_t payload_length, uint8_t* BTMAC, int8_t rssi);
typedef void (*ConnectCallback)(uint8_t* BTMAC);
typedef void (*SecurityCallback)(uint8_t* BTMAC, ProteusIII_SPI_Security_t security_state);
typedef void (*PasskeyCallback)(uint8_t* BTMAC);
typedef void (*DisplayPasskeyCallback)(ProteusIII_SPI_DisplayPasskeyAction_t action, uint8_t* BTMAC, uint8_t* passkey);
typedef void (*DisconnectCallback)();
typedef void (*ChannelopenCallback)(uint8_t* BTMAC, uint16_t max_payload);
typedef void (*PhyupdateCallback)(uint8_t* BTMAC, uint8_t phy_rx, uint8_t phy_tx);

typedef struct ProteusIII_SPI_CallbackConfig_t {
    RxCallback              rxCb;
    ConnectCallback         connectCp;
    SecurityCallback        securityCb;
    PasskeyCallback         passkeyCb;
    DisplayPasskeyCallback  displayPasskeyCb;
    DisconnectCallback      disconnectCb;
    ChannelopenCallback     channelOpenCb;
    PhyupdateCallback       phyUpdateCb;
} ProteusIII_SPI_CallbackConfig_t;


extern bool ProteusIII_SPI_Init(ProteusIII_SPI_Config_t index, int datarate_bit, int rp, int wp, int bp, ProteusIII_SPI_CallbackConfig_t callbackConfig);
extern bool ProteusIII_SPI_Deinit(void);

extern bool ProteusIII_SPI_PinReset(void);
extern bool ProteusIII_SPI_Reset(void);

extern bool ProteusIII_SPI_Sleep();
extern bool ProteusIII_SPI_PinWakeup();

extern bool ProteusIII_SPI_Connect(uint8_t *btmacP);
extern bool ProteusIII_SPI_Disconnect();

extern bool ProteusIII_SPI_Scanstart();
extern bool ProteusIII_SPI_Scanstop();
extern bool ProteusIII_SPI_GetDevices(ProteusIII_SPI_GetDevices_t* devicesP);

extern bool ProteusIII_SPI_Transmit(uint8_t* PayloadP, uint16_t length);

extern bool ProteusIII_SPI_Passkey(uint8_t* passkeyP);
extern bool ProteusIII_SPI_NumericCompareConfirm(bool keyIsOk);

extern bool ProteusIII_SPI_PhyUpdate(ProteusIII_SPI_Phy_t phy);

extern ProteusIII_SPI_States_t ProteusIII_SPI_GetDriverState();

/* functions to control the GPIO feature */
extern bool ProteusIII_SPI_GPIOLocalWriteConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t number_of_configs);
extern bool ProteusIII_SPI_GPIOLocalReadConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t* number_of_configsP);
extern bool ProteusIII_SPI_GPIOLocalWrite(ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t number_of_controls);
extern bool ProteusIII_SPI_GPIOLocalRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t* number_of_controlsP);

extern bool ProteusIII_SPI_GPIORemoteWriteConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t number_of_configs);
extern bool ProteusIII_SPI_GPIORemoteReadConfig(ProteusIII_SPI_GPIOConfigBlock_t* configP, uint16_t* number_of_configsP);
extern bool ProteusIII_SPI_GPIORemoteWrite(ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t number_of_controls);
extern bool ProteusIII_SPI_GPIORemoteRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIII_SPI_GPIOControlBlock_t* controlP, uint16_t* number_of_controlsP);

extern bool ProteusIII_SPI_Allowunbondedconnections();

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ProteusIII_SPI_FactoryReset();
extern bool ProteusIII_SPI_Set(ProteusIII_SPI_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length);
extern bool ProteusIII_SPI_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength);
extern bool ProteusIII_SPI_SetAdvertisingTimeout(uint16_t advTimeout);
extern bool ProteusIII_SPI_SetCFGFlags(uint16_t cfgflags);
extern bool ProteusIII_SPI_SetConnectionTiming(ProteusIII_SPI_ConnectionTiming_t connectionTiming);
extern bool ProteusIII_SPI_SetScanTiming(ProteusIII_SPI_ScanTiming_t scanTiming);
extern bool ProteusIII_SPI_SetTXPower(ProteusIII_SPI_TXPower_t txpower);
extern bool ProteusIII_SPI_SetSecFlags(ProteusIII_SPI_SecFlags_t secflags);
extern bool ProteusIII_SPI_SetSPIConfigIndex(ProteusIII_SPI_Config_t index);
extern bool ProteusIII_SPI_SetStaticPasskey(uint8_t *staticPasskeyP);

/* read the non-volatile settings */
extern bool ProteusIII_SPI_Get(ProteusIII_SPI_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP);
extern bool ProteusIII_SPI_GetFWVersion(uint8_t *versionP);
extern bool ProteusIII_SPI_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP);
extern bool ProteusIII_SPI_GetMAC(uint8_t *MacP);
extern bool ProteusIII_SPI_GetBTMAC(uint8_t *BTMacP);
extern bool ProteusIII_SPI_GetAdvertisingTimeout(uint16_t *advTimeoutP);
extern bool ProteusIII_SPI_GetCFGFlags(uint16_t *cfgflags);
extern bool ProteusIII_SPI_GetConnectionTiming(ProteusIII_SPI_ConnectionTiming_t *connectionTimingP);
extern bool ProteusIII_SPI_GetScanTiming(ProteusIII_SPI_ScanTiming_t *scanTimingP);
extern bool ProteusIII_SPI_GetTXPower(ProteusIII_SPI_TXPower_t *txpowerP);
extern bool ProteusIII_SPI_GetSecFlags(ProteusIII_SPI_SecFlags_t *secflagsP);
extern bool ProteusIII_SPI_GetSPIConfigIndex(ProteusIII_SPI_Config_t *indexP);
extern bool ProteusIII_SPI_GetStaticPasskey(uint8_t *staticPasskeyP);
extern bool ProteusIII_SPI_GetState(ProteusIII_SPI_BLE_Role_t *BLE_roleP, ProteusIII_SPI_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP);

#endif // _ProteusIII_SPI_defined
#ifdef __cplusplus
}
#endif

