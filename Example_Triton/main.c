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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "../drivers/Triton/Triton.h"
#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"


static void RX_test(void);

static void Triton_test_function(void);

static void RXcallback(uint8_t* payload, uint8_t payload_length, uint8_t dest_address, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 0
    /* function to test all functions of the Triton driver */
    Triton_test_function();
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
static void RXcallback(uint8_t* payload, uint8_t payload_length, uint8_t dest_address, int8_t rssi)
{
    int i = 0;
    printf ("Received data from address (0x%02x) with %d dBm:\n-> ", dest_address, rssi);
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

    /* initialize the module Triton */
    ret = Triton_Init(9600, Triton_PIN_RESET, Triton_PIN_WAKEUP, RXcallback);
    Debug_out("Triton_Init", ret);

    ret = Triton_PinReset();
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

/* test function to demonstrate the usage of the Triton functions */
static void Triton_test_function()
{
    bool ret = false;
    uint8_t serial_number[4];
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Triton driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module Triton */
    ret = Triton_Init(9600, Triton_PIN_RESET, Triton_PIN_WAKEUP, RXcallback);
    Debug_out("Triton_Init", ret);

    if(ret)
    {
        ret = Triton_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        ret = Triton_GetSerialNumber(serial_number);
        Debug_out("GetSerialNumber",ret);
        if(ret)
        {
            uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
            fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(500);

#if 0
        ret = Triton_FactoryReset();
        Debug_out("facReset", ret);
        delay(500);
#endif

        ret = Triton_Sleep();
        Debug_out("Sleep", ret);
        delay(500);

        ret = Triton_PinWakeup();
        Debug_out("Wakeup", ret);
        fprintf (stdout, "\n");
        delay(500);

        uint8_t data[6];
        uint8_t data_length = 6;
        ret = Triton_Get(Triton_CMD_SETGET_OPTION_RFPOWERLEVEL, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        data[0] = 0; /* 0dbm */
        data_length = 1;
        ret = Triton_Set(Triton_CMD_SETGET_OPTION_RFPOWERLEVEL, data, data_length);
        Debug_out("Set TXpower to 0 dBm", ret);
        delay(500);

        data[0] = 88;
        ret = Triton_Get(Triton_CMD_SETGET_OPTION_RFPOWERLEVEL, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        ret = Triton_SetVolatile_TXPower(10);
        Debug_out("Set volatile TXpower to 10 dBm", ret);
        delay(500);

        ret = Triton_SetVolatile_Channel(10);
        Debug_out("Set volatile Channel to 10", ret);
        delay(500);

        fprintf (stdout, "\n");

        ret = Triton_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        uint8_t txpower = 10;
        ret = Triton_GetDefaultTXPower(&txpower);
        Debug_out("Triton_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        delay(500);
        ret = Triton_SetDefaultTXPower(2);
        Debug_out("Triton_SetDefaultTXPower to 2dBm", ret);
        delay(500);

        uint8_t channel = 0;
        ret = Triton_GetDefaultRFChannel(&channel);
        Debug_out("Triton_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        delay(500);
        ret = Triton_SetDefaultRFChannel(5);
        Debug_out("Triton_SetDefaultRFChannel to 5", ret);
        delay(500);

        uint8_t srcnetid = 0;
        ret = Triton_GetSubnet(&srcnetid);
        Debug_out("Triton_GetSubnet", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        delay(500);
        ret = Triton_SetSubnet(17);
        Debug_out("Triton_SetSubnet to 17", ret);
        delay(500);

        uint8_t srcaddr = 0;
        ret = Triton_GetSourceAddr(&srcaddr);
        Debug_out("Triton_GetSourceAddr", ret);
        fprintf (stdout,"Source address is 0x%x\n",srcaddr);
        delay(500);
        ret = Triton_SetSourceAddr(0x13);
        Debug_out("Triton_SetSourceAddr to 0x13", ret);
        delay(500);

        uint8_t profile = 0;
        ret = Triton_GetDefaultRFProfile(&profile);
        Debug_out("Triton_GetDefaultRFProfile", ret);
        fprintf (stdout,"RF profile is %d\n",profile);
        delay(500);
        ret = Triton_SetDefaultRFProfile(2);
        Debug_out("Triton_SetDefaultRFProfile to 2", ret);
        delay(500);

        ret = Triton_PinReset();
        Debug_out("Triton_PinReset", ret);
        delay(500);

        fprintf(stdout, "\n");

        txpower = 88;
        ret = Triton_GetDefaultTXPower(&txpower);
        Debug_out("Triton_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        fprintf (stdout, "\n");
        delay(500);

        ret = Triton_GetDefaultRFChannel(&channel);
        Debug_out("Triton_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        fprintf (stdout, "\n");
        delay(500);

        ret = Triton_GetSubnet(&srcnetid);
        Debug_out("Triton_GetSubnet", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        fprintf (stdout, "\n");
        delay(500);

        ret = Triton_GetSourceAddr(&srcaddr);
        fprintf (stdout,"Source address is 0x%x\n",srcaddr);
        Debug_out("Triton_GetSourceAddr", ret);
        fprintf (stdout, "\n");
        delay(500);

        ret = Triton_GetDefaultRFProfile(&profile);
        Debug_out("Triton_GetDefaultRFProfile", ret);
        fprintf (stdout,"RF profile is %d\n",profile);
        fprintf (stdout, "\n");
        delay(500);

        ret = Triton_FactoryReset();
        Debug_out("Triton_FactoryReset", ret);
        delay(500);

        ret = Triton_GetDefaultTXPower(&txpower);
        Debug_out("Triton_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        fprintf (stdout, "\n");
        delay(500);

        // Transmit "Raspberry Alive" to address 0
        uint8_t payload[15] = { 'R','a','s','p','b','e','r','r','y',' ','A','l','i','v','e' };
        ret = Triton_Transmit_Extended(payload,sizeof(payload),0);
        Debug_out("Transmit extended", ret);
        delay(500);
    }
    ret = Triton_Deinit();
    Debug_out("Triton_Deinit", ret);
}

