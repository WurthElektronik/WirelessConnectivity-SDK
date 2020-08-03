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

#ifndef _ThebeII_defined
#define _ThebeII_defined

/* Pins */
#define ThebeII_PIN_RESET  7  // LED Pin - wiringPi (pin 7 - BCM_GPIO 4).
#define ThebeII_PIN_WAKEUP 0  // LED Pin - wiringPi (pin 0 - BCM_GPIO 17)
#define ThebeII_PIN_PWM    1  // LED Pin - wiringPi (pin 1 - BCM_GPIO 18)
#define ThebeII_PIN_BOOT   2  // LED Pin - wiringPi (pin 2 - BCM_GPIO Rv1)
#define ThebeII_PIN_RTS    4  // LED Pin - wiringPi (pin 4 - BCM_GPIO 23)
#define ThebeII_PIN_CTS    5  // LED Pin - wiringPi (pin 5 - BCM_GPIO 24)

#define ThebeII_BROADCASTADDRESS 0xFF
#define MAX_USERSETTING_LENGTH 4

#define ThebeII_MIN_RFCHANNEL 128
#define ThebeII_MAX_RFCHANNEL 133


typedef enum ThebeII_AddressMode_t
{
    AddressMode_0 = 0x00,
    AddressMode_1,
    AddressMode_2,
    AddressMode_3,
}
ThebeII_AddressMode_t;

typedef enum ThebeII_UserSettings_t
{
    ThebeII_CMD_SETGET_OPTION_UARTBAUDRATE = 0x00,
    ThebeII_CMD_SETGET_OPTION_DEFAULTRFPROFILE = 0x01,
    ThebeII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER = 0x02,
    ThebeII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL = 0x03,
    ThebeII_CMD_SETGET_OPTION_DEFAULTADDRESSMODE = 0x04,
    ThebeII_CMD_SETGET_OPTION_NUMRETRYS = 0x06,
    ThebeII_CMD_SETGET_OPTION_DEFAULTDESTNETID = 0x07,
    ThebeII_CMD_SETGET_OPTION_DEFAULTDESTADDR = 0x08,
    ThebeII_CMD_SETGET_OPTION_SOURCENETID = 0x0A,
    ThebeII_CMD_SETGET_OPTION_SOURCEADDR = 0x0B,
    ThebeII_CMD_SETGET_OPTION_CFG_FLAGS = 0x0F,
    ThebeII_CMD_SETGET_OPTION_RP_FLAGS = 0x10,
    ThebeII_CMD_SETGET_OPTION_RP_NUMSLOTS = 0x11,
    ThebeII_CMD_SETGET_OPTION_FACTORYSETTINGS = 0x20,
    ThebeII_CMD_SETGET_OPTION_FWVERSION = 0x21,
    ThebeII_CMD_SETGET_OPTION_RUNTIMESETTINGS = 0x22,
} ThebeII_UserSettings_t;

/*
 * Struct repesenting  a usersetting
 * members:
 * -usersetting:  usersetting
 * -value:        value
 * -value-length: length of the value
 */
typedef struct ThebeII_Configuration_t
{
   ThebeII_UserSettings_t usersetting;
   uint8_t value[MAX_USERSETTING_LENGTH];
   uint8_t value_length;
} ThebeII_Configuration_t;


extern bool ThebeII_Init(int baudrate, int rp, int wp, int bp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThebeII_AddressMode_t addrmode);
extern bool ThebeII_Deinit(void);

extern bool ThebeII_PinReset(void);
extern bool ThebeII_Reset(void);

extern bool ThebeII_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb);
extern bool ThebeII_Transmit(uint8_t* payload, uint8_t length);

extern bool ThebeII_Shutdown(void);
extern bool ThebeII_Standby(void);
extern bool ThebeII_PinWakeup();

/* read the non-volatile settings */
extern bool ThebeII_Get(ThebeII_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool ThebeII_GetFirmwareVersion(uint8_t* fw);
extern bool ThebeII_GetSerialNumber(uint8_t* sn);
extern bool ThebeII_GetDefaultTXPower(uint8_t* txpower);
extern bool ThebeII_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* srcaddr_msb);
extern bool ThebeII_GetSourceNetID(uint8_t* srcnetid);
extern bool ThebeII_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* srcaddr_msb);
extern bool ThebeII_GetDefaultDestNetID(uint8_t* destnetid);
extern bool ThebeII_GetDefaultRFChannel(uint8_t* channel);
extern bool ThebeII_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool ThebeII_FactoryReset(void);
extern bool ThebeII_Set(ThebeII_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool ThebeII_Configure(ThebeII_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool ThebeII_SetDefaultTXPower(uint8_t txpower);
extern bool ThebeII_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool ThebeII_SetSourceNetID(uint8_t srcnetid);
extern bool ThebeII_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool ThebeII_SetDefaultDestNetID(uint8_t destnetid);
extern bool ThebeII_SetDefaultRFChannel(uint8_t channel);
extern bool ThebeII_SetDefaultRFProfile(uint8_t profile);
extern bool ThebeII_EnableSnifferMode();

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool ThebeII_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool ThebeII_SetVolatile_DestNetID(uint8_t destnetid);
extern bool ThebeII_SetVolatile_TXPower(uint8_t power);
extern bool ThebeII_SetVolatile_Channel(uint8_t channel);

extern bool ThebeII_Ping();

#endif // _ThebeII_defined
#ifdef __cplusplus
}
#endif
