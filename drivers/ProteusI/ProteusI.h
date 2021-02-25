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
 * COPYRIGHT (c) 2019 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ProteusI_defined
#define _ProteusI_defined

/* Pins */
#ifdef libgpiod_numbers
#define ProteusI_PIN_RESET  4   // BCM_GPIO 4
#define ProteusI_PIN_WAKEUP 17  // BCM_GPIO 17
#define ProteusI_PIN_BOOT   27  // BCM_GPIO 27
#elif defined wiringPi_numbers
#define ProteusI_PIN_RESET  7  // wiringPi (pin 7 - BCM_GPIO 4
#define ProteusI_PIN_WAKEUP 0  // wiringPi (pin 0 - BCM_GPIO 17
#define ProteusI_PIN_BOOT   2  // wiringPi (pin 2 - BCM_GPIO 27
#else
#warning "No pins defined. Can be ignored for plug projects"
#endif

/* timings */
#define ProteusI_BOOT_DURATION (uint16_t)75

typedef struct ProteusI_Device_t {
    uint8_t btmac[6];
    int8_t rssi;
    int8_t txpower;
    uint8_t devicenamelength;
    uint8_t devicename[32];
} ProteusI_Device_t;

#define MAX_NUMBER_OF_DEVICES (uint8_t)10
typedef struct ProteusI_GetDevices_t {
    uint8_t numberofdevices;
    ProteusI_Device_t devices[MAX_NUMBER_OF_DEVICES];
} ProteusI_GetDevices_t;

typedef enum ProteusI_States_t {
    ProteusI_State_BLE_Invalid =         (uint8_t)0x00,
    ProteusI_State_BLE_Connected =       (uint8_t)0x01,
    ProteusI_State_BLE_Channel_Open =    (uint8_t)0x02,
    ProteusI_State_Uart_Open =           (uint8_t)0x04,
} ProteusI_States_t;

typedef enum ProteusI_Security_t {
    ProteusI_State_BLE_ReBonded = (uint8_t)0x00,
    ProteusI_State_BLE_Bonded =   (uint8_t)0x01,
    ProteusI_State_BLE_Paired =   (uint8_t)0x02,
} ProteusI_Security_t;

typedef enum ProteusI_BLE_Role_t {
    ProteusI_BLE_Role_None =         (uint8_t)0x00,
    ProteusI_BLE_Role_Pheripheral =  (uint8_t)0x01,
    ProteusI_BLE_Role_Central =      (uint8_t)0x02,
    ProteusI_BLE_Role_DTM =          (uint8_t)0x10,
} ProteusI_BLE_Role_t;

typedef enum ProteusI_BLE_Action_t {
    ProteusI_BLE_Action_None =       (uint8_t)0x00,
    ProteusI_BLE_Action_Idle =       (uint8_t)0x01,
    ProteusI_BLE_Action_Scanning =   (uint8_t)0x02,
    ProteusI_BLE_Action_Connected =  (uint8_t)0x03,
    ProteusI_BLE_Action_Sleep =      (uint8_t)0x04,
    ProteusI_BLE_Action_DTM =        (uint8_t)0x05,
} ProteusI_BLE_Action_t;


/* user settings */
typedef enum ProteusI_UserSettings_t {
    ProteusI_USERSETTING_POSITION_FS_FWVersion =             (uint8_t)0x01,
    ProteusI_USERSETTING_POSITION_RF_DEVICE_NAME =           (uint8_t)0x02,
    ProteusI_USERSETTING_POSITION_FS_MAC =                   (uint8_t)0x03,
    ProteusI_USERSETTING_POSITION_FS_BTMAC =                 (uint8_t)0x04,
    ProteusI_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT =   (uint8_t)0x07,
    ProteusI_USERSETTING_POSITION_RF_CONNECTION_TIMING =     (uint8_t)0x08,
    ProteusI_USERSETTING_POSITION_RF_SCAN_TIMING =           (uint8_t)0x09,
    ProteusI_USERSETTING_POSITION_RF_SCAN_FACTOR =           (uint8_t)0x0A,
    ProteusI_USERSETTING_POSITION_UART_BAUDRATE_INDEX =      (uint8_t)0x0B,
    ProteusI_USERSETTING_POSITION_RF_SEC_FLAGS =             (uint8_t)0x0C,
    ProteusI_USERSETTING_POSITION_RF_SCAN_FLAGS =            (uint8_t)0x0D,
    ProteusI_USERSETTING_POSITION_RF_BEACON_FLAGS =          (uint8_t)0x0E,
    ProteusI_USERSETTING_POSITION_FS_DEVICE_INFO =           (uint8_t)0x0F,
    ProteusI_USERSETTING_POSITION_FS_SERIAL_NUMBER =         (uint8_t)0x10,
    ProteusI_USERSETTING_POSITION_RF_TX_POWER =              (uint8_t)0x11,
    ProteusI_USERSETTING_POSITION_RF_STATIC_PASSKEY =        (uint8_t)0x12,
} ProteusI_UserSettings_t;

#define SEC_MODE_BONDING_ENABLE_MASK (uint8_t)0x04
typedef enum ProteusI_SecFlags_t {
    ProteusI_SecFlags_NONE =                     (uint8_t)0x00,
    ProteusI_SecFlags_JustWorks =                (uint8_t)0x02,
    ProteusI_SecFlags_StaticPassKey =            (uint8_t)0x03,
    ProteusI_SecFlags_JustWorks_Bonding =        (uint8_t)(ProteusI_SecFlags_JustWorks | SEC_MODE_BONDING_ENABLE_MASK),
    ProteusI_SecFlags_StaticPassKey_Bonding =    (uint8_t)(ProteusI_SecFlags_StaticPassKey | SEC_MODE_BONDING_ENABLE_MASK),
} ProteusI_SecFlags_t;

typedef enum ProteusI_ConnectionTiming_t {
    ProteusI_ConnectionTiming_0 = (uint8_t)0x00,
    ProteusI_ConnectionTiming_1 = (uint8_t)0x01,
    ProteusI_ConnectionTiming_2 = (uint8_t)0x02,
    ProteusI_ConnectionTiming_3 = (uint8_t)0x03,
    ProteusI_ConnectionTiming_4 = (uint8_t)0x04,
    ProteusI_ConnectionTiming_5 = (uint8_t)0x05,
    ProteusI_ConnectionTiming_6 = (uint8_t)0x06

} ProteusI_ConnectionTiming_t;

typedef enum ProteusI_ScanTiming_t {
    ProteusI_ScanTiming_0 = (uint8_t)0x00,
    ProteusI_ScanTiming_1 = (uint8_t)0x01,
    ProteusI_ScanTiming_2 = (uint8_t)0x02,
    ProteusI_ScanTiming_3 = (uint8_t)0x03,
    ProteusI_ScanTiming_4 = (uint8_t)0x04,
    ProteusI_ScanTiming_5 = (uint8_t)0x05

} ProteusI_ScanTiming_t;

typedef enum ProteusI_TXPower_t {
    ProteusI_TXPower_4       = (int8_t) 4,
    ProteusI_TXPower_0       = (int8_t) 0,
    ProteusI_TXPower_minus4  = (int8_t)-4,
    ProteusI_TXPower_minus8  = (int8_t)-8,
    ProteusI_TXPower_minus12 = (int8_t)-12,
} ProteusI_TXPower_t;

typedef enum ProteusI_BaudRateIndex_t {
    ProteusI_BaudRateIndex_9600 =     (uint8_t)0x00,
    ProteusI_BaudRateIndex_19200 =    (uint8_t)0x01,
    ProteusI_BaudRateIndex_38400 =    (uint8_t)0x02,
    ProteusI_BaudRateIndex_115200 =   (uint8_t)0x03,
    ProteusI_BaudRateIndex_230400 =   (uint8_t)0x04
} ProteusI_BaudRateIndex_t;


extern bool ProteusI_Init(int baudrate, int rp,  int wp, int bp, void(*RXcb)(uint8_t*,uint16_t,uint8_t*,int8_t),void(*Ccb)(uint8_t*),void(*DCcb)(),void(*COcb)(uint8_t*,uint16_t),void(*Scb)(uint8_t*,ProteusI_Security_t),void(*PKcb)(uint8_t*));
extern bool ProteusI_Deinit(void);

extern bool ProteusI_PinReset(void);
extern bool ProteusI_Reset(void);

extern bool ProteusI_Sleep();
extern bool ProteusI_PinWakeup();

extern bool ProteusI_Connect(uint8_t *btmacP);
extern bool ProteusI_Disconnect();

extern bool ProteusI_Scanstart();
extern bool ProteusI_Scanstop();
extern bool ProteusI_GetDevices(ProteusI_GetDevices_t* devicesP);

extern bool ProteusI_Transmit(uint8_t* PayloadP, uint16_t length);

extern bool ProteusI_Passkey(uint8_t* passkeyP);

extern ProteusI_States_t ProteusI_GetDriverState();

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ProteusI_FactoryReset();
extern bool ProteusI_Set(ProteusI_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length);
extern bool ProteusI_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength);
extern bool ProteusI_SetAdvertisingTimeout(uint16_t advTimeout);
extern bool ProteusI_SetConnectionTiming(ProteusI_ConnectionTiming_t connectionTiming);
extern bool ProteusI_SetScanTiming(ProteusI_ScanTiming_t scanTiming);
extern bool ProteusI_SetTXPower(ProteusI_TXPower_t txpower);
extern bool ProteusI_SetSecFlags(ProteusI_SecFlags_t secflags);
extern bool ProteusI_SetBaudrateIndex(ProteusI_BaudRateIndex_t baudrateIndex);
extern bool ProteusI_SetStaticPasskey(uint8_t *staticPasskeyP);

/* read the non-volatile settings */
extern bool ProteusI_Get(ProteusI_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP);
extern bool ProteusI_GetFWVersion(uint8_t *versionP);
extern bool ProteusI_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP);
extern bool ProteusI_GetMAC(uint8_t *MacP);
extern bool ProteusI_GetBTMAC(uint8_t *MacP);
extern bool ProteusI_GetAdvertisingTimeout(uint16_t *advTimeoutP);
extern bool ProteusI_GetConnectionTiming(ProteusI_ConnectionTiming_t *connectionTimingP);
extern bool ProteusI_GetScanTiming(ProteusI_ScanTiming_t *scanTimingP);
extern bool ProteusI_GetTXPower(ProteusI_TXPower_t *txpowerP);
extern bool ProteusI_GetSecFlags(ProteusI_SecFlags_t *secflagsP);
extern bool ProteusI_GetBaudrateIndex(ProteusI_BaudRateIndex_t *baudrateIndexP);
extern bool ProteusI_GetStaticPasskey(uint8_t *staticPasskeyP);
extern bool ProteusI_GetState(ProteusI_BLE_Role_t *BLE_roleP, ProteusI_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP);

#endif // _ProteusI_defined
#ifdef __cplusplus
}
#endif
