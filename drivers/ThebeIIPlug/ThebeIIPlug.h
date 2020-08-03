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

#include "../../drivers/ThebeII/ThebeII.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pins */
#define ThebeIIPlug_CBUS_RESET PIN_FTDI_CBUS0

#define ThebeIIPlug_BROADCASTADDRESS 0xFF

#define ThebeIIPlug_AddressMode_t                       ThebeII_AddressMode_t
#define ThebeIIPlug_UserSettings_t                      ThebeII_UserSettings_t
#define ThebeIIPlug_Configuration_t                     ThebeII_Configuration_t

#define ThebeIIPlug_CMD_SETGET_OPTION_FULLACCESS            ThebeII_CMD_SETGET_OPTION_FULLACCESS
#define ThebeIIPlug_CMD_SETGET_OPTION_UARTBAUDRATE          ThebeII_CMD_SETGET_OPTION_UARTBAUDRATE
#define ThebeIIPlug_CMD_SETGET_OPTION_DEFAULTRFPROFILE      ThebeII_CMD_SETGET_OPTION_DEFAULTRFPROFILE
#define ThebeIIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER      ThebeII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER
#define ThebeIIPlug_CMD_SETGET_OPTION_DEFAULTRFCHANNEL      ThebeII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL
#define ThebeIIPlug_CMD_SETGET_OPTION_DEFAULTADDRESSMODE    ThebeII_CMD_SETGET_OPTION_DEFAULTADDRESSMODE
#define ThebeIIPlug_CMD_SETGET_OPTION_NUMRETRYS             ThebeII_CMD_SETGET_OPTION_NUMRETRYS
#define ThebeIIPlug_CMD_SETGET_OPTION_DEFAULTDESTNETID      ThebeII_CMD_SETGET_OPTION_DEFAULTDESTNETID
#define ThebeIIPlug_CMD_SETGET_OPTION_DEFAULTDESTADDR       ThebeII_CMD_SETGET_OPTION_DEFAULTDESTADDR
#define ThebeIIPlug_CMD_SETGET_OPTION_SOURCENETID           ThebeII_CMD_SETGET_OPTION_SOURCENETID
#define ThebeIIPlug_CMD_SETGET_OPTION_SOURCEADDR            ThebeII_CMD_SETGET_OPTION_SOURCEADDR
#define ThebeIIPlug_CMD_SETGET_OPTION_OP_MODE               ThebeII_CMD_SETGET_OPTION_OP_MODE
#define ThebeIIPlug_CMD_SETGET_OPTION_CFG_FLAGS             ThebeII_CMD_SETGET_OPTION_CFG_FLAGS
#define ThebeIIPlug_CMD_SETGET_OPTION_RP_FLAGS              ThebeII_CMD_SETGET_OPTION_RP_FLAGS
#define ThebeIIPlug_CMD_SETGET_OPTION_RP_NUMSLOTS           ThebeII_CMD_SETGET_OPTION_RP_NUMSLOTS
#define ThebeIIPlug_CMD_SETGET_OPTION_FACTORYSETTINGS       ThebeII_CMD_SETGET_OPTION_FACTORYSETTINGS
#define ThebeIIPlug_CMD_SETGET_OPTION_FWVERSION             ThebeII_CMD_SETGET_OPTION_FWVERSION
#define ThebeIIPlug_CMD_SETGET_OPTION_RUNTIMESETTINGS       ThebeII_CMD_SETGET_OPTION_RUNTIMESETTINGS


extern bool ThebeIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThebeII_AddressMode_t addrmode);
extern bool ThebeIIPlug_Deinit(void);

extern bool ThebeIIPlug_PinReset(void);
extern bool ThebeIIPlug_Reset(void);

extern bool ThebeIIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb);
extern bool ThebeIIPlug_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool ThebeIIPlug_Get(ThebeIIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool ThebeIIPlug_GetFirmwareVersion(uint8_t* fw);
extern bool ThebeIIPlug_GetSerialNumber(uint8_t* sn);
extern bool ThebeIIPlug_GetDefaultTXPower(uint8_t* txpower);
extern bool ThebeIIPlug_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* srcaddr_msb);
extern bool ThebeIIPlug_GetSourceNetID(uint8_t* srcnetid);
extern bool ThebeIIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* srcaddr_msb);
extern bool ThebeIIPlug_GetDefaultDestNetID(uint8_t* destnetid);
extern bool ThebeIIPlug_GetDefaultRFChannel(uint8_t* channel);
extern bool ThebeIIPlug_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ThebeIIPlug_FactoryReset(void);
extern bool ThebeIIPlug_Set(ThebeIIPlug_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool ThebeIIPlug_Configure(ThebeIIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool ThebeIIPlug_SetDefaultTXPower(uint8_t txpower);
extern bool ThebeIIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool ThebeIIPlug_SetSourceNetID(uint8_t srcnetid);
extern bool ThebeIIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool ThebeIIPlug_SetDefaultDestNetID(uint8_t destnetid);
extern bool ThebeIIPlug_SetDefaultRFChannel(uint8_t channel);
extern bool ThebeIIPlug_SetDefaultRFProfile(uint8_t profile);
extern bool ThebeIIPlug_EnableSnifferMode();

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool ThebeIIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool ThebeIIPlug_SetVolatile_DestNetID(uint8_t destnetid);
extern bool ThebeIIPlug_SetVolatile_TXPower(uint8_t power);
extern bool ThebeIIPlug_SetVolatile_Channel(uint8_t channel);

extern bool ThebeIIPlug_SendUART(uint8_t* payload, uint8_t length, uint16_t waittime);

#ifdef __cplusplus
}
#endif
