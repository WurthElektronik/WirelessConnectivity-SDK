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

#include "../drivers/ThyoneI/ThyoneI.h"
#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"


static void Application(void);

static void RX_test(void);

static void ThyoneI_test_function(void);

static void RXcallback(uint8_t* payload, uint16_t payload_length, uint32_t sourceAddress, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 1
    /* function to test all functions of the ThyoneI driver */
    ThyoneI_test_function();
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

    /* initialize the module ThyoneI */
    ret = ThyoneI_Init(115200, ThyoneI_PIN_RESET, ThyoneI_PIN_WAKEUP, ThyoneI_PIN_BOOT, RXcallback);
    Debug_out("ThyoneI_Init", ret);

    ret = ThyoneI_PinReset();
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

/* test function to demonstrate the usage of the ThyoneI functions */
static void ThyoneI_test_function()
{
    bool ret = false;
    uint8_t serial_number[4];
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf(stdout, COLOR_CYAN "ThyoneI driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module ThyoneI */
    ret = ThyoneI_Init(115200, ThyoneI_PIN_RESET, ThyoneI_PIN_WAKEUP, ThyoneI_PIN_BOOT, RXcallback);
    Debug_out("ThyoneI_Init", ret);

    if(ret)
    {
        ret = ThyoneI_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        ret = ThyoneI_GetSerialNumber(serial_number);
        Debug_out("GetSerialNumber",ret);
        if(ret)
        {
            uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
            fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(500);

#if 0
        ThyoneI_BaudRateIndex_t baudrateSet = ThyoneI_BaudRateIndex_9600;
        ThyoneI_BaudRateIndex_t baudrateGet = ThyoneI_BaudRateIndex_4800;

        ThyoneI_UartParity_t parity = ThyoneI_UartParity_None;
        bool flowControlEnabled = false;

        ret = ThyoneI_GetBaudrateIndex(&baudrateGet, &parity, &flowControlEnabled);
        Debug_out("Get baud rate index", ret);
        fprintf(stdout, "baudrate:%u parity: %u, flowControl: %s \n", baudrateGet, parity, (flowControlEnabled ? "enabled" : "disabled"));

        ret = ThyoneI_SetBaudrateIndex(baudrateSet, parity, flowControlEnabled);
        Debug_out("Set baud rate index", ret);
        delay(ThyoneI_BOOT_DURATION);

        if(ret)
        {
            ThyoneI_Deinit();
            ret = ThyoneI_Init(9600, ThyoneI_PIN_RESET, ThyoneI_PIN_WAKEUP, ThyoneI_PIN_BOOT, RXcallback);
        }

        ret = ThyoneI_GetBaudrateIndex(&baudrateGet, &parity, &flowControlEnabled);
        Debug_out("Get baud rate index", ret);
        fprintf(stdout, "baudrate:%u parity: %u, flowControl: %s \n", baudrateGet, parity, (flowControlEnabled ? "enabled" : "disabled"));

#endif

#if 0
        /* Group ID */
        uint8_t groupIDSet = 0x16;
        uint8_t groupIDGet = 0;

        ret = ThyoneI_GetGroupID(&groupIDGet);
        Debug_out("Get Group ID", ret);
        fprintf(stdout, "Group ID: %u\n", groupIDGet);

        ret = ThyoneI_SetGroupID(groupIDSet);
        Debug_out("Set Group ID", ret);
        delay(ThyoneI_BOOT_DURATION);

        ret = ThyoneI_GetGroupID(&groupIDGet);
        Debug_out("Get Group ID", ret);
        fprintf(stdout, "Group ID: %u\n", groupIDGet);

        /* Rf Profile */
        ThyoneI_Profile_t profileSet = ThyoneI_Profile_500kbit;
        ThyoneI_Profile_t profileGet = ThyoneI_Profile_Invalid;

        ret = ThyoneI_GetRfProfile(&profileGet);
        Debug_out("Get RF profile", ret);
        fprintf(stdout, "RF profile: %u\n", profileGet);

        ret = ThyoneI_SetRfProfile(profileSet);
        Debug_out("Set RF profile", ret);
        delay(ThyoneI_BOOT_DURATION);

        ret = ThyoneI_GetRfProfile(&profileGet);
        Debug_out("Get RF profile", ret);
        fprintf(stdout, "RF profile: %u\n", profileGet);

        /* TX power */
        ThyoneI_TXPower_t txPowerSet = ThyoneI_TXPower_8;
        ThyoneI_TXPower_t txPowerGet = ThyoneI_TXPower_0;

        ret = ThyoneI_GetTXPower(&txPowerGet);
        Debug_out("Get tx power", ret);
        fprintf(stdout, "Tx power: %u\n", txPowerGet);

        ret = ThyoneI_SetTXPower(txPowerSet);
        Debug_out("Set tx power", ret);
        delay(ThyoneI_BOOT_DURATION);

        ret = ThyoneI_GetTXPower(&txPowerGet);
        Debug_out("Get tx power", ret);
        fprintf(stdout, "Tx power: %u\n", txPowerGet);
#endif

#if 0
        /* RP_NumRetries*/
        uint8_t numRetriesSet = 5;
        uint8_t numRetriesGet = 0;

        ret = ThyoneI_GetNumRetries(&numRetriesGet);
        Debug_out("ThyoneI_GetNumRetries", ret);
        fprintf(stdout, "Number of retries: %u\n", numRetriesGet);

        ret = ThyoneI_SetNumRetries(numRetriesSet);
        Debug_out("ThyoneI_SetNumRetries", ret);
        delay(ThyoneI_BOOT_DURATION);

        ret = ThyoneI_GetNumRetries(&numRetriesGet);
        Debug_out("ThyoneI_GetNumRetries", ret);
        fprintf(stdout, "Number of retries: %u\n", numRetriesGet);

        /* RP Num Slots */
        uint8_t numSlotsSet = 64;
        uint8_t numSlotsGet = 0;

        ret = ThyoneI_GetRpNumSlots(&numSlotsGet);
        Debug_out("ThyoneI_GetRpNumSlots", ret);
        fprintf(stdout, "Number of slots: %u\n", numSlotsGet);

        ret = ThyoneI_SetRpNumSlots(numSlotsSet);
        Debug_out("ThyoneI_SetRpNumSlots", ret);
        delay(ThyoneI_BOOT_DURATION);

        ret = ThyoneI_GetRpNumSlots(&numSlotsGet);
        Debug_out("ThyoneI_GetRpNumSlots", ret);
        fprintf(stdout, "Number of slots: %u\n", numSlotsGet);


#endif

#if 0

        uint8_t payload[11] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

        ret = ThyoneI_TransmitBroadcast(payload, sizeof(payload));
        Debug_out("ThyoneI_TransmitBroadcast", ret);


        ret = ThyoneI_TransmitMulticastExtended(16, payload, sizeof(payload));
        Debug_out("ThyoneI_TransmitUnicastExtended", ret);

        /* Send unicast to address as defined by usersetting*/
        uint32_t destAddressSet = 0x12345678;
        uint32_t destAddressGet = 0;

        /* the module will store the destination address LSB-first. But as uint32_t is also stored as LSB-first we can input the address just as an uint32_t value*/
        ret = ThyoneI_SetDestinationAddress(destAddressSet);
        Debug_out("ThyoneI_SetDestinationAddress", ret);
        delay(ThyoneI_BOOT_DURATION);

        ret = ThyoneI_GetDestinationAddress(&destAddressGet);
        Debug_out("ThyoneI_GetDestinationAddress", ret);
        fprintf(stdout, "DestinationAddress: 0x%X\n", destAddressGet);

        if(destAddressGet == destAddressSet)
        {
            ret = ThyoneI_TransmitUnicast(payload, sizeof(payload));
            Debug_out("ThyoneI_TransmitUnicast", ret);
        }

#endif

#if 0
        ret = ThyoneI_FactoryReset();
        Debug_out("facReset", ret);
        delay(500);

        ret = ThyoneI_Sleep();
        Debug_out("Sleep", ret);
        delay(2500);

        ret = ThyoneI_PinWakeup();
        Debug_out("Pin wake up", ret);

#endif

#if 0
        /* Config Local */
        ThyoneI_GPIOConfigBlock_t configLocalGPIO[ThyoneI_AMOUNT_GPIO_PINS];

        /* config GPIO 1 as output high*/
        configLocalGPIO[0].GPIO_ID = ThyoneI_GPIO_4;
        configLocalGPIO[0].function = ThyoneI_GPIO_IO_Output;
        configLocalGPIO[0].value.output = ThyoneI_GPIO_Output_Low;

        /* config GPIO 2 as input pullup*/
        configLocalGPIO[1].GPIO_ID = ThyoneI_GPIO_5;
        configLocalGPIO[1].function = ThyoneI_GPIO_IO_Input;
        configLocalGPIO[1].value.input = ThyoneI_GPIO_Input_PullUp;

        /* config GPIO 3 as pwm */
        configLocalGPIO[2].GPIO_ID = ThyoneI_GPIO_3;
        configLocalGPIO[2].function = ThyoneI_GPIO_IO_PWM;

        ThyoneI_GPIO_PwmValue_t pwmValue;
        pwmValue.period = 50; /* 50ms period */
        pwmValue.ratio = 128; /* 50% duty cycle */
        configLocalGPIO[2].value.pwm = pwmValue;

        ret = ThyoneI_GPIOLocalSetConfig(configLocalGPIO, 3);
        Debug_out("ThyoneI_GPIO_Local_Write_Config",ret);
        delay(2000);

        uint16_t number_of_configs = 0;
        memset(configLocalGPIO, 0, sizeof(configLocalGPIO)* ThyoneI_AMOUNT_GPIO_PINS);

        ret = ThyoneI_GPIOLocalGetConfig(configLocalGPIO, &number_of_configs);
        Debug_out("ThyoneI_GPIORemoteWriteConfig",ret);

        for(int i = 0; i < number_of_configs; i++)
        {
            printf("Pin %i; function: %i\n", configLocalGPIO[i].GPIO_ID, configLocalGPIO[i].function);
            if(configLocalGPIO[i].function == ThyoneI_GPIO_IO_PWM)
            {
                printf("pwm period: %i, ratio: %i \n", configLocalGPIO[i].value.pwm.period, configLocalGPIO[i].value.pwm.ratio);
            }
        }

        /* Write local */
        ThyoneI_GPIOControlBlock_t controlLocalGPIO[ThyoneI_AMOUNT_GPIO_PINS];

        /* set GPIO 1 to low */
        controlLocalGPIO[0].GPIO_ID = ThyoneI_GPIO_4;
        controlLocalGPIO[0].value.output = ThyoneI_GPIO_Output_High;

        ret = ThyoneI_GPIOLocalWrite(controlLocalGPIO, 1);
        Debug_out("ThyoneI_GPIOLocalWrite - Set GPIO 1 to low", ret);
        delay(500);

        /* Read local pin */
        /* Read GPIO 1 and 2 */
        uint16_t number_of_controls = 0;

        uint8_t GPIOToRead[2];
        GPIOToRead[0] = ThyoneI_GPIO_4;
        GPIOToRead[1] = ThyoneI_GPIO_5;

        ret = ThyoneI_GPIOLocalRead(GPIOToRead, 2, controlLocalGPIO, &number_of_controls);
        Debug_out("ThyoneI_GPIOLocalRead", ret);
        printf("GPIO %x: %x \n", controlLocalGPIO[0].GPIO_ID, controlLocalGPIO[0].value.output);
        printf("GPIO %x: %x \n", controlLocalGPIO[1].GPIO_ID, controlLocalGPIO[1].value.output);

#endif
#if 0

        /* Config remote */
        ThyoneI_GPIOConfigBlock_t configRemoteGPIO[ThyoneI_AMOUNT_GPIO_PINS];

        uint32_t destAddressSet = 0x22222222;

        /* config GPIO 1 as output high*/
        configRemoteGPIO[0].GPIO_ID = ThyoneI_GPIO_1;
        configRemoteGPIO[0].InputOutput = ThyoneI_GPIO_IO_Output;
        configRemoteGPIO[0].value.output = ThyoneI_GPIO_Output_High;

        /* config GPIO 2 as input pulldown*/
        configRemoteGPIO[1].GPIO_ID = ThyoneI_GPIO_2;
        configRemoteGPIO[1].InputOutput = ThyoneI_GPIO_IO_Input;
        configRemoteGPIO[1].value.input = ThyoneI_GPIO_Input_PullUp;

        ret = ThyoneI_GPIORemoteSetConfig(destAddressSet, configRemoteGPIO, 2);
        Debug_out("ThyoneI_GPIORemoteWriteConfig",ret);
        delay(500);

        uint16_t number_of_configs = 0;
        ret = ThyoneI_GPIORemoteGetConfig(destAddressSet, configRemoteGPIO, &number_of_configs);
        Debug_out("ThyoneI_GPIORemoteWriteConfig",ret);


        /* Write remote */
        ThyoneI_GPIOControlBlock_t controlRemoteGPIO[ThyoneI_AMOUNT_GPIO_PINS];

        /* set GPIO 1 to low */
        controlRemoteGPIO[0].GPIO_ID = ThyoneI_GPIO_1;
        controlRemoteGPIO[0].value.output = ThyoneI_GPIO_Output_Low;

        ret = ThyoneI_GPIORemoteWrite(destAddressSet, controlRemoteGPIO, 1);
        Debug_out("ThyoneI_GPIORemoteWrite - Set GPIO 1 to low", ret);
        delay(500);

        uint8_t RemoteGPIOToRead[2];
        RemoteGPIOToRead[0] = ThyoneI_GPIO_1;
        RemoteGPIOToRead[1] = ThyoneI_GPIO_2;

        uint16_t number_of_controls = 0;
        /* Read GPIO 1 and 2 */
        ret = ThyoneI_GPIORemoteRead(destAddressSet, RemoteGPIOToRead, 2, controlRemoteGPIO, &number_of_controls);
        Debug_out("ThyoneI_GPIORemoteRead", ret);
        if(ret)
        {
            printf("GPIO %x: %x \n", controlRemoteGPIO[0].GPIO_ID, controlRemoteGPIO[0].value.output);
            printf("GPIO %x: %x \n", controlRemoteGPIO[1].GPIO_ID, controlRemoteGPIO[1].value.output);
        }

#endif
    }

    ret = ThyoneI_Deinit();
    Debug_out("ThyoneI_Deinit", ret);
}


