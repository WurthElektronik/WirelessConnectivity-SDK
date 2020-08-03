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

#include "../../drivers/TarvosIII/TarvosIII.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pins */
#define TarvosIIIPlug_CBUS_RESET PIN_FTDI_CBUS0

#define TarvosIIIPlug_BROADCASTADDRESS 0xFF

#define TarvosIIIPlug_AddressMode_t                       TarvosIII_AddressMode_t
#define TarvosIIIPlug_UserSettings_t                      TarvosIII_UserSettings_t
#define TarvosIIIPlug_Configuration_t                     TarvosIII_Configuration_t

#define TarvosIIIPlug_CMD_SETGET_OPTION_FULLACCESS            TarvosIII_CMD_SETGET_OPTION_FULLACCESS
#define TarvosIIIPlug_CMD_SETGET_OPTION_UARTBAUDRATE          TarvosIII_CMD_SETGET_OPTION_UARTBAUDRATE
#define TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTRFPROFILE      TarvosIII_CMD_SETGET_OPTION_DEFAULTRFPROFILE
#define TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER      TarvosIII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER
#define TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTRFCHANNEL      TarvosIII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL
#define TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTADDRESSMODE    TarvosIII_CMD_SETGET_OPTION_DEFAULTADDRESSMODE
#define TarvosIIIPlug_CMD_SETGET_OPTION_NUMRETRYS             TarvosIII_CMD_SETGET_OPTION_NUMRETRYS
#define TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTDESTNETID      TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTNETID
#define TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTDESTADDR       TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTADDR
#define TarvosIIIPlug_CMD_SETGET_OPTION_SOURCENETID           TarvosIII_CMD_SETGET_OPTION_SOURCENETID
#define TarvosIIIPlug_CMD_SETGET_OPTION_SOURCEADDR            TarvosIII_CMD_SETGET_OPTION_SOURCEADDR
#define TarvosIIIPlug_CMD_SETGET_OPTION_OP_MODE               TarvosIII_CMD_SETGET_OPTION_OP_MODE
#define TarvosIIIPlug_CMD_SETGET_OPTION_CFG_FLAGS             TarvosIII_CMD_SETGET_OPTION_CFG_FLAGS
#define TarvosIIIPlug_CMD_SETGET_OPTION_RP_FLAGS              TarvosIII_CMD_SETGET_OPTION_RP_FLAGS
#define TarvosIIIPlug_CMD_SETGET_OPTION_RP_NUMSLOTS           TarvosIII_CMD_SETGET_OPTION_RP_NUMSLOTS
#define TarvosIIIPlug_CMD_SETGET_OPTION_FACTORYSETTINGS       TarvosIII_CMD_SETGET_OPTION_FACTORYSETTINGS
#define TarvosIIIPlug_CMD_SETGET_OPTION_FWVERSION             TarvosIII_CMD_SETGET_OPTION_FWVERSION
#define TarvosIIIPlug_CMD_SETGET_OPTION_RUNTIMESETTINGS       TarvosIII_CMD_SETGET_OPTION_RUNTIMESETTINGS


extern bool TarvosIIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIII_AddressMode_t addrmode);
extern bool TarvosIIIPlug_Deinit(void);

extern bool TarvosIIIPlug_PinReset(void);
extern bool TarvosIIIPlug_Reset(void);

extern bool TarvosIIIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb);
extern bool TarvosIIIPlug_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool TarvosIIIPlug_Get(TarvosIIIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool TarvosIIIPlug_GetFirmwareVersion(uint8_t* fw);
extern bool TarvosIIIPlug_GetSerialNumber(uint8_t* sn);
extern bool TarvosIIIPlug_GetDefaultTXPower(uint8_t* txpower);
extern bool TarvosIIIPlug_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* srcaddr_msb);
extern bool TarvosIIIPlug_GetSourceNetID(uint8_t* srcnetid);
extern bool TarvosIIIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* srcaddr_msb);
extern bool TarvosIIIPlug_GetDefaultDestNetID(uint8_t* destnetid);
extern bool TarvosIIIPlug_GetDefaultRFChannel(uint8_t* channel);
extern bool TarvosIIIPlug_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool TarvosIIIPlug_FactoryReset(void);
extern bool TarvosIIIPlug_Set(TarvosIIIPlug_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool TarvosIIIPlug_Configure(TarvosIIIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool TarvosIIIPlug_SetDefaultTXPower(uint8_t txpower);
extern bool TarvosIIIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIIIPlug_SetSourceNetID(uint8_t srcnetid);
extern bool TarvosIIIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIIIPlug_SetDefaultDestNetID(uint8_t destnetid);
extern bool TarvosIIIPlug_SetDefaultRFChannel(uint8_t channel);
extern bool TarvosIIIPlug_SetDefaultRFProfile(uint8_t profile);
extern bool TarvosIIIPlug_EnableSnifferMode();

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool TarvosIIIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool TarvosIIIPlug_SetVolatile_DestNetID(uint8_t destnetid);
extern bool TarvosIIIPlug_SetVolatile_TXPower(uint8_t power);
extern bool TarvosIIIPlug_SetVolatile_Channel(uint8_t channel);

extern bool TarvosIIIPlug_SendUART(uint8_t* payload, uint8_t length, uint16_t waittime);

#ifdef __cplusplus
}
#endif
