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
 * COPYRIGHT (c) 2020 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "../drivers/ThyoneIPlug/ThyoneIPlug.h"
#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"


static void Application(void);

static void RX_test(void);

static void ThyoneIPlug_test_function(void);

static void RXcallback(uint8_t* payload, uint16_t payload_length, uint32_t sourceAddress, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 0
    /* function to test all functions of the ThyoneIPlug driver */
    ThyoneIPlug_test_function();
#else
    RX_test();
#endif

    AbortMainLoop = true;
    return 0 ;

}

/* the main function simply starts the MainThread */
int main ()
{
    AbortMainLoop = false;

    if(pthread_create(&thread_main, NULL, &main_Thread, NULL))
    {
        fprintf(stdout, "Failed to start thread_main");
        return false;
    }

    while(1)
    {
        delay(1000);
        if(AbortMainLoop == true)
        {
            /* jump out of the main loop and exit program */
            return 0;
        }
    }

    return 0;
}


/* callback for data reception */
static void RXcallback(uint8_t* payload, uint16_t payload_length, uint32_t sourceAddress, int8_t rssi)
{
    int i = 0;
    printf ("Received data from address 0x%02x with %d dBm:\n-> ", sourceAddress, rssi);
    printf("0x ");
    for(i=0; i<payload_length; i++)
    {
        printf ("%02x ", *(payload+i)) ;
    }
    printf ("\n-> ") ;
    for(i=0; i<payload_length; i++)
    {
        printf ("%c", *(payload+i)) ;
    }
    printf ("\n") ;
    fflush (stdout) ;
}


/* test function to only stay on RX */
static void RX_test()
{
    bool ret = false;

    /* initialize the module ThyoneIPlug */
    ret = ThyoneIPlug_Init(115200, ThyoneIPlug_CBUS_RESET, RXcallback);
    Debug_out("ThyoneIPlug_Init", ret);

    ret = ThyoneIPlug_PinReset();
    Debug_out("PinReset", ret);
    delay(500);

    printf ("Waiting for incoming data\n");
    fflush (stdout) ;
    while(1)
    {
        /* do nothing, just stay on RX */
        delay(1000);
    }
}

/* test function to demonstrate the usage of the ThyoneIPlug functions */
static void ThyoneIPlug_test_function()
{
    bool ret = false;
    uint8_t serial_number[4];
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf(stdout, COLOR_CYAN "ThyoneIPlug driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module ThyoneIPlug */
    ret = ThyoneIPlug_Init(115200, ThyoneIPlug_CBUS_RESET, RXcallback);
    Debug_out("ThyoneIPlug_Init", ret);

    if(ret)
    {
        ret = ThyoneIPlug_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        ret = ThyoneIPlug_GetSerialNumber(serial_number);
        Debug_out("GetSerialNumber",ret);
        if(ret)
        {
            uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
            fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(500);

#if 0
        ThyoneIPlug_BaudRateIndex_t baudrateSet = ThyoneI_BaudRateIndex_9600;
        ThyoneIPlug_BaudRateIndex_t baudrateGet = ThyoneI_BaudRateIndex_4800;

        ThyoneIPlug_UartParity_t parity = ThyoneI_UartParity_None;
        bool flowControlEnabled = false;

        ret = ThyoneIPlug_GetBaudrateIndex(&baudrateGet, &parity, &flowControlEnabled);
        Debug_out("Get baud rate index", ret);
        fprintf(stdout, "baudrate:%u parity: %u, flowControl: %s \n", baudrateGet, parity, (flowControlEnabled ? "enabled" : "disabled"));

        ret = ThyoneIPlug_SetBaudrateIndex(baudrateSet, parity, flowControlEnabled);
        Debug_out("Set baud rate index", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        if(ret)
        {
            ThyoneIPlug_Deinit();
            ret = ThyoneIPlug_Init(9600, ThyoneIPlug_CBUS_RESET, RXcallback);
        }

        ret = ThyoneIPlug_GetBaudrateIndex(&baudrateGet, &parity, &flowControlEnabled);
        Debug_out("Get baud rate index", ret);
        fprintf(stdout, "baudrate:%u parity: %u, flowControl: %s \n", baudrateGet, parity, (flowControlEnabled ? "enabled" : "disabled"));

#endif

#if 0
        /* Group ID */
        uint8_t groupIDSet = 0x16;
        uint8_t groupIDGet = 0;

        ret = ThyoneIPlug_GetGroupID(&groupIDGet);
        Debug_out("Get Group ID", ret);
        fprintf(stdout, "Group ID: %u\n", groupIDGet);

        ret = ThyoneIPlug_SetGroupID(groupIDSet);
        Debug_out("Set Group ID", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        ret = ThyoneIPlug_GetGroupID(&groupIDGet);
        Debug_out("Get Group ID", ret);
        fprintf(stdout, "Group ID: %u\n", groupIDGet);

        /* Rf Profile */
        ThyoneIPlug_Profile_t profileSet = ThyoneI_Profile_500kbit;
        ThyoneIPlug_Profile_t profileGet = ThyoneI_Profile_Invalid;

        ret = ThyoneIPlug_GetRfProfile(&profileGet);
        Debug_out("Get RF profile", ret);
        fprintf(stdout, "RF profile: %u\n", profileGet);

        ret = ThyoneIPlug_SetRfProfile(profileSet);
        Debug_out("Set RF profile", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        ret = ThyoneIPlug_GetRfProfile(&profileGet);
        Debug_out("Get RF profile", ret);
        fprintf(stdout, "RF profile: %u\n", profileGet);

        /* TX power */
        ThyoneIPlug_TXPower_t txPowerSet = ThyoneI_TXPower_8;
        ThyoneIPlug_TXPower_t txPowerGet = ThyoneI_TXPower_0;

        ret = ThyoneIPlug_GetTXPower(&txPowerGet);
        Debug_out("Get tx power", ret);
        fprintf(stdout, "Tx power: %u\n", txPowerGet);

        ret = ThyoneIPlug_SetTXPower(txPowerSet);
        Debug_out("Set tx power", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        ret = ThyoneIPlug_GetTXPower(&txPowerGet);
        Debug_out("Get tx power", ret);
        fprintf(stdout, "Tx power: %u\n", txPowerGet);
#endif

#if 0
        /* RP_NumRetries*/
        uint8_t numRetriesSet = 5;
        uint8_t numRetriesGet = 0;

        ret = ThyoneIPlug_GetNumRetries(&numRetriesGet);
        Debug_out("ThyoneIPlug_GetNumRetries", ret);
        fprintf(stdout, "Number of retries: %u\n", numRetriesGet);

        ret = ThyoneIPlug_SetNumRetries(numRetriesSet);
        Debug_out("ThyoneIPlug_SetNumRetries", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        ret = ThyoneIPlug_GetNumRetries(&numRetriesGet);
        Debug_out("ThyoneIPlug_GetNumRetries", ret);
        fprintf(stdout, "Number of retries: %u\n", numRetriesGet);

        /* RP Num Slots */
        uint8_t numSlotsSet = 64;
        uint8_t numSlotsGet = 0;

        ret = ThyoneIPlug_GetRpNumSlots(&numSlotsGet);
        Debug_out("ThyoneIPlug_GetRpNumSlots", ret);
        fprintf(stdout, "Number of slots: %u\n", numSlotsGet);

        ret = ThyoneIPlug_SetRpNumSlots(numSlotsSet);
        Debug_out("ThyoneIPlug_SetRpNumSlots", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        ret = ThyoneIPlug_GetRpNumSlots(&numSlotsGet);
        Debug_out("ThyoneIPlug_GetRpNumSlots", ret);
        fprintf(stdout, "Number of slots: %u\n", numSlotsGet);


#endif

#if 0

        uint8_t payload[11] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

        ret = ThyoneIPlug_TransmitBroadcast(payload, sizeof(payload));
        Debug_out("ThyoneIPlug_TransmitBroadcast", ret);


        ret = ThyoneIPlug_TransmitMulticastExtended(16, payload, sizeof(payload));
        Debug_out("ThyoneIPlug_TransmitUnicastExtended", ret);

        /* Send unicast to address as defined by usersetting*/
        uint32_t destAddressSet = 0x12345678;
        uint32_t destAddressGet = 0;

        /* the module will store the destination address LSB-first. But as uint32_t is also stored as LSB-first we can input the address just as an uint32_t value*/
        ret = ThyoneIPlug_SetDestinationAddress(destAddressSet);
        Debug_out("ThyoneIPlug_SetDestinationAddress", ret);
        delay(ThyoneIPlug_BOOT_DURATION);

        ret = ThyoneIPlug_GetDestinationAddress(&destAddressGet);
        Debug_out("ThyoneIPlug_GetDestinationAddress", ret);
        fprintf(stdout, "DestinationAddress: 0x%X\n", destAddressGet);

        if(destAddressGet == destAddressSet)
        {
            ret = ThyoneIPlug_TransmitUnicast(payload, sizeof(payload));
            Debug_out("ThyoneIPlug_TransmitUnicast", ret);
        }

#endif

#if 0
        ret = ThyoneIPlug_FactoryReset();
        Debug_out("facReset", ret);
        delay(500);

#endif

#if 1

        /* Config remote */
        ThyoneIPlug_GPIOConfigBlock_t configRemoteGPIO[ThyoneIPlug_AMOUNT_GPIO_PINS];

        uint32_t destAddressRemoteGPIO = 0x22222222;

        /* config GPIO 1 as output high*/
        configRemoteGPIO[0].length = 0x03;
        configRemoteGPIO[0].GPIO_ID = ThyoneI_GPIO_1;
        configRemoteGPIO[0].InputOutput = ThyoneI_GPIO_IO_Output;
        configRemoteGPIO[0].value = ThyoneI_GPIO_Output_High;

        /* config GPIO 2 as input pulldown*/
        configRemoteGPIO[1].length = 0x03;
        configRemoteGPIO[1].GPIO_ID = ThyoneI_GPIO_2;
        configRemoteGPIO[1].InputOutput = ThyoneI_GPIO_IO_Input;
        configRemoteGPIO[1].value = ThyoneI_GPIO_Input_PullUp;

        ret = ThyoneIPlug_GPIORemoteSetConfig(destAddressRemoteGPIO, configRemoteGPIO, 2*sizeof(ThyoneIPlug_GPIOConfigBlock_t));
        Debug_out("ThyoneIPlug_GPIORemoteWriteConfig",ret);
        delay(500);

        uint16_t remoteConfigLength = 0;
        ret = ThyoneIPlug_GPIORemoteGetConfig(destAddressRemoteGPIO, configRemoteGPIO, &remoteConfigLength);
        Debug_out("ThyoneIPlug_GPIORemoteReadConfig",ret);


        /* Write remote */
        ThyoneIPlug_GPIOControlBlock_t controlRemoteGPIO[ThyoneIPlug_AMOUNT_GPIO_PINS];

        /* set GPIO 1 to low */
        controlRemoteGPIO[0].length = 0x02;
        controlRemoteGPIO[0].GPIO_ID = ThyoneI_GPIO_1;
        controlRemoteGPIO[0].value = ThyoneI_GPIO_Output_Low;

        ret = ThyoneIPlug_GPIORemoteWrite(destAddressRemoteGPIO, controlRemoteGPIO, sizeof(ThyoneIPlug_GPIOControlBlock_t));
        Debug_out("ThyoneIPlug_GPIORemoteWrite - Set GPIO 1 to low", ret);
        delay(500);

        uint8_t RemoteGPIOToRead[2];
        RemoteGPIOToRead[0] = ThyoneI_GPIO_1;
        RemoteGPIOToRead[1] = ThyoneI_GPIO_2;

        uint16_t readRemoteLength = 0;
        /* Read GPIO 1 and 2 */
        ret = ThyoneIPlug_GPIORemoteRead(destAddressRemoteGPIO, RemoteGPIOToRead, 2, controlRemoteGPIO, &readRemoteLength);
        Debug_out("ThyoneIPlug_GPIORemoteRead", ret);
        if(ret)
        {
            printf("GPIO %x: %x \n", controlRemoteGPIO[0].GPIO_ID, controlRemoteGPIO[0].value);
            printf("GPIO %x: %x \n", controlRemoteGPIO[1].GPIO_ID, controlRemoteGPIO[1].value);
        }

#endif
    }
    ret = ThyoneIPlug_Deinit();
    Debug_out("ThyoneIPlug_Deinit", ret);
}


