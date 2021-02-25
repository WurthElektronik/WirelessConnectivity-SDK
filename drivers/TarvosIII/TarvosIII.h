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

#ifndef _TarvosIII_defined
#define _TarvosIII_defined

/* Pins */
#ifdef libgpiod_numbers
#define TarvosIII_PIN_RESET  4   // BCM_GPIO 4
#define TarvosIII_PIN_WAKEUP 17  // BCM_GPIO 17
#define TarvosIII_PIN_BOOT   27  // BCM_GPIO 27
#define TarvosIII_PIN_RTS    23  // BCM_GPIO 23
#define TarvosIII_PIN_CTS    24  // BCM_GPIO 24
#elif defined wiringPi_numbers
#define TarvosIII_PIN_RESET  7  // wiringPi (pin 7 - BCM_GPIO 4
#define TarvosIII_PIN_WAKEUP 0  // wiringPi (pin 0 - BCM_GPIO 17
#define TarvosIII_PIN_BOOT   2  // wiringPi (pin 2 - BCM_GPIO 27
#define TarvosIII_PIN_RTS    4  // wiringPi (pin 4 - BCM_GPIO 23
#define TarvosIII_PIN_CTS    5  // wiringPi (pin 5 - BCM_GPIO 24
#else
#warning "No pins defined. Can be ignored for plug projects"
#endif

#define TarvosIII_BROADCASTADDRESS 0xFF
#define MAX_USERSETTING_LENGTH 4

typedef enum TarvosIII_AddressMode_t
{
    AddressMode_0 = 0x00,
    AddressMode_1,
    AddressMode_2,
    AddressMode_3,
}
TarvosIII_AddressMode_t;

typedef enum TarvosIII_UserSettings_t
{
    TarvosIII_CMD_SETGET_OPTION_UARTBAUDRATE = 0x00,
    TarvosIII_CMD_SETGET_OPTION_DEFAULTRFPROFILE = 0x01,
    TarvosIII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER = 0x02,
    TarvosIII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL = 0x03,
    TarvosIII_CMD_SETGET_OPTION_DEFAULTADDRESSMODE = 0x04,
    TarvosIII_CMD_SETGET_OPTION_NUMRETRYS = 0x06,
    TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTNETID = 0x07,
    TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTADDR = 0x08,
    TarvosIII_CMD_SETGET_OPTION_SOURCENETID = 0x0A,
    TarvosIII_CMD_SETGET_OPTION_SOURCEADDR = 0x0B,
    TarvosIII_CMD_SETGET_OPTION_CFG_FLAGS = 0x0F,
    TarvosIII_CMD_SETGET_OPTION_RP_FLAGS = 0x10,
    TarvosIII_CMD_SETGET_OPTION_RP_NUMSLOTS = 0x11,
    TarvosIII_CMD_SETGET_OPTION_FACTORYSETTINGS = 0x20,
    TarvosIII_CMD_SETGET_OPTION_FWVERSION = 0x21,
    TarvosIII_CMD_SETGET_OPTION_RUNTIMESETTINGS = 0x22,
} TarvosIII_UserSettings_t;

/*
 * Struct repesenting  a usersetting
 * members:
 * -usersetting:  usersetting
 * -value:        value
 * -value-length: length of the value
 */
typedef struct TarvosIII_Configuration_t
{
   TarvosIII_UserSettings_t usersetting;
   uint8_t value[MAX_USERSETTING_LENGTH];
   uint8_t value_length;
} TarvosIII_Configuration_t;


extern bool TarvosIII_Init(int baudrate, int rp, int wp, int bp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIII_AddressMode_t addrmode);
extern bool TarvosIII_Deinit(void);

extern bool TarvosIII_PinReset(void);
extern bool TarvosIII_Reset(void);

extern bool TarvosIII_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb);
extern bool TarvosIII_Transmit(uint8_t* payload, uint8_t length);

extern bool TarvosIII_Shutdown(void);
extern bool TarvosIII_Standby(void);
extern bool TarvosIII_PinWakeup();

/* read the non-volatile settings */
extern bool TarvosIII_Get(TarvosIII_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool TarvosIII_GetFirmwareVersion(uint8_t* fw);
extern bool TarvosIII_GetSerialNumber(uint8_t* sn);
extern bool TarvosIII_GetDefaultTXPower(uint8_t* txpower);
extern bool TarvosIII_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* srcaddr_msb);
extern bool TarvosIII_GetSourceNetID(uint8_t* srcnetid);
extern bool TarvosIII_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* srcaddr_msb);
extern bool TarvosIII_GetDefaultDestNetID(uint8_t* destnetid);
extern bool TarvosIII_GetDefaultRFChannel(uint8_t* channel);
extern bool TarvosIII_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool TarvosIII_FactoryReset(void);
extern bool TarvosIII_Set(TarvosIII_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool TarvosIII_Configure(TarvosIII_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool TarvosIII_SetDefaultTXPower(uint8_t txpower);
extern bool TarvosIII_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIII_SetSourceNetID(uint8_t srcnetid);
extern bool TarvosIII_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool TarvosIII_SetDefaultDestNetID(uint8_t destnetid);
extern bool TarvosIII_SetDefaultRFChannel(uint8_t channel);
extern bool TarvosIII_SetDefaultRFProfile(uint8_t profile);
extern bool TarvosIII_EnableSnifferMode();

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool TarvosIII_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool TarvosIII_SetVolatile_DestNetID(uint8_t destnetid);
extern bool TarvosIII_SetVolatile_TXPower(uint8_t power);
extern bool TarvosIII_SetVolatile_Channel(uint8_t channel);

extern bool TarvosIII_Ping();

#endif // _TarvosIII_defined
#ifdef __cplusplus
}
#endif
