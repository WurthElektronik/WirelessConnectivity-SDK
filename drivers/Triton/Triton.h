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

#ifndef _Triton_defined
#define _Triton_defined

/* Pins */
#ifdef libgpiod_numbers
#define Triton_PIN_RESET  4   // BCM_GPIO 4
#define Triton_PIN_WAKEUP 17  // BCM_GPIO 17
#elif defined wiringPi_numbers
#define Triton_PIN_RESET  7  // wiringPi (pin 7 - BCM_GPIO 4).
#define Triton_PIN_WAKEUP 0  // wiringPi (pin 0 - BCM_GPIO 17
#else
#warning "No pins defined. Can be ignored for plug projects"
#endif

#define Triton_BROADCASTADDRESS 0xFF
#define MAX_USERSETTING_LENGTH 4

typedef enum Triton_UserSettings_t
{
    Triton_CMD_SETGET_OPTION_RFCHANNEL     = 0x00,
    Triton_CMD_SETGET_OPTION_RFRATE        = 0x01,
    Triton_CMD_SETGET_OPTION_SUBNETID      = 0x02,
    Triton_CMD_SETGET_OPTION_ADDRESSSOURCE = 0x03,
    Triton_CMD_SETGET_OPTION_RFPOWERLEVEL  = 0x04,
    Triton_CMD_SETGET_OPTION_CFG_FLAGS     = 0x05,
    Triton_CMD_SETGET_OPTION_UARTBAUDRATE  = 0x3C,
} Triton_UserSettings_t;

/*
 * Struct repesenting  a usersetting
 * members:
 * -usersetting:  usersetting
 * -value:        value
 * -value-length: length of the value
 */
typedef struct Triton_Configuration_t
{
   Triton_UserSettings_t usersetting;
   uint8_t value[MAX_USERSETTING_LENGTH];
   uint8_t value_length;
} Triton_Configuration_t;


extern bool Triton_Init(int baudrate, int rp, int wp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,int8_t));
extern bool Triton_Deinit(void);

extern bool Triton_PinReset(void);
extern bool Triton_Reset(void);

extern bool Triton_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t dest_address);

extern bool Triton_Sleep(void);
extern bool Triton_PinWakeup();

extern bool Triton_GetRSSI(int8_t* rssi);

/* read the non-volatile settings */
extern bool Triton_Get(Triton_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool Triton_GetFirmwareVersion(uint8_t* fw);
extern bool Triton_GetSerialNumber(uint8_t* sn);
extern bool Triton_GetDefaultTXPower(uint8_t* txpower);
extern bool Triton_GetSourceAddr(uint8_t* srcaddr);
extern bool Triton_GetSubnet(uint8_t* subnet);
extern bool Triton_GetDefaultRFChannel(uint8_t* channel);
extern bool Triton_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool Triton_FactoryReset(void);
extern bool Triton_Set(Triton_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool Triton_Configure(Triton_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool Triton_SetDefaultTXPower(int8_t txpower);
extern bool Triton_SetSourceAddr(uint8_t srcaddr);
extern bool Triton_SetSubnet(uint8_t subnet);
extern bool Triton_SetDefaultRFChannel(uint8_t channel);
extern bool Triton_SetDefaultRFProfile(uint8_t profile);
extern bool Triton_SetDefaultBaudrate(uint32_t baudrate);

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool Triton_SetVolatile_TXPower(int8_t txpower);
extern bool Triton_SetVolatile_Channel(uint8_t channel);
extern bool Triton_SetVolatile_SourceAddress(uint8_t sourceaddr);
extern bool Triton_SetVolatile_Subnet(uint8_t subnet);

#endif // _Triton_defined
#ifdef __cplusplus
}
#endif
