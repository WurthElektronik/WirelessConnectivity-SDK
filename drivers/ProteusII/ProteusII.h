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

#ifndef _ProteusII_defined
#define _ProteusII_defined

/* Pins */
#define ProteusII_PIN_RESET  7  // LED Pin - wiringPi (pin 7 - BCM_GPIO 4).
#define ProteusII_PIN_WAKEUP 0  // LED Pin - wiringPi (pin 0 - BCM_GPIO 17)
#define ProteusII_PIN_BOOT   2  // LED Pin - wiringPi (pin 2 - BCM_GPIO Rv1)

/* timings */
#define ProteusII_BOOT_DURATION (uint16_t)75

typedef struct ProteusII_Device_t {
    uint8_t btmac[6];
    int8_t rssi;
    int8_t txpower;
    uint8_t devicenamelength;
    uint8_t devicename[32];
} ProteusII_Device_t;

#define MAX_NUMBER_OF_DEVICES (uint8_t)10
typedef struct ProteusII_GetDevices_t {
    uint8_t numberofdevices;
    ProteusII_Device_t devices[MAX_NUMBER_OF_DEVICES];
} ProteusII_GetDevices_t;

typedef enum ProteusII_States_t {
    ProteusII_State_BLE_Invalid =         (uint8_t)0x00,
    ProteusII_State_BLE_Connected =       (uint8_t)0x01,
    ProteusII_State_BLE_Channel_Open =    (uint8_t)0x02,
    ProteusII_State_Uart_Open =           (uint8_t)0x04,
} ProteusII_States_t;

typedef enum ProteusII_Security_t {
    ProteusII_State_BLE_ReBonded = (uint8_t)0x00,
    ProteusII_State_BLE_Bonded =   (uint8_t)0x01,
    ProteusII_State_BLE_Paired =   (uint8_t)0x02,
} ProteusII_Security_t;

typedef enum ProteusII_BLE_Role_t {
    ProteusII_BLE_Role_None =         (uint8_t)0x00,
    ProteusII_BLE_Role_Pheripheral =  (uint8_t)0x01,
    ProteusII_BLE_Role_Central =      (uint8_t)0x02,
    ProteusII_BLE_Role_DTM =          (uint8_t)0x10,
} ProteusII_BLE_Role_t;

typedef enum ProteusII_BLE_Action_t {
    ProteusII_BLE_Action_None =       (uint8_t)0x00,
    ProteusII_BLE_Action_Idle =       (uint8_t)0x01,
    ProteusII_BLE_Action_Scanning =   (uint8_t)0x02,
    ProteusII_BLE_Action_Connected =  (uint8_t)0x03,
    ProteusII_BLE_Action_Sleep =      (uint8_t)0x04,
    ProteusII_BLE_Action_DTM =        (uint8_t)0x05,
} ProteusII_BLE_Action_t;


/* user settings */
typedef enum ProteusII_UserSettings_t {
    ProteusII_USERSETTING_POSITION_FS_FWVersion =             (uint8_t)0x01,
    ProteusII_USERSETTING_POSITION_RF_DEVICE_NAME =           (uint8_t)0x02,
    ProteusII_USERSETTING_POSITION_FS_MAC =                   (uint8_t)0x03,
    ProteusII_USERSETTING_POSITION_FS_BTMAC =                 (uint8_t)0x04,
    ProteusII_USERSETTING_POSITION_RF_ADVERTISING_TIMEOUT =   (uint8_t)0x07,
    ProteusII_USERSETTING_POSITION_RF_CONNECTION_TIMING =     (uint8_t)0x08,
    ProteusII_USERSETTING_POSITION_RF_SCAN_TIMING =           (uint8_t)0x09,
    ProteusII_USERSETTING_POSITION_RF_SCAN_FACTOR =           (uint8_t)0x0A,
    ProteusII_USERSETTING_POSITION_UART_BAUDRATE_INDEX =      (uint8_t)0x0B,
    ProteusII_USERSETTING_POSITION_RF_SEC_FLAGS =             (uint8_t)0x0C,
    ProteusII_USERSETTING_POSITION_RF_SCAN_FLAGS =            (uint8_t)0x0D,
    ProteusII_USERSETTING_POSITION_RF_BEACON_FLAGS =          (uint8_t)0x0E,
    ProteusII_USERSETTING_POSITION_FS_DEVICE_INFO =           (uint8_t)0x0F,
    ProteusII_USERSETTING_POSITION_FS_SERIAL_NUMBER =         (uint8_t)0x10,
    ProteusII_USERSETTING_POSITION_RF_TX_POWER =              (uint8_t)0x11,
    ProteusII_USERSETTING_POSITION_RF_STATIC_PASSKEY =        (uint8_t)0x12,
    ProteusII_USERSETTING_POSITION_RF_CFGFLAGS =              (uint8_t)0x1C,
} ProteusII_UserSettings_t;

#define SEC_MODE_BONDING_ENABLE_MASK (uint8_t)0x04
typedef enum ProteusII_SecFlags_t {
    ProteusII_SecFlags_NONE =                     (uint8_t)0x00,
    ProteusII_SecFlags_JustWorks =                (uint8_t)0x02,
    ProteusII_SecFlags_StaticPassKey =            (uint8_t)0x03,
    ProteusII_SecFlags_JustWorks_Bonding =        (uint8_t)(ProteusII_SecFlags_JustWorks | SEC_MODE_BONDING_ENABLE_MASK),
    ProteusII_SecFlags_StaticPassKey_Bonding =    (uint8_t)(ProteusII_SecFlags_StaticPassKey | SEC_MODE_BONDING_ENABLE_MASK),
} ProteusII_SecFlags_t;

typedef enum ProteusII_ConnectionTiming_t {
    ProteusII_ConnectionTiming_0 = (uint8_t)0x00,
    ProteusII_ConnectionTiming_1 = (uint8_t)0x01,
    ProteusII_ConnectionTiming_2 = (uint8_t)0x02,
    ProteusII_ConnectionTiming_3 = (uint8_t)0x03,
    ProteusII_ConnectionTiming_4 = (uint8_t)0x04,
    ProteusII_ConnectionTiming_5 = (uint8_t)0x05,
    ProteusII_ConnectionTiming_6 = (uint8_t)0x06

} ProteusII_ConnectionTiming_t;

typedef enum ProteusII_ScanTiming_t {
    ProteusII_ScanTiming_0 = (uint8_t)0x00,
    ProteusII_ScanTiming_1 = (uint8_t)0x01,
    ProteusII_ScanTiming_2 = (uint8_t)0x02,
    ProteusII_ScanTiming_3 = (uint8_t)0x03,
    ProteusII_ScanTiming_4 = (uint8_t)0x04,
    ProteusII_ScanTiming_5 = (uint8_t)0x05

} ProteusII_ScanTiming_t;

typedef enum ProteusII_TXPower_t {
    ProteusII_TXPower_4       = (int8_t) 4,
    ProteusII_TXPower_0       = (int8_t) 0,
    ProteusII_TXPower_minus4  = (int8_t)-4,
    ProteusII_TXPower_minus8  = (int8_t)-8,
    ProteusII_TXPower_minus12 = (int8_t)-12,
} ProteusII_TXPower_t;

typedef enum ProteusII_BaudRateIndex_t {
    ProteusII_BaudRateIndex_9600 =     (uint8_t)0x00,
    ProteusII_BaudRateIndex_19200 =    (uint8_t)0x01,
    ProteusII_BaudRateIndex_38400 =    (uint8_t)0x02,
    ProteusII_BaudRateIndex_115200 =   (uint8_t)0x03,
    ProteusII_BaudRateIndex_230400 =   (uint8_t)0x04
} ProteusII_BaudRateIndex_t;


extern bool ProteusII_Init(int baudrate, int rp,  int wp, int bp, void(*RXcb)(uint8_t*,uint16_t,uint8_t*,int8_t),void(*Ccb)(uint8_t*),void(*DCcb)(),void(*COcb)(uint8_t*,uint16_t),void(*Scb)(uint8_t*,ProteusII_Security_t),void(*PKcb)(uint8_t*),void(*PUcb)(uint8_t*,uint8_t,uint8_t));
extern bool ProteusII_Deinit(void);

extern bool ProteusII_PinReset(void);
extern bool ProteusII_Reset(void);

extern bool ProteusII_Sleep();
extern bool ProteusII_PinWakeup();

extern bool ProteusII_Connect(uint8_t *btmacP);
extern bool ProteusII_Disconnect();

extern bool ProteusII_Scanstart();
extern bool ProteusII_Scanstop();
extern bool ProteusII_GetDevices(ProteusII_GetDevices_t* devicesP);

extern bool ProteusII_Transmit(uint8_t* PayloadP, uint16_t length);

extern bool ProteusII_Passkey(uint8_t* passkeyP);

extern bool ProteusII_PhyUpdate(uint8_t phy);

extern ProteusII_States_t ProteusII_GetDriverState();

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ProteusII_FactoryReset();
extern bool ProteusII_Set(ProteusII_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length);
extern bool ProteusII_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength);
extern bool ProteusII_SetAdvertisingTimeout(uint16_t advTimeout);
extern bool ProteusII_SetCFGFlags(uint16_t cfgflags);
extern bool ProteusII_SetConnectionTiming(ProteusII_ConnectionTiming_t connectionTiming);
extern bool ProteusII_SetScanTiming(ProteusII_ScanTiming_t scanTiming);
extern bool ProteusII_SetTXPower(ProteusII_TXPower_t txpower);
extern bool ProteusII_SetSecFlags(ProteusII_SecFlags_t secflags);
extern bool ProteusII_SetBaudrateIndex(ProteusII_BaudRateIndex_t baudrateIndex);
extern bool ProteusII_SetStaticPasskey(uint8_t *staticPasskeyP);

/* read the non-volatile settings */
extern bool ProteusII_Get(ProteusII_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP);
extern bool ProteusII_GetFWVersion(uint8_t *versionP);
extern bool ProteusII_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP);
extern bool ProteusII_GetMAC(uint8_t *MacP);
extern bool ProteusII_GetBTMAC(uint8_t *MacP);
extern bool ProteusII_GetAdvertisingTimeout(uint16_t *advTimeoutP);
extern bool ProteusII_GetCFGFlags(uint16_t *cfgflags);
extern bool ProteusII_GetConnectionTiming(ProteusII_ConnectionTiming_t *connectionTimingP);
extern bool ProteusII_GetScanTiming(ProteusII_ScanTiming_t *scanTimingP);
extern bool ProteusII_GetTXPower(ProteusII_TXPower_t *txpowerP);
extern bool ProteusII_GetSecFlags(ProteusII_SecFlags_t *secflagsP);
extern bool ProteusII_GetBaudrateIndex(ProteusII_BaudRateIndex_t *baudrateIndexP);
extern bool ProteusII_GetStaticPasskey(uint8_t *staticPasskeyP);
extern bool ProteusII_GetState(ProteusII_BLE_Role_t *BLE_roleP, ProteusII_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP);

#endif // _ProteusII_defined
#ifdef __cplusplus
}
#endif
