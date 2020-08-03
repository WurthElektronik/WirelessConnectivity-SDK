#ifndef _AT_DEVICE_DEFINED
#define _AT_DEVICE_DEFINED

/* ###########################################################################
* Includes:
*/

#include <stdbool.h>
#include <stdint.h>
#include "../Calypso.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/

#define AT_SLEEP_MIN_TIMEOUT (uint32_t)1
#define AT_SLEEP_MAX_TIMEOUT (uint32_t)86400
#define AT_STOP_MAX_TIMEOUT ( uint16_t)0xFFFF

/*
* Constant Macros.
* ########################### */

/* #############################
* Function Macros:
*/

#define ATDEVICE_ID(GENERATOR)       \
    GENERATOR(ATDEVICE_ID_, status)  \
    GENERATOR(ATDEVICE_ID_, general) \
    GENERATOR(ATDEVICE_ID_, fprov)   \
    GENERATOR(ATDEVICE_ID_, IOT)     \
    GENERATOR(ATDEVICE_ID_, UART)    \
    GENERATOR(ATDEVICE_ID_, NumberOfValues)    \


#define ATDEVICE_STATUS(GENERATOR)       \
    GENERATOR(ATDEVICE_STATUS_, device)   \
    GENERATOR(ATDEVICE_STATUS_, WLAN)     \
    GENERATOR(ATDEVICE_STATUS_, BSD)      \
    GENERATOR(ATDEVICE_STATUS_, netapp)   \
    GENERATOR(ATDEVICE_STATUS_, NumberOfValues)   \

#define ATDEVICE_GENERAL(GENERATOR)          \
    GENERATOR(ATDEVICE_GENERAL_, version)     \
    GENERATOR(ATDEVICE_GENERAL_, time)        \
    GENERATOR(ATDEVICE_GENERAL_, persistent)  \
    GENERATOR(ATDEVICE_GENERAL_, NumberOfValues)  \

#define ATDEVICE_IOT(GENERATOR)      \
    GENERATOR(ATDEVICE_IOT_, UDID)    \
    GENERATOR(ATDEVICE_IOT_, NumberOfValues)    \


#define ATDEVICE_UART(GENERATOR)         \
    GENERATOR(ATDEVICE_UART_, baudrate)   \
    GENERATOR(ATDEVICE_UART_, parity)     \
    GENERATOR(ATDEVICE_UART_, NumberOfValues)     \


    /*
* Function Macros.
* ########################## */

/* #############################
* Typedefs:
*/

/* Generate enum with all valid ID parmeters for AT+get=[ID],[OPTION]*/
 typedef enum ATDeviceGetId_t
 {
    ATDEVICE_ID(GENERATE_ENUM)
 } ATDeviceGetId_t;

/* Generate enum with all valid option parmeters for Status ID*/
 typedef enum ATDeviceGetStatus_t
 {
    ATDEVICE_STATUS(GENERATE_ENUM)
 } ATDeviceGetStatus_t;

/* Generate enum with all valid option parmeters for General ID*/
 typedef enum ATDeviceGetGeneral_t
 {
    ATDEVICE_GENERAL(GENERATE_ENUM)
 } ATDeviceGetGeneral_t;

/* Generate enum with all valid option parmeters for IoT ID*/
 typedef enum ATDeviceGetIot_t
 {
    ATDEVICE_IOT(GENERATE_ENUM)
 } ATDeviceGetIot_t;

/* Generate enum with all valid option parmeters for UART ID*/
 typedef enum ATDeviceGetUart_t
 {
    ATDEVICE_UART(GENERATE_ENUM)
 } ATDeviceGetUart_t;


typedef struct ATDevice_Time_t
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} ATDevice_Time_t;

typedef struct ATDevice_Version_t
{
    char chipId[12];
    char FWVersion[8];
    char PHYVersion[8];
    char NWPVersion[8];
    char ROMVersion[8];
} ATDevice_Version_t;

 /* struct to hold the response of AT+get command*/
// typedef struct  ATDevice_GetResponse_t
// {
//
// } ATDevice_GetResponse_t;

/*
* Typedefs.
* ########################## */

/*
* Macros & Typedefs.
* ######################################################################## */

/* ##########################################################################
* Declarations:
*/

/* #############################
* Exported Variables:
*/


/*
* Exported Variables.
* ########################## */

/* #############################
* Exported Functions:
*/

extern bool ATDevice_test();
extern bool ATDevice_start();
extern bool ATDevice_stop(uint32_t timeoutInMS);
extern bool ATDevice_reboot();
extern bool ATDevice_factoryReset();
extern bool ATDevice_sleep(uint32_t timeoutInSeconds);
extern bool ATDevice_get(uint8_t id, uint8_t option, void *pValues, size_t *pValuesSize);
extern bool ATDevice_set(uint8_t id, uint8_t option, void *pValues);

/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */


#endif /* _AT_DEVICE_DEFINED*/
