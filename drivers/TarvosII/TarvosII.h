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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TarvosII_defined
#define _TarvosII_defined

/* Pins */
#ifdef libgpiod_numbers
#define TarvosII_PIN_RESET  4 // BCM_GPIO 4
#elif defined wiringPi_numbers
#define TarvosII_PIN_RESET  7  // wiringPi (pin 7 - BCM_GPIO 4).
#else
#warning "No pins defined. Can be ignored for plug projects"
#endif

#define TarvosII_BROADCASTADDRESS 0xFF
#define MAX_USERSETTING_LENGTH 4

typedef enum TarvosII_AddressMode_t
{
    AddressMode_0 = 0x00,
    AddressMode_1,
    AddressMode_2,
}
TarvosII_AddressMode_t;

typedef enum TarvosII_UserSettings_t
{
    TarvosII_USERSETTING_MEMPOSITION_NUMRETRYS = 0x14,
    TarvosII_USERSETTING_MEMPOSITION_DEFAULTDESTNETID = 0x18,
    TarvosII_USERSETTING_MEMPOSITION_DEFAULTDESTADDR = 0x19,
    TarvosII_USERSETTING_MEMPOSITION_SOURCENETID = 0x1C,
    TarvosII_USERSETTING_MEMPOSITION_SOURCEADDR = 0x1D,
    TarvosII_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER = 0x29,
    TarvosII_USERSETTING_MEMPOSITION_DEFAULTRFCHANNEL = 0x2A,
    TarvosII_USERSETTING_MEMPOSITION_OP_MODE = 0x3C,
    TarvosII_USERSETTING_MEMPOSITION_CFG_FLAGS = 0x48,
    TarvosII_USERSETTING_MEMPOSITION_RP_FLAGS = 0x4A,
    TarvosII_USERSETTING_MEMPOSITION_RP_NUMSLOTS = 0x4C,
    TarvosII_USERSETTING_MEMPOSITION_UARTBAUDRATE = 0x50,
} TarvosII_UserSettings_t;

/*
 * Struct repesenting  a usersetting
 * members:
 * -usersetting:  usersetting
 * -value:        value
 * -value-length: length of the value
 */
typedef struct TarvosII_Configuration_t
{
    TarvosII_UserSettings_t usersetting;
    uint8_t value[MAX_USERSETTING_LENGTH];
    uint8_t value_length;
} TarvosII_Configuration_t;

/* Functions to initialize/deinitialize the module.
 * Deinit can be used for both.
 */
extern bool TarvosII_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosII_AddressMode_t addrmode);
extern bool TarvosII_Deinit(void);

extern bool TarvosII_PinReset(void);
extern bool TarvosII_Reset(void);

extern bool TarvosII_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb);
extern bool TarvosII_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool TarvosII_Get(TarvosII_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool TarvosII_GetMultiple(uint8_t startAddress, uint8_t lengthToRead, uint8_t *response, uint8_t *response_length);
extern bool TarvosII_GetFirmwareVersion(uint8_t* fw);
extern bool TarvosII_GetSerialNumber(uint8_t* sn);
extern bool TarvosII_GetDefaultTXPower(int8_t* txpower);
extern bool TarvosII_GetSourceAddr(uint8_t* srcaddr_lsb);
extern bool TarvosII_GetSourceNetID(uint8_t* srcnetid);
extern bool TarvosII_GetDefaultDestAddr(uint8_t* destaddr_lsb);
extern bool TarvosII_GetDefaultDestNetID(uint8_t* destnetid);
extern bool TarvosII_GetDefaultRFChannel(uint8_t* channel);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool TarvosII_FactoryReset(void);
extern bool TarvosII_Set(TarvosII_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool TarvosII_Configure(TarvosII_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool TarvosII_SetDefaultTXPower(int8_t txpower);
extern bool TarvosII_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosII_SetSourceNetID(uint8_t srcnetid);
extern bool TarvosII_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosII_SetDefaultDestNetID(uint8_t destnetid);
extern bool TarvosII_SetDefaultRFChannel(uint8_t channel);

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool TarvosII_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool TarvosII_SetVolatile_DestNetID(uint8_t destnetid);
extern bool TarvosII_SetVolatile_TXPower(int8_t power);
extern bool TarvosII_SetVolatile_Channel(uint8_t channel);

#endif // _TarvosII_defined
#ifdef __cplusplus
}
#endif
