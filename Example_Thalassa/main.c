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

#include "../drivers/Thalassa/Thalassa.h"
#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"


static void Thalassa_test_function();
static void Thalassa_rx_test();
static void RXcallback(uint8_t* payload, uint8_t payload_length, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

//
void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 1
    /* enable this to test the rx capabilities */
   Thalassa_rx_test();
#else
    /* function to test all functions of the Thalassa driver */
    Thalassa_test_function();
#endif

    AbortMainLoop = true;
    return 0 ;
}
//
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

static void Thalassa_test_function()
{
    bool ret = false;
    uint8_t driver_version[3];
    uint8_t serial_number[4];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Thalassa driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(200);

    /* initialize the module Thalassa */
    ret = Thalassa_Init(38400, Thalassa_PIN_RESET, RXcallback, AddressMode_0);
    Debug_out("Thalassa init", ret);
    delay(200);

    if(ret)
    {
        ret = Thalassa_GetSerialNumber(serial_number);
        Debug_out("Thalassa_GetSerialNumber", ret);
        if(ret)
        {
            uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
            fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(200);

        ret = Thalassa_Reset();
        Debug_out("Thalassa reset", ret);
        delay(200);

        fprintf(stdout, "\n");

        uint8_t data_length = 2;
        uint8_t data[data_length];

        ret = Thalassa_GetMultiple(Thalassa_USERSETTING_MEMPOSITION_SOURCENETID, 2, data, &data_length);
        Debug_out("GetMultiple", ret);
        fprintf(stdout, "SourceNetId is %d , SourceAddressLSB is %d\n", data[0], data[1]);
        delay(200);

        uint8_t numretries[1];
        uint8_t numretries_len = 1;
        ret = Thalassa_Get(Thalassa_USERSETTING_MEMPOSITION_NUMRETRYS, numretries, &numretries_len);
        Debug_out("GET: NUMRETRYS", ret);
        fprintf(stdout, "numretries is %d\n", numretries[0]);
        delay(500);

        numretries[0] = 0;
        ret = Thalassa_Set(Thalassa_USERSETTING_MEMPOSITION_NUMRETRYS, numretries, 1);
        Debug_out("SET: NUMRETRIES", ret);
        delay(200);

        ret = Thalassa_Get(Thalassa_USERSETTING_MEMPOSITION_NUMRETRYS, numretries, &numretries_len);
        Debug_out("GET: NUMRETRYS", ret);
        fprintf(stdout, "numretries is %d\n", numretries[0]);
        delay(200);

        uint8_t opMode[1];
        uint8_t opMode_len = 1;
        ret = Thalassa_Get(Thalassa_USERSETTING_MEMPOSITION_OP_MODE, opMode, &opMode_len);
        Debug_out("GET: opMode", ret);
        fprintf(stdout, "OpMode is %d\n", opMode[0]);
        delay(200);

        uint8_t cfgFlags[2];
        uint8_t cfgFlags_len = 2;
        ret = Thalassa_Get(Thalassa_USERSETTING_MEMPOSITION_CFG_FLAGS, cfgFlags, &cfgFlags_len);
        Debug_out("GET: CFGFlags", ret);
        fprintf(stdout, "CFG Flags: %x %x\n", cfgFlags[0], cfgFlags[1]);
        delay(200);

        fprintf (stdout, "\n");

        ret = Thalassa_SetVolatile_RFChannel(110);
        Debug_out("Set volatile Channel to 110", ret);
        delay(200);

        ret = Thalassa_SetVolatile_DestNetID(1);
        Debug_out("Set volatile DestNetID to 1", ret);
        delay(200);

        ret = Thalassa_SetVolatile_DestAddr(0x00);
        Debug_out("Set volatile DestAddr to 0", ret);
        delay(200);

        fprintf (stdout, "\n");

        uint8_t channel = -1;
        ret = Thalassa_GetDefaultRFChannel(&channel);
        Debug_out("Thalassa_GetDefaultRFChannel", ret);
        fprintf (stdout,"RFChannel is %d\n",channel);
        delay(200);

        uint8_t srcnetid = -1;
        ret = Thalassa_GetSourceNetID(&srcnetid);
        Debug_out("Thalassa_GetSourceNetID", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        delay(200);

        ret = Thalassa_SetSourceNetID(1);
        Debug_out("Thalassa_SetSourceNetId", ret);
        delay(200);

        ret = Thalassa_GetSourceNetID(&srcnetid);
        Debug_out("Thalassa_GetSourceNetID", ret);
        fprintf (stdout,"Source net ID is %d\n",srcnetid);
        delay(200);

        uint8_t destnetid = -1;
        ret = Thalassa_GetDefaultDestNetID(&destnetid);
        Debug_out("Thalassa_GetDefaultDestNetID", ret);
        fprintf (stdout,"Destination net ID is 0x%x\n",destnetid);
        delay(200);

        ret = Thalassa_SetDefaultDestNetID(1);
        Debug_out("Thalassa_SetDefaultDestNetID", ret);
        delay(200);

        ret = Thalassa_GetDefaultDestNetID(&destnetid);
        Debug_out("Thalassa_GetDefaultDestNetID", ret);
        fprintf (stdout,"Destination net ID is 0x%x\n",destnetid);
        delay(200);

        uint8_t srcaddr_lsb = -1;
        ret = Thalassa_GetSourceAddr(&srcaddr_lsb);
        Debug_out("Thalassa_GetSourceAddr", ret);
        fprintf (stdout,"Source address is 0x%x\n",srcaddr_lsb);
        delay(200);

        ret = Thalassa_SetSourceAddr(0x01);
        Debug_out("Thalassa_SetSourceAddr", ret);
        delay(200);

        ret = Thalassa_GetSourceAddr(&srcaddr_lsb);
        Debug_out("Thalassa_GetSourceAddr", ret);
        fprintf (stdout,"Source address is 0x%x\n",srcaddr_lsb);
        delay(200);

        uint8_t destaddr_lsb = -1;
        ret = Thalassa_GetDefaultDestAddr(&destaddr_lsb);
        Debug_out("Thalassa_GetDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x\n",srcaddr_lsb);
        delay(200);

        ret = Thalassa_SetDefaultDestAddr(0x01);
        Debug_out("Thalassa_SetDestAddr", ret);
        delay(200);

        ret = Thalassa_GetDefaultDestAddr(&destaddr_lsb);
        Debug_out("Thalassa_GetDefaultDestAddr", ret);
        fprintf (stdout,"Destination address is 0x%x\n",destaddr_lsb);
        delay(200);

        uint8_t payload[16] = { 'H','e','l','l','o',' ','I',' ','a','m', ' ', 'A','m','b','e','r'};
        ret = Thalassa_Transmit(payload, sizeof(payload));
        Debug_out("Thalassa Transmit", ret);
        delay(200);

        ret = Thalassa_Transmit_Extended(payload, sizeof(payload), 79, 0, 0);
        Debug_out("Thalassa Transmit_Extended", ret);
        delay(200);
    }

    ret = Thalassa_Deinit();
    Debug_out("Thalassa deinit", ret);
}

/* test function to only stay on RX */
static void Thalassa_rx_test()
{
    bool ret = false;
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Thalassa driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module Thalassa */
    ret = Thalassa_Init(38400, Thalassa_PIN_RESET, RXcallback, AddressMode_0);
    Debug_out("Thalassa init", ret);

    ret = Thalassa_PinReset();
    Debug_out("PinReset", ret);
    delay(500);

    /* transmit data repeatedly */
    while(1)
    {
        uint8_t payload[16] = { 'H','e','l','l','o',' ','I',' ','a','m', ' ', 'A','m','b','e','r'};
        ret = Thalassa_Transmit(payload, sizeof(payload));
        Debug_out("Thalassa Transmit", ret);
        delay(1000);
    }
}
