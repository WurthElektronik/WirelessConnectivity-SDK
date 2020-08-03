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

#include "../drivers/Titania/Titania.h"
#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"


static void Titania_test_function();
static void Titania_rx_test();

static void RXcallback(uint8_t* payload, uint8_t payload_length, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

//
void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 0
    /* enable this to test the rx capabilities */
   Titania_rx_test();
#else
    /* function to test all functions of the Titania driver */
    Titania_test_function();
#endif

    AbortMainLoop = true;
    return 0 ;
}

//    /* callback for data reception */
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

static void Titania_test_function()
{
    bool ret = false;
    uint8_t serial_number[4];
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Titania driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module Titania */
    ret = Titania_Init(9600, Titania_PIN_RESET, RXcallback, AddressMode_0);
    Debug_out("Titania init", ret);

    if(ret)
    {
        ret = Titania_PinReset();
        Debug_out("PinReset", ret);
        delay(500);


        ret = Titania_Reset();
        Debug_out("Reset", ret);
        delay(500);

        ret = Titania_GetSerialNumber(serial_number);
        Debug_out("GetSerialNumber",ret);
        if(ret)
        {
            uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
            fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(1000);

        uint8_t data[6];
        uint8_t data_length = 6;
        ret = Titania_Get(Titania_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        data[0] = 0; /* 0dbm */
        data_length = 1;
        ret = Titania_Set(Titania_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER, data, data_length);
        Debug_out("Set TXpower to 0 dBm", ret);
        delay(500);

        data[0] = 88;
        ret = Titania_Get(Titania_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        fprintf (stdout, "\n");

        uint8_t numretries[1];
        uint8_t numretries_len = 1;
        ret = Titania_Get(Titania_USERSETTING_MEMPOSITION_NUMRETRYS, numretries, &numretries_len);
        Debug_out("GET: NUMRETRYS", ret);
        fprintf(stdout, "numretries is %d\n", numretries[0]);
        delay(500);

        uint8_t opMode[1];
        uint8_t opMode_len = 1;
        ret = Titania_Get(Titania_USERSETTING_MEMPOSITION_OP_MODE, opMode, &opMode_len);
        Debug_out("GET: opMode", ret);
        fprintf(stdout, "OpMode is %d\n", opMode[0]);
        delay(500);

        uint8_t cfgFlags[2];
        uint8_t cfgFlags_len = 2;
        ret = Titania_Get(Titania_USERSETTING_MEMPOSITION_CFG_FLAGS, cfgFlags, &cfgFlags_len);
        Debug_out("GET: CFGFlags\n", ret);
        fprintf(stdout, "CFG Flags: %x %x\n", cfgFlags[0], cfgFlags[1]);
        delay(500);

        uint8_t baudrate[4];
        uint8_t baudrate_len = 4;
        ret = Titania_Get(Titania_USERSETTING_MEMPOSITION_UARTBAUDRATE, baudrate, &baudrate_len);
        Debug_out("GET: baudrate\n", ret);
        fprintf(stdout, "Baudrate: %x %x %x %x\n", baudrate[0], baudrate[1], baudrate[2], baudrate[3]);

        delay(500);

        fprintf (stdout, "\n");

        ret = Titania_SetVolatile_TXPower(10);
        Debug_out("Set volatile TXpower to 10 dBm", ret);
        delay(500);

        ret = Titania_SetVolatile_Channel(2);
        Debug_out("Set volatile Channel to 2", ret);
        delay(500);

        ret = Titania_SetVolatile_DestNetID(1);
        Debug_out("Set volatile DestNetID to 1", ret);
        delay(500);

        ret = Titania_SetVolatile_DestAddr(0x00,0x00);
        Debug_out("Set volatile DestAddr to 0", ret);
        delay(500);

        fprintf (stdout, "\n");

        ret = Titania_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        int8_t txpower = 10;
        ret = Titania_GetDefaultTXPower(&txpower);
        Debug_out("Titania_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        delay(500);

        ret = Titania_SetDefaultTXPower(2);
        Debug_out("Titania_SetDefaultTXPower to 2dBm", ret);
        delay(500);

        ret = Titania_GetDefaultTXPower(&txpower);
        Debug_out("Titania_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        delay(500);


        uint8_t channel = 0;
        ret = Titania_GetDefaultRFChannel(&channel);
        Debug_out("Titania_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        delay(500);

        ret = Titania_SetDefaultRFChannel(1);
        Debug_out("Titania_SetDefaultRFChannel to 1", ret);
        delay(500);

        ret = Titania_GetDefaultRFChannel(&channel);
        Debug_out("Titania_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        fprintf (stdout, "\n");
        delay(500);


        uint8_t srcnetid = 0;
        ret = Titania_GetSourceNetID(&srcnetid);
        Debug_out("Titania_GetSourceNetID", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        delay(500);

        ret = Titania_SetSourceNetID(17);
        Debug_out("Titania_SetSourceNetID to 17", ret);
        delay(500);

        ret = Titania_GetSourceNetID(&srcnetid);
        Debug_out("Titania_GetSourceNetID", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        fprintf (stdout, "\n");
        delay(500);


        uint8_t destnetid = 0;
        ret = Titania_GetDefaultDestNetID(&destnetid);
        Debug_out("Titania_GetDefaultDestNetID", ret);
        fprintf (stdout,"Destination net ID is 0x%x\n",destnetid);
        delay(500);

        ret = Titania_SetDefaultDestNetID(0x00);
        Debug_out("Titania_SetDefaultDestNetID to 0x12", ret);
        delay(500);

        ret = Titania_GetDefaultDestNetID(&destnetid);
        Debug_out("Titania_GetDefaultDestNetID", ret);
        fprintf (stdout,"Destination net ID is 0x%x\n",destnetid);
        fprintf (stdout, "\n");
        delay(500);


        uint8_t srcaddr_lsb = 0;
        ret = Titania_GetSourceAddr(&srcaddr_lsb);
        Debug_out("Titania_GetSourceAddr", ret);
        fprintf (stdout,"Source address is 0x%x\n",srcaddr_lsb);
        delay(500);

        ret = Titania_SetSourceAddr(0x13,0x14);
        Debug_out("Titania_SetSourceAddr to 0x13", ret);
        delay(500);

        ret = Titania_GetSourceAddr(&srcaddr_lsb);
        fprintf (stdout,"Source address is 0x%x\n",srcaddr_lsb);
        Debug_out("Titania_GetSourceAddr", ret);
        fprintf (stdout, "\n");
        delay(500);

        uint8_t destaddr_lsb = 0;
        ret = Titania_GetDefaultDestAddr(&destaddr_lsb);
        Debug_out("Titania_GetDefaultDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x\n",destaddr_lsb);
        delay(500);

        ret = Titania_SetDefaultDestAddr(0x20,0x21);
        Debug_out("Titania_SetDefaultDestAddr to 0x20", ret);
        delay(500);

        ret = Titania_GetDefaultDestAddr(&destaddr_lsb);
        Debug_out("Titania_GetDefaultDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x\n",destaddr_lsb);
        fprintf (stdout, "\n");
        delay(500);

        fprintf (stdout, "\n");

        ret = Titania_Reset();
        Debug_out("Reset", ret);
        delay(500);

        ret = Titania_GetDefaultDestAddr(&destaddr_lsb);
        Debug_out("Titania_GetDefaultDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x\n",destaddr_lsb);
        fprintf (stdout, "\n");
        delay(500);


        uint8_t payload[16] = { 'H','e','l','l','o',' ','I',' ','a','m', ' ', 'A','m','b','e','r'};
        ret = Titania_Transmit(payload,sizeof(payload));
        Debug_out("Transmit", ret);
        delay(500);

        /* Transmit "Hello, I am Amber" on channel 2 to address 0 0 */
        ret = Titania_Transmit_Extended(payload,sizeof(payload),2,0,0);
        Debug_out("Transmit extended", ret);
        delay(500);

    }

    ret = Titania_Deinit();
    Debug_out("Titania deinit", ret);
}

/* test function to only stay on RX */
static void Titania_rx_test()
{
    bool ret = false;
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Titania driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module Titania */
    ret = Titania_Init(9600, Titania_PIN_RESET, RXcallback, AddressMode_0);
    Debug_out("Titania init", ret);

    uint8_t payload[16] = { 'H','e','l','l','o',' ','I',' ','a','m', ' ', 'A','m','b','e','r'};
    int i = 0;
    for (i=0;i<5;i++)
    {
        ret = Titania_Transmit(payload,sizeof(payload));
        Debug_out("Transmit", ret);
        delay(500);
    }

    ret = Titania_PinReset();
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

