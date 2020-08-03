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


#include <stdint.h>
#include <stdbool.h>

#include "../../drivers/ThemistoI/ThemistoI.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pins */
#define ThemistoIPlug_CBUS_RESET PIN_FTDI_CBUS0

#define ThemistoIPlug_BROADCASTADDRESS 0xFF

#define ThemistoIPlug_AddressMode_t                       ThemistoI_AddressMode_t
#define ThemistoIPlug_UserSettings_t                      ThemistoI_UserSettings_t
#define ThemistoIPlug_Configuration_t                     ThemistoI_Configuration_t

#define ThemistoIPlug_CMD_SETGET_OPTION_FULLACCESS            ThemistoI_CMD_SETGET_OPTION_FULLACCESS
#define ThemistoIPlug_CMD_SETGET_OPTION_UARTBAUDRATE          ThemistoI_CMD_SETGET_OPTION_UARTBAUDRATE
#define ThemistoIPlug_CMD_SETGET_OPTION_DEFAULTRFPROFILE      ThemistoI_CMD_SETGET_OPTION_DEFAULTRFPROFILE
#define ThemistoIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER      ThemistoI_CMD_SETGET_OPTION_DEFAULTRFTXPOWER
#define ThemistoIPlug_CMD_SETGET_OPTION_DEFAULTRFCHANNEL      ThemistoI_CMD_SETGET_OPTION_DEFAULTRFCHANNEL
#define ThemistoIPlug_CMD_SETGET_OPTION_DEFAULTADDRESSMODE    ThemistoI_CMD_SETGET_OPTION_DEFAULTADDRESSMODE
#define ThemistoIPlug_CMD_SETGET_OPTION_NUMRETRYS             ThemistoI_CMD_SETGET_OPTION_NUMRETRYS
#define ThemistoIPlug_CMD_SETGET_OPTION_DEFAULTDESTNETID      ThemistoI_CMD_SETGET_OPTION_DEFAULTDESTNETID
#define ThemistoIPlug_CMD_SETGET_OPTION_DEFAULTDESTADDR       ThemistoI_CMD_SETGET_OPTION_DEFAULTDESTADDR
#define ThemistoIPlug_CMD_SETGET_OPTION_SOURCENETID           ThemistoI_CMD_SETGET_OPTION_SOURCENETID
#define ThemistoIPlug_CMD_SETGET_OPTION_SOURCEADDR            ThemistoI_CMD_SETGET_OPTION_SOURCEADDR
#define ThemistoIPlug_CMD_SETGET_OPTION_OP_MODE               ThemistoI_CMD_SETGET_OPTION_OP_MODE
#define ThemistoIPlug_CMD_SETGET_OPTION_CFG_FLAGS             ThemistoI_CMD_SETGET_OPTION_CFG_FLAGS
#define ThemistoIPlug_CMD_SETGET_OPTION_RP_FLAGS              ThemistoI_CMD_SETGET_OPTION_RP_FLAGS
#define ThemistoIPlug_CMD_SETGET_OPTION_RP_NUMSLOTS           ThemistoI_CMD_SETGET_OPTION_RP_NUMSLOTS
#define ThemistoIPlug_CMD_SETGET_OPTION_FACTORYSETTINGS       ThemistoI_CMD_SETGET_OPTION_FACTORYSETTINGS
#define ThemistoIPlug_CMD_SETGET_OPTION_FWVERSION             ThemistoI_CMD_SETGET_OPTION_FWVERSION
#define ThemistoIPlug_CMD_SETGET_OPTION_RUNTIMESETTINGS       ThemistoI_CMD_SETGET_OPTION_RUNTIMESETTINGS


extern bool ThemistoIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThemistoI_AddressMode_t addrmode);
extern bool ThemistoIPlug_Deinit(void);

extern bool ThemistoIPlug_PinReset(void);
extern bool ThemistoIPlug_Reset(void);

extern bool ThemistoIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb);
extern bool ThemistoIPlug_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool ThemistoIPlug_Get(ThemistoIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool ThemistoIPlug_GetFirmwareVersion(uint8_t* fw);
extern bool ThemistoIPlug_GetSerialNumber(uint8_t* sn);
extern bool ThemistoIPlug_GetDefaultTXPower(uint8_t* txpower);
extern bool ThemistoIPlug_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* srcaddr_msb);
extern bool ThemistoIPlug_GetSourceNetID(uint8_t* srcnetid);
extern bool ThemistoIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* srcaddr_msb);
extern bool ThemistoIPlug_GetDefaultDestNetID(uint8_t* destnetid);
extern bool ThemistoIPlug_GetDefaultRFChannel(uint8_t* channel);
extern bool ThemistoIPlug_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ThemistoIPlug_FactoryReset(void);
extern bool ThemistoIPlug_Set(ThemistoIPlug_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool ThemistoIPlug_Configure(ThemistoIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool ThemistoIPlug_SetDefaultTXPower(uint8_t txpower);
extern bool ThemistoIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool ThemistoIPlug_SetSourceNetID(uint8_t srcnetid);
extern bool ThemistoIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool ThemistoIPlug_SetDefaultDestNetID(uint8_t destnetid);
extern bool ThemistoIPlug_SetDefaultRFChannel(uint8_t channel);
extern bool ThemistoIPlug_SetDefaultRFProfile(uint8_t profile);
extern bool ThemistoIPlug_EnableSnifferMode();

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool ThemistoIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool ThemistoIPlug_SetVolatile_DestNetID(uint8_t destnetid);
extern bool ThemistoIPlug_SetVolatile_TXPower(uint8_t power);
extern bool ThemistoIPlug_SetVolatile_Channel(uint8_t channel);

extern bool ThemistoIPlug_SendUART(uint8_t* payload, uint8_t length, uint16_t waittime);

#ifdef __cplusplus
}
#endif
