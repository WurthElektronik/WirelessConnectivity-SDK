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
#include "../ProteusII/ProteusII.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ProteusIIPlug_defined
#define _ProteusIIPlug_defined

/* Pins */
#define ProteusIIPlug_CBUS_RESET PIN_FTDI_CBUS0

/* timings */
#define ProteusIIPlug_BOOT_DURATION ProteusII_BOOT_DURATION

#define MAX_NUMBER_OF_DEVICES (uint8_t)10

#define ProteusIIPlug_Device_t ProteusII_Device_t
#define ProteusIIPlug_GetDevices_t ProteusII_GetDevices_t
#define ProteusIIPlug_States_t ProteusII_States_t
#define ProteusIIPlug_Security_t ProteusII_Security_t
#define ProteusIIPlug_BLE_Role_t ProteusII_BLE_Role_t
#define ProteusIIPlug_BLE_Action_t ProteusII_BLE_Action_t


/* user settings */
#define ProteusIIPlug_UserSettings_t ProteusII_UserSettings_t

#define ProteusIIPlug_SecFlags_t ProteusII_SecFlags_t
#define ProteusIIPlug_ConnectionTiming_t ProteusII_ConnectionTiming_t
#define ProteusIIPlug_ScanTiming_t ProteusII_ScanTiming_t
#define ProteusIIPlug_TXPower_t ProteusII_TXPower_t
#define ProteusIIPlug_BaudRateIndex_t ProteusII_BaudRateIndex_t

extern bool ProteusIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint16_t,uint8_t*,int8_t),void(*Ccb)(uint8_t*),void(*DCcb)(),void(*COcb)(uint8_t*,uint16_t),
                                    void(*Scb)(uint8_t*,ProteusIIPlug_Security_t),void(*PKcb)(uint8_t*),void(*PUcb)(uint8_t*,uint8_t,uint8_t));
extern bool ProteusIIPlug_Deinit(void);

extern bool ProteusIIPlug_PinReset(void);
extern bool ProteusIIPlug_Reset(void);

extern bool ProteusIIPlug_Sleep();
extern bool ProteusIIPlug_PinWakeup();

extern bool ProteusIIPlug_Connect(uint8_t *btmacP);
extern bool ProteusIIPlug_Disconnect();

extern bool ProteusIIPlug_Scanstart();
extern bool ProteusIIPlug_Scanstop();
extern bool ProteusIIPlug_GetDevices(ProteusIIPlug_GetDevices_t* devicesP);

extern bool ProteusIIPlug_Transmit(uint8_t* PayloadP, uint16_t length);

extern bool ProteusIIPlug_Passkey(uint8_t* passkeyP);

extern bool ProteusIIPlug_PhyUpdate(uint8_t phy);

extern ProteusIIPlug_States_t ProteusIIPlug_GetDriverState();

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ProteusIIPlug_FactoryReset();
extern bool ProteusIIPlug_Set(ProteusIIPlug_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length);
extern bool ProteusIIPlug_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength);
extern bool ProteusIIPlug_SetAdvertisingTimeout(uint16_t advTimeout);
extern bool ProteusIIPlug_SetCFGFlags(uint16_t cfgflags);
extern bool ProteusIIPlug_SetConnectionTiming(ProteusIIPlug_ConnectionTiming_t connectionTiming);
extern bool ProteusIIPlug_SetScanTiming(ProteusIIPlug_ScanTiming_t scanTiming);
extern bool ProteusIIPlug_SetTXPower(ProteusIIPlug_TXPower_t txpower);
extern bool ProteusIIPlug_SetSecFlags(ProteusIIPlug_SecFlags_t secflags);
extern bool ProteusIIPlug_SetBaudrateIndex(ProteusIIPlug_BaudRateIndex_t baudrateIndex);
extern bool ProteusIIPlug_SetStaticPasskey(uint8_t *staticPasskeyP);

/* read the non-volatile settings */
extern bool ProteusIIPlug_Get(ProteusIIPlug_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP);
extern bool ProteusIIPlug_GetFWVersion(uint8_t *versionP);
extern bool ProteusIIPlug_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP);
extern bool ProteusIIPlug_GetMAC(uint8_t *MacP);
extern bool ProteusIIPlug_GetBTMAC(uint8_t *MacP);
extern bool ProteusIIPlug_GetAdvertisingTimeout(uint16_t *advTimeoutP);
extern bool ProteusIIPlug_GetCFGFlags(uint16_t *cfgflags);
extern bool ProteusIIPlug_GetConnectionTiming(ProteusIIPlug_ConnectionTiming_t *connectionTimingP);
extern bool ProteusIIPlug_GetScanTiming(ProteusIIPlug_ScanTiming_t *scanTimingP);
extern bool ProteusIIPlug_GetTXPower(ProteusIIPlug_TXPower_t *txpowerP);
extern bool ProteusIIPlug_GetSecFlags(ProteusIIPlug_SecFlags_t *secflagsP);
extern bool ProteusIIPlug_GetBaudrateIndex(ProteusIIPlug_BaudRateIndex_t *baudrateIndexP);
extern bool ProteusIIPlug_GetStaticPasskey(uint8_t *staticPasskeyP);
extern bool ProteusIIPlug_GetState(ProteusIIPlug_BLE_Role_t *BLE_roleP, ProteusIIPlug_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP);

#endif // _ProteusIIPlug_defined
#ifdef __cplusplus
}
#endif
