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

#include "../drivers/TarvosIIIPlug/TarvosIIIPlug.h"
#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"


static void Application(void);

static void TarvosIIIPlug_test_function();
static void RX_test();

static void RXcallback(uint8_t* payload, uint8_t payload_length, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 1
    /* default application*/
    Application();
#elif 1
    /* enable this to test the rx capabilities */
    RX_test();
#else
    /* function to test all functions of the TarvosIII driver */
    TarvosIIIPlug_test_function();
#endif

    AbortMainLoop = true;
    return 0 ;
}

/* callback for data reception */
static void RXcallback(uint8_t* payload, uint8_t payload_length, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb, int8_t rssi)
{
    int i = 0;
    printf ("Received data from address (NetID:0x%02x,Addr:0x%02x%02x) with %d dBm:\n-> ", dest_network_id, dest_address_lsb, dest_address_msb, rssi);
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


static void Application()
{
    bool ret;
    char ascii_data[100];
    ret = TarvosIIIPlug_Init(115200, TarvosIIIPlug_CBUS_RESET, RXcallback, AddressMode_0);
    Debug_out("TarvosIIIPlug_Init", ret);

    while(1)
    {
        /* transmit data repeatedly */
        sprintf(ascii_data, "Hello World");
        ret = TarvosIIIPlug_Transmit((uint8_t*)ascii_data,strlen(ascii_data));
        Debug_out("TarvosIIIPlug Transmit", ret);

        delay(1000);
    }

    ret = TarvosIIIPlug_Deinit();
    Debug_out("TarvosIIIPlug deinit", ret);
}

/* test function to demonstrate the usage of the TarvosIIIPlug functions */
static void TarvosIIIPlug_test_function()
{
    bool ret = false;
    uint8_t serial_number[4];
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "TarvosIIIPlug driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module TarvosIIIPlug */
    ret = TarvosIIIPlug_Init(115200, TarvosIIIPlug_CBUS_RESET, RXcallback, AddressMode_0);
    Debug_out("TarvosIIIPlug_Init", ret);
    if(ret)
    {

        ret = TarvosIIIPlug_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        ret = TarvosIIIPlug_GetSerialNumber(serial_number);
        Debug_out("GetSerialNumber",ret);
        if(ret)
        {
            uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
            fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(500);

#if 0
        ret = TarvosIIIPlug_FactoryReset();
        Debug_out("facReset", ret);
        delay(500);

        ret = TarvosIIIPlug_Standby();
        Debug_out("Standby", ret);
        delay(500);
#endif

        uint8_t data[6];
        uint8_t data_length = 6;
        ret = TarvosIIIPlug_Get(TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        data[0] = 0; /* 0dbm */
        data_length = 1;
        ret = TarvosIIIPlug_Set(TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER, data, data_length);
        Debug_out("Set TXpower to 0 dBm", ret);
        delay(500);

        data[0] = 88;
        ret = TarvosIIIPlug_Get(TarvosIIIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        ret = TarvosIIIPlug_SetVolatile_TXPower(10);
        Debug_out("Set volatile TXpower to 10 dBm", ret);
        delay(500);

        ret = TarvosIIIPlug_SetVolatile_Channel(110);
        Debug_out("Set volatile Channel to 110", ret);
        delay(500);

        ret = TarvosIIIPlug_SetVolatile_DestNetID(1);
        Debug_out("Set volatile DestNetID to 1", ret);
        delay(500);

        ret = TarvosIIIPlug_SetVolatile_DestAddr(0x00,0x00);
        Debug_out("Set volatile DestAddr to 0", ret);
        delay(500);

        fprintf (stdout, "\n");

        ret = TarvosIIIPlug_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        uint8_t txpower = 10;
        ret = TarvosIIIPlug_GetDefaultTXPower(&txpower);
        Debug_out("TarvosIIIPlug_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        delay(500);
        ret = TarvosIIIPlug_SetDefaultTXPower(2);
        Debug_out("TarvosIIIPlug_SetDefaultTXPower to 2dBm", ret);
        delay(500);


        uint8_t channel = 0;
        ret = TarvosIIIPlug_GetDefaultRFChannel(&channel);
        Debug_out("TarvosIIIPlug_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        delay(500);
        ret = TarvosIIIPlug_SetDefaultRFChannel(101);
        Debug_out("TarvosIIIPlug_SetDefaultRFChannel to 101", ret);
        delay(500);


        uint8_t srcnetid = 0;
        ret = TarvosIIIPlug_GetSourceNetID(&srcnetid);
        Debug_out("TarvosIIIPlug_GetSourceNetID", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        delay(500);
        ret = TarvosIIIPlug_SetSourceNetID(17);
        Debug_out("TarvosIIIPlug_SetSourceNetID to 17", ret);
        delay(500);

        uint8_t destnetid = 0;
        ret = TarvosIIIPlug_GetDefaultDestNetID(&destnetid);
        Debug_out("TarvosIIIPlug_GetDefaultDestNetID", ret);
        fprintf (stdout,"Destination net ID is 0x%x\n",destnetid);
        delay(500);
        ret = TarvosIIIPlug_SetDefaultDestNetID(0x12);
        Debug_out("TarvosIIIPlug_SetDefaultDestNetID to 0x12", ret);
        delay(500);



        uint8_t srcaddr_lsb = 0, srcaddr_msb = 0;
        ret = TarvosIIIPlug_GetSourceAddr(&srcaddr_lsb,&srcaddr_msb);
        Debug_out("TarvosIIIPlug_GetSourceAddr", ret);
        fprintf (stdout,"Source address is 0x%x%x\n",srcaddr_lsb,srcaddr_msb);
        delay(500);
        ret = TarvosIIIPlug_SetSourceAddr(0x13,0x14);
        Debug_out("TarvosIIIPlug_SetSourceAddr to 0x1314", ret);
        delay(500);

        uint8_t destaddr_lsb = 0, destaddr_msb = 0;
        ret = TarvosIIIPlug_GetDefaultDestAddr(&destaddr_lsb,&destaddr_msb);
        Debug_out("TarvosIIIPlug_GetDefaultDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x%x\n",destaddr_lsb,destaddr_msb);
        delay(500);
        ret = TarvosIIIPlug_SetDefaultDestAddr(0x20,0x21);
        Debug_out("TarvosIIIPlug_SetDefaultDestAddr to 0x2021", ret);
        delay(500);


        uint8_t profile = 0;
        ret = TarvosIIIPlug_GetDefaultRFProfile(&profile);
        Debug_out("TarvosIIIPlug_GetDefaultRFProfile", ret);
        fprintf (stdout,"RF profile is %d\n",profile);
        delay(500);
        ret = TarvosIIIPlug_SetDefaultRFProfile(2);
        Debug_out("TarvosIIIPlug_SetDefaultRFProfile to 2", ret);
        delay(500);



        ret = TarvosIIIPlug_PinReset();
        Debug_out("TarvosIIIPlug_PinReset", ret);
        delay(500);

        fprintf(stdout, "\n");

        txpower = 88;
        ret = TarvosIIIPlug_GetDefaultTXPower(&txpower);
        Debug_out("TarvosIIIPlug_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_GetDefaultRFChannel(&channel);
        Debug_out("TarvosIIIPlug_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_GetSourceNetID(&srcnetid);
        Debug_out("TarvosIIIPlug_GetSourceNetID", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_GetDefaultDestNetID(&destnetid);
        Debug_out("TarvosIIIPlug_GetDefaultDestNetID", ret);
        fprintf (stdout,"Destination net ID is 0x%x\n",destnetid);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_GetSourceAddr(&srcaddr_lsb,&srcaddr_msb);
        fprintf (stdout,"Source address is 0x%x%x\n",srcaddr_lsb,srcaddr_msb);
        Debug_out("TarvosIIIPlug_GetSourceAddr", ret);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_GetDefaultDestAddr(&destaddr_lsb,&destaddr_msb);
        Debug_out("TarvosIIIPlug_GetDefaultDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x%x\n",destaddr_lsb,destaddr_msb);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_GetDefaultRFProfile(&profile);
        Debug_out("TarvosIIIPlug_GetDefaultRFProfile", ret);
        fprintf (stdout,"RF profile is %d\n",profile);
        fprintf (stdout, "\n");
        delay(500);


        ret = TarvosIIIPlug_FactoryReset();
        Debug_out("TarvosIIIPlug_FactoryReset", ret);
        delay(500);

        ret = TarvosIIIPlug_GetDefaultTXPower(&txpower);
        Debug_out("TarvosIIIPlug_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        fprintf (stdout, "\n");
        delay(500);

        ret = TarvosIIIPlug_Reset();
        Debug_out("TarvosIIIPlug_Reset", ret);
        delay(500);

		/* Transmit "Raspberry Alive" */
        uint8_t payload[15] = { 'R','a','s','p','b','e','r','r','y',' ','A','l','i','v','e' };

        ret = TarvosIIIPlug_Transmit(payload,sizeof(payload));
        Debug_out("Transmit", ret);
        delay(500);

        /* Transmit "Raspberry Alive" on channel 106 to address 0 0 0 */
        ret = TarvosIIIPlug_Transmit_Extended(payload,sizeof(payload),106,0,0,0);
        Debug_out("Transmit extended", ret);
        delay(500);

        ret = TarvosIIIPlug_Deinit();
        Debug_out("TarvosIIIPlug_Deinit", ret);
    }
}

/* test function to only stay on RX */
static void RX_test()
{
    bool ret = false;

    /* initialize the module TarvosIIIPlug */
    ret = TarvosIIIPlug_Init(115200, TarvosIIIPlug_CBUS_RESET, RXcallback, AddressMode_0);
    Debug_out("TarvosIIIPlug_Init", ret);

    ret = TarvosIIIPlug_PinReset();
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
