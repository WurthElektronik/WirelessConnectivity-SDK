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
 * COPYRIGHT (c) 2020 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <stdbool.h>
#include "../ProteusIII/ProteusIII.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ProteusIIIPlug_defined
#define _ProteusIIIPlug_defined

/* Pins */
#define ProteusIIIPlug_CBUS_RESET PIN_FTDI_CBUS0

/* timings */
#define ProteusIIIPlug_BOOT_DURATION ProteusIII_BOOT_DURATION

#define MAX_NUMBER_OF_DEVICES (uint8_t)10

#define ProteusIIIPlug_CallbackConfig_t ProteusIII_CallbackConfig_t

#define ProteusIIIPlug_Device_t ProteusIII_Device_t
#define ProteusIIIPlug_GetDevices_t ProteusIII_GetDevices_t
#define ProteusIIIPlug_States_t ProteusIII_States_t
#define ProteusIIIPlug_Security_t ProteusIII_Security_t
#define ProteusIIIPlug_BLE_Role_t ProteusIII_BLE_Role_t
#define ProteusIIIPlug_BLE_Action_t ProteusIII_BLE_Action_t

#define ProteusIIIPlug_GPIO_t ProteusIII_GPIO_t
#define ProteusIIIPlug_GPIO_IO_t ProteusIII_GPIO_IO_t
#define ProteusIIIPlug_GPIO_Output_t ProteusIII_GPIO_Output_t
#define ProteusIIIPlug_GPIO_Input_t ProteusIII_GPIO_Input_t
#define ProteusIIIPlug_GPIOConfigBlock_t ProteusIII_GPIOConfigBlock_t
#define ProteusIIIPlug_GPIOControlBlock_t ProteusIII_GPIOControlBlock_t

/* user settings */
#define ProteusIIIPlug_UserSettings_t ProteusIII_UserSettings_t

#define ProteusIIIPlug_SecFlags_t ProteusIII_SecFlags_t
#define ProteusIIIPlug_ConnectionTiming_t ProteusIII_ConnectionTiming_t
#define ProteusIIIPlug_ScanTiming_t ProteusIII_ScanTiming_t
#define ProteusIIIPlug_TXPower_t ProteusIII_TXPower_t
#define ProteusIIIPlug_BaudRate_t ProteusIII_BaudRate_t
#define ProteusIIIPlug_UartParity_t ProteusIII_UartParity_t
#define ProteusIIIPlug_Phy_t ProteusIII_Phy_t

#define ProteusIIIPlug_DisplayPasskeyAction_t ProteusIII_DisplayPasskeyAction_t


extern bool ProteusIIIPlug_Init(int baudrate, int rp, ProteusIIIPlug_CallbackConfig_t callbackConfig);
extern bool ProteusIIIPlug_Deinit(void);

extern bool ProteusIIIPlug_PinReset(void);
extern bool ProteusIIIPlug_Reset(void);

extern bool ProteusIIIPlug_Sleep();
extern bool ProteusIIIPlug_PinWakeup();

extern bool ProteusIIIPlug_Connect(uint8_t *btmacP);
extern bool ProteusIIIPlug_Disconnect();

extern bool ProteusIIIPlug_Scanstart();
extern bool ProteusIIIPlug_Scanstop();
extern bool ProteusIIIPlug_GetDevices(ProteusIIIPlug_GetDevices_t* devicesP);

extern bool ProteusIIIPlug_Transmit(uint8_t* PayloadP, uint16_t length);

extern bool ProteusIIIPlug_Passkey(uint8_t* passkeyP);
extern bool ProteusIIIPlug_NumericCompareConfirm(bool keyIsOk);

extern bool ProteusIIIPlug_PhyUpdate(ProteusIIIPlug_Phy_t phy);

extern ProteusIIIPlug_States_t ProteusIIIPlug_GetDriverState();

/* functions to control the GPIO feature */
extern bool ProteusIIIPlug_GPIOLocalWriteConfig(ProteusIIIPlug_GPIOConfigBlock_t* configP, uint16_t configLength);
extern bool ProteusIIIPlug_GPIOLocalReadConfig(ProteusIIIPlug_GPIOConfigBlock_t* configP, uint16_t* configLengthP);
extern bool ProteusIIIPlug_GPIOLocalWrite(ProteusIIIPlug_GPIOControlBlock_t* controlP, uint16_t controlLength);
extern bool ProteusIIIPlug_GPIOLocalRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIIIPlug_GPIOControlBlock_t* controlP, uint16_t* controlLengthP);

extern bool ProteusIIIPlug_GPIORemoteWriteConfig(ProteusIIIPlug_GPIOConfigBlock_t* configP, uint16_t configLength);
extern bool ProteusIIIPlug_GPIORemoteReadConfig(ProteusIIIPlug_GPIOConfigBlock_t* configP, uint16_t* configLengthP);
extern bool ProteusIIIPlug_GPIORemoteWrite(ProteusIIIPlug_GPIOControlBlock_t* controlP, uint16_t controlLength);
extern bool ProteusIIIPlug_GPIORemoteRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIIIPlug_GPIOControlBlock_t* controlP, uint16_t* controlLengthP);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ProteusIIIPlug_FactoryReset();
extern bool ProteusIIIPlug_Set(ProteusIIIPlug_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length);
extern bool ProteusIIIPlug_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength);
extern bool ProteusIIIPlug_SetAdvertisingTimeout(uint16_t advTimeout);
extern bool ProteusIIIPlug_SetCFGFlags(uint16_t cfgflags);
extern bool ProteusIIIPlug_SetConnectionTiming(ProteusIIIPlug_ConnectionTiming_t connectionTiming);
extern bool ProteusIIIPlug_SetScanTiming(ProteusIIIPlug_ScanTiming_t scanTiming);
extern bool ProteusIIIPlug_SetTXPower(ProteusIIIPlug_TXPower_t txpower);
extern bool ProteusIIIPlug_SetSecFlags(ProteusIIIPlug_SecFlags_t secflags);
extern bool ProteusIIIPlug_SetBaudrateIndex(ProteusIIIPlug_BaudRate_t baudrate, ProteusIIIPlug_UartParity_t parity, bool flowcontrolEnable);
extern bool ProteusIIIPlug_SetStaticPasskey(uint8_t *staticPasskeyP);

/* read the non-volatile settings */
extern bool ProteusIIIPlug_Get(ProteusIIIPlug_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP);
extern bool ProteusIIIPlug_GetFWVersion(uint8_t *versionP);
extern bool ProteusIIIPlug_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP);
extern bool ProteusIIIPlug_GetMAC(uint8_t *MacP);
extern bool ProteusIIIPlug_GetBTMAC(uint8_t *BTMacP);
extern bool ProteusIIIPlug_GetAdvertisingTimeout(uint16_t *advTimeoutP);
extern bool ProteusIIIPlug_GetCFGFlags(uint16_t *cfgflags);
extern bool ProteusIIIPlug_GetConnectionTiming(ProteusIIIPlug_ConnectionTiming_t *connectionTimingP);
extern bool ProteusIIIPlug_GetScanTiming(ProteusIIIPlug_ScanTiming_t *scanTimingP);
extern bool ProteusIIIPlug_GetTXPower(ProteusIIIPlug_TXPower_t *txpowerP);
extern bool ProteusIIIPlug_GetSecFlags(ProteusIIIPlug_SecFlags_t *secflagsP);
extern bool ProteusIIIPlug_GetBaudrateIndex(ProteusIIIPlug_BaudRate_t *baudrateP, ProteusIIIPlug_UartParity_t *parityP, bool *flowcontrolEnableP);
extern bool ProteusIIIPlug_GetStaticPasskey(uint8_t *staticPasskeyP);
extern bool ProteusIIIPlug_GetState(ProteusIIIPlug_BLE_Role_t *BLE_roleP, ProteusIIIPlug_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP);

#endif // _ProteusIIIPlug_defined
#ifdef __cplusplus
}
#endif

