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
#include "../../drivers/TarvosII/TarvosII.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Pins */
#define TarvosIIPlug_CBUS_RESET PIN_FTDI_CBUS0

#define TarvosIIPlug_BROADCASTADDRESS 0xFF

#define TarvosIIPlug_AddressMode_t                       TarvosII_AddressMode_t
#define TarvosIIPlug_UserSettings_t                      TarvosII_UserSettings_t
#define TarvosIIPlug_Configuration_t                     TarvosII_Configuration_t

#define TarvosIIPlug_USERSETTING_MEMPOSITION_NUMRETRYS         TarvosII_USERSETTING_MEMPOSITION_NUMRETRYS
#define TarvosIIPlug_USERSETTING_MEMPOSITION_DEFAULTDESTNETID  TarvosII_USERSETTING_MEMPOSITION_DEFAULTDESTNETID
#define TarvosIIPlug_USERSETTING_MEMPOSITION_DEFAULTDESTADDR   TarvosII_USERSETTING_MEMPOSITION_DEFAULTDESTADDR
#define TarvosIIPlug_USERSETTING_MEMPOSITION_SOURCENETID       TarvosII_USERSETTING_MEMPOSITION_SOURCENETID
#define TarvosIIPlug_USERSETTING_MEMPOSITION_SOURCEADDR        TarvosII_USERSETTING_MEMPOSITION_SOURCEADDR
#define TarvosIIPlug_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER  TarvosII_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER
#define TarvosIIPlug_USERSETTING_MEMPOSITION_DEFAULTRFCHANNEL  TarvosII_USERSETTING_MEMPOSITION_DEFAULTRFCHANNEL
#define TarvosIIPlug_USERSETTING_MEMPOSITION_OP_MODE           TarvosII_USERSETTING_MEMPOSITION_OP_MODE
#define TarvosIIPlug_USERSETTING_MEMPOSITION_CFG_FLAGS         TarvosII_USERSETTING_MEMPOSITION_CFG_FLAGS
#define TarvosIIPlug_USERSETTING_MEMPOSITION_RP_FLAGS          TarvosII_USERSETTING_MEMPOSITION_RP_FLAGS
#define TarvosIIPlug_USERSETTING_MEMPOSITION_RP_NUMSLOTS       TarvosII_USERSETTING_MEMPOSITION_RP_NUMSLOTS
#define TarvosIIPlug_USERSETTING_MEMPOSITION_UARTBAUDRATE      TarvosII_USERSETTING_MEMPOSITION_UARTBAUDRATE


extern bool TarvosIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIIPlug_AddressMode_t addrmode);
extern bool TarvosIIPlug_Deinit(void);

extern bool TarvosIIPlug_PinReset(void);
extern bool TarvosIIPlug_Reset(void);

extern bool TarvosIIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb);
extern bool TarvosIIPlug_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool TarvosIIPlug_Get(TarvosIIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool TarvosIIPlug_GetMultiple(uint8_t startAddress, uint8_t lengthToRead, uint8_t *response, uint8_t *response_length);
extern bool TarvosIIPlug_GetFirmwareVersion(uint8_t* fw);
extern bool TarvosIIPlug_GetSerialNumber(uint8_t* sn);
extern bool TarvosIIPlug_GetDefaultTXPower(int8_t* txpower);
extern bool TarvosIIPlug_GetSourceAddr(uint8_t* srcaddr_lsb);
extern bool TarvosIIPlug_GetSourceNetID(uint8_t* srcnetid);
extern bool TarvosIIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb);
extern bool TarvosIIPlug_GetDefaultDestNetID(uint8_t* destnetid);
extern bool TarvosIIPlug_GetDefaultRFChannel(uint8_t* channel);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool TarvosIIPlug_FactoryReset(void);
extern bool TarvosIIPlug_Set(TarvosIIPlug_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool TarvosIIPlug_Configure(TarvosIIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool TarvosIIPlug_SetDefaultTXPower(int8_t txpower);
extern bool TarvosIIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIIPlug_SetSourceNetID(uint8_t srcnetid);
extern bool TarvosIIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIIPlug_SetDefaultDestNetID(uint8_t destnetid);
extern bool TarvosIIPlug_SetDefaultRFChannel(uint8_t channel);

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool TarvosIIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool TarvosIIPlug_SetVolatile_DestNetID(uint8_t destnetid);
extern bool TarvosIIPlug_SetVolatile_TXPower(int8_t power);
extern bool TarvosIIPlug_SetVolatile_Channel(uint8_t channel);

extern bool TarvosIIPlug_SendUART(uint8_t* payload, uint8_t length, uint16_t waittime);

#ifdef __cplusplus
}
#endif
