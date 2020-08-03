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
#include "../../drivers/TarvosI/TarvosI.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Pins */
#define TarvosIPlug_CBUS_RESET PIN_FTDI_CBUS0

#define TarvosIPlug_BROADCASTADDRESS 0xFF

#define TarvosIPlug_AddressMode_t                       TarvosI_AddressMode_t
#define TarvosIPlug_UserSettings_t                      TarvosI_UserSettings_t
#define TarvosIPlug_Configuration_t                     TarvosI_Configuration_t

#define TarvosIPlug_USERSETTING_MEMPOSITION_NUMRETRYS         TarvosI_USERSETTING_MEMPOSITION_NUMRETRYS
#define TarvosIPlug_USERSETTING_MEMPOSITION_DEFAULTDESTNETID  TarvosI_USERSETTING_MEMPOSITION_DEFAULTDESTNETID
#define TarvosIPlug_USERSETTING_MEMPOSITION_DEFAULTDESTADDR   TarvosI_USERSETTING_MEMPOSITION_DEFAULTDESTADDR
#define TarvosIPlug_USERSETTING_MEMPOSITION_SOURCENETID       TarvosI_USERSETTING_MEMPOSITION_SOURCENETID
#define TarvosIPlug_USERSETTING_MEMPOSITION_SOURCEADDR        TarvosI_USERSETTING_MEMPOSITION_SOURCEADDR
#define TarvosIPlug_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER  TarvosI_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER
#define TarvosIPlug_USERSETTING_MEMPOSITION_DEFAULTRFCHANNEL  TarvosI_USERSETTING_MEMPOSITION_DEFAULTRFCHANNEL
#define TarvosIPlug_USERSETTING_MEMPOSITION_OP_MODE           TarvosI_USERSETTING_MEMPOSITION_OP_MODE
#define TarvosIPlug_USERSETTING_MEMPOSITION_CFG_FLAGS         TarvosI_USERSETTING_MEMPOSITION_CFG_FLAGS


extern bool TarvosIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIPlug_AddressMode_t addrmode);
extern bool TarvosIPlug_Deinit(void);

extern bool TarvosIPlug_PinReset(void);
extern bool TarvosIPlug_Reset(void);

extern bool TarvosIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb);
extern bool TarvosIPlug_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool TarvosIPlug_Get(TarvosIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool TarvosIPlug_GetMultiple(uint8_t startAddress, uint8_t lengthToRead, uint8_t *response, uint8_t *response_length);
extern bool TarvosIPlug_GetFirmwareVersion(uint8_t* fw);
extern bool TarvosIPlug_GetSerialNumber(uint8_t* sn);
extern bool TarvosIPlug_GetSourceAddr(uint8_t* srcaddr_lsb);
extern bool TarvosIPlug_GetSourceNetID(uint8_t* srcnetid);
extern bool TarvosIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb);
extern bool TarvosIPlug_GetDefaultDestNetID(uint8_t* destnetid);
extern bool TarvosIPlug_GetDefaultRFChannel(uint8_t* channel);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool TarvosIPlug_Set(TarvosIPlug_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool TarvosIPlug_Configure(TarvosIPlug_Configuration_t* config, uint8_t config_length);
extern bool TarvosIPlug_SetDefaultTXPower(int8_t txpower);
extern bool TarvosIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIPlug_SetSourceNetID(uint8_t srcnetid);
extern bool TarvosIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIPlug_SetDefaultDestNetID(uint8_t destnetid);
extern bool TarvosIPlug_SetDefaultRFChannel(uint8_t channel);

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool TarvosIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool TarvosIPlug_SetVolatile_DestNetID(uint8_t destnetid);
extern bool TarvosIPlug_SetVolatile_Channel(uint8_t channel);

extern bool TarvosIPlug_SendUART(uint8_t* payload, uint8_t length, uint16_t waittime);

#ifdef __cplusplus
}
#endif
