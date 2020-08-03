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
#include "../ThyoneI/ThyoneI.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ThyoneIPlug_defined
#define _ThyoneIPlug_defined

/* Pins */
#define ThyoneIPlug_CBUS_RESET PIN_FTDI_CBUS0

/* timings */
#define ThyoneIPlug_BOOT_DURATION ThyoneI_BOOT_DURATION

#define ThyoneIPlug_ResetReason_t    ThyoneI_ResetReason_t
#define ThyoneIPlug_OperatingMode_t  ThyoneI_OperatingMode_t
#define ThyoneIPlug_States_t         ThyoneI_States_t
#define ThyoneIPlug_Profile_t        ThyoneI_Profile_t
#define ThyoneIPlug_AddressMode_t    ThyoneI_AddressMode_t
#define ThyoneIPlug_EncryptionMode_t ThyoneI_EncryptionMode_t

#define ThyoneIPlug_AMOUNT_GPIO_PINS   ThyoneI_AMOUNT_GPIO_PINS
#define ThyoneIPlug_GPIO_t             ThyoneI_GPIO_t
#define ThyoneIPlug_GPIO_IO_t          ThyoneI_GPIO_IO_t
#define ThyoneIPlug_GPIO_Output_t      ThyoneI_GPIO_Output_t
#define ThyoneIPlug_GPIO_Input_t       ThyoneI_GPIO_Input_t
#define ThyoneIPlug_GPIOConfigBlock_t  ThyoneI_GPIOConfigBlock_t
#define ThyoneIPlug_GPIOControlBlock_t ThyoneI_GPIOControlBlock_t

/* user settings */
#define ThyoneIPlug_UserSettings_t ThyoneI_UserSettings_t

#define ThyoneIPlug_TXPower_t       ThyoneI_TXPower_t
#define ThyoneIPlug_BaudRateIndex_t ThyoneI_BaudRateIndex_t
#define ThyoneIPlug_UartParity_t    ThyoneI_UartParity_t


extern bool ThyoneIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint16_t,uint32_t,int8_t));
extern bool ThyoneIPlug_Deinit(void);

extern bool ThyoneIPlug_PinReset(void);
extern bool ThyoneIPlug_Reset(void);

extern bool ThyoneIPlug_TransmitBroadcast(uint8_t* payloadP, uint16_t length);
extern bool ThyoneIPlug_TransmitMulticast(uint8_t* payloadP, uint16_t length);
extern bool ThyoneIPlug_TransmitUnicast(uint8_t* payloadP, uint16_t length);

extern bool ThyoneIPlug_TransmitMulticastExtended(uint8_t groupID, uint8_t* payloadP, uint16_t length);
extern bool ThyoneIPlug_TransmitUnicastExtended(uint32_t address, uint8_t* payloadP, uint16_t length);

extern bool ThyoneIPlug_GPIORemoteSetConfig(uint32_t destAddress, ThyoneIPlug_GPIOConfigBlock_t* configP, uint16_t configLength);
extern bool ThyoneIPlug_GPIORemoteGetConfig(uint32_t destAddress, ThyoneIPlug_GPIOConfigBlock_t* configP, uint16_t* configLength);
extern bool ThyoneIPlug_GPIORemoteWrite(uint32_t destAddress, ThyoneIPlug_GPIOControlBlock_t* controlP, uint16_t controlLength);
extern bool ThyoneIPlug_GPIORemoteRead(uint32_t destAddress, uint8_t *GPIOToRead, uint8_t amountGPIOToRead, ThyoneIPlug_GPIOControlBlock_t* controlP, uint16_t* controlLength);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ThyoneIPlug_FactoryReset();
extern bool ThyoneIPlug_Set(ThyoneIPlug_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length);
extern bool ThyoneIPlug_SetBaudrateIndex(ThyoneIPlug_BaudRateIndex_t baudrate, ThyoneIPlug_UartParity_t parity, bool flowcontrolEnable);
extern bool ThyoneIPlug_SetEncryptionMode(ThyoneIPlug_EncryptionMode_t encryptionMode);
extern bool ThyoneIPlug_SetRfProfile(ThyoneIPlug_Profile_t profile);
extern bool ThyoneIPlug_SetRFChannel(uint8_t channel);
extern bool ThyoneIPlug_SetNumRetries(uint8_t numRetries);
extern bool ThyoneIPlug_SetTXPower(ThyoneIPlug_TXPower_t txpower);
extern bool ThyoneIPlug_SetRpNumSlots(uint8_t numSlots);
extern bool ThyoneIPlug_SetSourceAddress(uint32_t sourceAddress);
extern bool ThyoneIPlug_SetDestinationAddress(uint32_t destinationAddress);
extern bool ThyoneIPlug_SetGroupID(uint8_t groupId);
extern bool ThyoneIPlug_SetEncryptionKey(uint8_t *keyP);
extern bool ThyoneIPlug_SetTimeToLive(uint8_t ttl);
extern bool ThyoneIPlug_SetCCAMode(uint8_t ccaMode);
extern bool ThyoneIPlug_SetCCAThreshold(uint8_t ccaThreshold);
extern bool ThyoneIPlug_SetGPIOBlockRemoteConfig(uint8_t remoteConfig);
extern bool ThyoneIPlug_SetModuleMode(ThyoneIPlug_OperatingMode_t moduleMode);

/* read the non-volatile settings */
extern bool ThyoneIPlug_Get(ThyoneIPlug_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP);
extern bool ThyoneIPlug_GetSerialNumber(uint8_t *serialNumberP);
extern bool ThyoneIPlug_GetFWVersion(uint8_t *versionP);
extern bool ThyoneIPlug_GetBaudrateIndex(ThyoneIPlug_BaudRateIndex_t *baudrate, ThyoneIPlug_UartParity_t *parity, bool *flowcontrolEnable);
extern bool ThyoneIPlug_GetEncryptionMode(ThyoneIPlug_EncryptionMode_t *encryptionModeP);
extern bool ThyoneIPlug_GetRfProfile(ThyoneIPlug_Profile_t *profileP);
extern bool ThyoneIPlug_GetRFChannel(uint8_t *channelP);
extern bool ThyoneIPlug_GetNumRetries(uint8_t *numRetriesP);
extern bool ThyoneIPlug_GetTXPower(ThyoneIPlug_TXPower_t *txpowerP);
extern bool ThyoneIPlug_GetRpNumSlots(uint8_t *numSlotsP);
extern bool ThyoneIPlug_GetSourceAddress(uint32_t *sourceAddressP);
extern bool ThyoneIPlug_GetDestinationAddress(uint32_t *destinationAddressP);
extern bool ThyoneIPlug_GetGroupID(uint8_t *groupIdP);
extern bool ThyoneIPlug_GetTimeToLive(uint8_t *ttlP);
extern bool ThyoneIPlug_GetCCAMode(uint8_t *ccaModeP);
extern bool ThyoneIPlug_GetCCAThreshold(uint8_t *ccaThresholdP);
extern bool ThyoneIPlug_GetGPIOBlockRemoteConfig(uint8_t *remoteConfigP);
extern bool ThyoneIPlug_GetModuleMode(ThyoneIPlug_OperatingMode_t *moduleModeP);
#endif // _ThyoneIPlug_defined
#ifdef __cplusplus
}
#endif

