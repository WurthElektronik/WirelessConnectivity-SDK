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

#include "../drivers/Metis/Metis.h"
#include "../drivers/global/global.h"
#include "../drivers/WE-common.h"

#define Metis_CBUS_RESET PIN_FTDI_CBUS0

static void Metis_Application();
static void Metis_rx_test();
static void Metis_test_function();

static void RXcallback(uint8_t* payload, uint8_t payload_length, int8_t rssi);

pthread_t thread_main;

bool AbortMainLoop = false;

//
void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 1
    /* application to send Test M-Bus Frame */
    Metis_Application();
#elif 1
    /* function to test all functions of the wMBus driver */
    Metis_test_function();
#else
    /* enable this to test the rx capabilities */
    Metis_rx_test();
#endif

    AbortMainLoop = true;
    return 0 ;
}

/* callback for data reception */
static void RXcallback(uint8_t* payload, uint8_t payload_length, int8_t rssi)
{
    int i = 0;
    printf ("Received data with %d dBm:\n-> ",rssi);
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

/* application to send Test M-Bus Frame */
static void Metis_Application()
{
    bool ret = false;
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Metis driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module Metis */
    ret = Metis_Init(9600, Metis_CBUS_RESET, MBus_Frequency_169, MBus_Mode_169_N2a, true, RXcallback);
    Debug_out("Metis init", ret);

    static uint8_t APP_Data[140] =
    {
        0x48, /* Length Field*/

        /*0*/
        0x44,                                                                                  /* C-Feld */
        0xA2,0x05,                                                                             /* M-Feld (AMB)*/
        0x11,0x47,0x15,0x08,                                                                   /* ID (Funkmodul)*/
        0x01,                                                                                  /* Version (2) */
        0x37,                                                                                  /* Radio Converter (meter side) */

        /*9*/
        0x72,                                                                                  /* CI-Feld (12 Byte Header)*/
        0x78,0x56,0x34,0x12,                                                                   /* ID (Zähler) */
        0xA2,0x05,                                                                             /* M-Feld (AMB) */
        0x01,                                                                                  /* Version (1) */
        0x37,                                                                                  /* Radio Converter (meter side) */
        0x00,                                                                                  /* AccCounter */
        0x00,                                                                                  /* Statusbyte */
        0x00,                                                                                  /* Signaturwort, hier: encryption mode 5 ohne verlüsselte blöcke */
        0x05,

        /*22*/
        /*-*/
        0x2F,                                                                                  /* AES Verification / Idle Filler */
        0x2F,                                                                                  /* AES Verification / Idle Filler */

        /*24*/
        0x0E,                                                                                  /* DIV 12Stellige BCD Zahl */
        0x13,                                                                                  /* VIF Volumen in l */
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,

        /*32*/
        0x0D,                                                                                   /* DIV Variable Datenlänge */
        0xFD,                                                                                   /* VIF Zweite Erweiterungstabelle */
        0x11,                                                                                   /* VIFE Abnehmer */
        0x20,                                                                                   /* 23 Stellen ASCII String */
        'e',
        'd',
        /*-*/
        'o',
        'M',
        ' ',
        'S', /* = index 41 */
        ' ',
        ' ',
        'A',/* = index 44 */
        ' ',
        'F',
        'F',
        ' ',
        'H',
        'b',
        'm',
        'G',
        ' ',
        /*-*/
        's',
        's',
        'e',
        'l',
        'e',
        'r',
        'i',
        'w',
        ' ',
        'R',
        'E',
        'B',
        'M',
        'A',
        /*70*/

        0x2F,
        0x2F
    };

    /* transmit data repeatedly */
    while(1)
    {
        ret = Metis_Transmit(APP_Data);
        Debug_out("Transmit", ret);
        delay(1000);
    }
}

/* function to test all functions of the TarvosIII driver */
static void Metis_test_function()
{
    bool ret = false;
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Metis driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module TarvosII-M */
    ret = Metis_Init(9600, Metis_CBUS_RESET, MBus_Frequency_169, MBus_Mode_169_N2a, true, RXcallback);
    Debug_out("Metis init", ret);

    if(ret)
    {
        ret = Metis_PinReset();
        Debug_out("PinReset", ret);
        delay(500);

        ret = Metis_Reset();
        Debug_out("Reset", ret);
        delay(500);

        uint8_t serial_number[4];
        ret = Metis_GetSerialNumber(serial_number);
        Debug_out("GetSerialNumber",ret);
        if(ret)
        {
        uint32_t serial_number_dez = serial_number[1] << 16 | serial_number[2] << 8 | serial_number[3];
        fprintf (stdout, COLOR_CYAN "Serial number %02d.%06d ",serial_number[0],serial_number_dez);
            fprintf (stdout, "(0x%02x.%02x.%02x.%02x)\n" COLOR_RESET,serial_number[0],serial_number[1],serial_number[2],serial_number[3]);
        }
        delay(1000);

        uint8_t uartEnable;
        ret = Metis_GetUartOutEnable(&uartEnable);
        Debug_out("GetUartOutEnable", ret);
        fprintf (stdout,"UartOutEnable is %d \n",uartEnable);

        uint8_t response[2];
        uint8_t length;
        ret = Metis_GetMultiple(Metis_USERSETTING_MEMPOSITION_RSSI_ENABLE, 0x02, response, &length);
        Debug_out("GetMultiple", ret);
        fprintf(stdout, "RSSI Enable %x and ModePreselect %d\n", response[0], response[1]);

        uint8_t aesEnable;
        ret = Metis_GetAESEnable(&aesEnable);
        Debug_out("GetAESEnable", ret);
        fprintf (stdout,"AESEnable is %d \n",aesEnable);


        uint8_t data[6];
        uint8_t data_length = 6;
        ret = Metis_Get(Metis_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        data[0] = 0; /* 0dbm */
        data_length = 1;
        ret = Metis_Set(Metis_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER, data, data_length);
        Debug_out("Set TXpower to 0 dBm", ret);
        delay(500);

        data[0] = 88;
        ret = Metis_Get(Metis_USERSETTING_MEMPOSITION_DEFAULTRFTXPOWER, data, &data_length);
        Debug_out("Get TXpower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",data[0]);
        delay(500);

        fprintf (stdout, "\n");

        int8_t txpower = 10;
        ret = Metis_GetDefaultTXPower(&txpower);
        Debug_out("Metis_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        delay(500);

        ret = Metis_SetDefaultTXPower(2);
        Debug_out("Metis_SetDefaultTXPower to 2dBm", ret);
        delay(500);

        ret = Metis_GetDefaultTXPower(&txpower);
        Debug_out("Metis_GetDefaultTXPower", ret);
        fprintf (stdout,"TXPower is %d dBm\n",txpower);
        delay(500);

        uint8_t mode = 0;
        ret = Metis_GetModePreselect(&mode);
        Debug_out("Metis_GetModePreselect", ret);
        fprintf (stdout,"Mode Preselect is %d\n",mode);
        delay(500);

        uint8_t payload[] = {0x09, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
        ret = Metis_Transmit(payload);
        Debug_out("Transmit", ret);
        delay(500);

        ret = Metis_FactoryReset();
        Debug_out("Metis_FactoryReset()", ret);
        delay(1000);

        ret = Metis_Reset();
        Debug_out("Metis_Reset", ret);
        delay(500);

        ret = Metis_GetUartOutEnable(&uartEnable);
        Debug_out("GetUartOutEnable", ret);
        fprintf (stdout,"UartOutEnable is %d \n",uartEnable);
    }
    ret = Metis_Deinit();
    Debug_out("Metis deinit", ret);
    delay(500);
}

/* test function to only stay on RX */
static void Metis_rx_test()
{
    bool ret = false;
    uint8_t driver_version[3];

    ret = GetDriverVersion(driver_version);
    Debug_out("GetDriverVersion",ret);
    if(ret)
    {
        fprintf (stdout, COLOR_CYAN "Metis driver version %d.%d.%d\n" COLOR_RESET,driver_version[0],driver_version[1],driver_version[2]);
    }
    delay(500);

    /* initialize the module Metis */
    ret = Metis_Init(9600, Metis_CBUS_RESET, MBus_Frequency_169, MBus_Mode_169_N2a, true, RXcallback);
    Debug_out("Metis init", ret);

    printf ("Waiting for incoming data\n");
    fflush (stdout) ;
    while(1)
    {
        /* do nothing, just stay on RX */
        delay(1000);
    }
}
