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
 * COPYRIGHT (c) 2021 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "../drivers/ProteusIII_SPI/ProteusIII_SPI.h"
#include "../drivers/global/global.h"
#include "../drivers/WE-common.h"

static void ProteusIII_SPI_test_function();
static void ProteusIII_SPI_wait4connect_function();
static void ProteusIII_SPI_scanNconnect_function();

static void SetCallbacks(ProteusIII_SPI_CallbackConfig_t *callbackConfigP);

static void RXcallback(uint8_t* payload, uint16_t payload_length, uint8_t* BTMAC, int8_t rssi);
static void Connectcallback(uint8_t* BTMAC);
static void Securitycallback(uint8_t* BTMAC, ProteusIII_SPI_Security_t security_state);
static void Passkeycallback(uint8_t* BTMAC);
static void DisplayPasskeycallback(ProteusIII_SPI_DisplayPasskeyAction_t action, uint8_t* BTMAC, uint8_t* passkey);
static void Disconnectcallback();
static void Channelopencallback(uint8_t* BTMAC, uint16_t max_payload);
static void Phyupdatecallback(uint8_t* BTMAC, uint8_t phy_rx, uint8_t phy_tx);

pthread_t thread_main;

bool AbortMainLoop = false;

//
void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 0
    ProteusIII_SPI_scanNconnect_function();
#elif 1
    ProteusIII_SPI_wait4connect_function();
#else
    /* function to test all functions of the ProteusIII_SPI driver */
    ProteusIII_SPI_test_function();
#endif

    AbortMainLoop = true;
    return 0 ;
}

/* callback for data reception */
static void RXcallback(uint8_t* payload, uint16_t payload_length, uint8_t* BTMAC, int8_t rssi)
{
    int i = 0;
    printf (COLOR_RED "Received data from device with BTMAC (0x%02x%02x%02x%02x%02x%02x) with RSSI: %d dBm:\n-> " COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5], rssi);
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

static void Connectcallback(uint8_t* BTMAC)
{
    printf (COLOR_RED "Connected to device with BTMAC (0x%02x%02x%02x%02x%02x%02x) " COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5]);
    printf ("\n") ;
    fflush (stdout) ;
}

static void Securitycallback(uint8_t* BTMAC, ProteusIII_SPI_Security_t security_state)
{
    printf (COLOR_RED "Security to device with BTMAC (0x%02x%02x%02x%02x%02x%02x) established " COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5]);
    printf ("\n") ;
    fflush (stdout) ;
}

static void Passkeycallback(uint8_t* BTMAC)
{
    printf (COLOR_RED "Passkey request from device with BTMAC (0x%02x%02x%02x%02x%02x%02x) " COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5]);
    printf ("\n") ;
    fflush (stdout) ;

    uint8_t passkey[6] = {'1','2','3','1','2','3'};

    ProteusIII_SPI_Passkey(passkey);
}

static void DisplayPasskeycallback(ProteusIII_SPI_DisplayPasskeyAction_t action, uint8_t* BTMAC, uint8_t* passkey)
{
    printf (COLOR_RED "Passkey request from device with BTMAC (0x%02x%02x%02x%02x%02x%02x) " COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5]);
    printf (COLOR_RED "and pass key (%c%c%c%c%c%c) " COLOR_CYAN, passkey[0],passkey[1],passkey[2],passkey[3],passkey[4],passkey[5]);
    printf ("\n") ;
    fflush (stdout) ;

    if(ProteusIII_SPI_DisplayPasskeyAction_PleaseConfirm == action)
	{
		/* confirm key */
		ProteusIII_SPI_NumericCompareConfirm(true);
	}
}

static void Disconnectcallback()
{
    printf (COLOR_RED "Disconnected" COLOR_CYAN);
    printf ("\n") ;
    fflush (stdout) ;
}

static void Channelopencallback(uint8_t* BTMAC, uint16_t max_payload)
{
    printf (COLOR_RED "Channel opened to BTMAC (0x%02x%02x%02x%02x%02x%02x) with maximum payload: %d" COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5], max_payload);
    printf ("\n") ;
    fflush (stdout) ;
}

static void Phyupdatecallback(uint8_t* BTMAC, uint8_t phy_rx, uint8_t phy_tx)
{
    printf (COLOR_RED "Phy of connection to BTMAC (0x%02x%02x%02x%02x%02x%02x) updated (RX: %dMBit, TX: %dMBit)" COLOR_CYAN, BTMAC[0],BTMAC[1],BTMAC[2],BTMAC[3],BTMAC[4],BTMAC[5], phy_rx,phy_tx);
    printf ("\n") ;
    fflush (stdout) ;
}

static void SetCallbacks(ProteusIII_SPI_CallbackConfig_t *callbackConfigP)
{
    callbackConfigP->rxCb = RXcallback;
    callbackConfigP->connectCp = Connectcallback;
    callbackConfigP->disconnectCb = Disconnectcallback;
    callbackConfigP->channelOpenCb = Channelopencallback;
    callbackConfigP->securityCb = Securitycallback;
    callbackConfigP->passkeyCb = Passkeycallback;
    callbackConfigP->displayPasskeyCb = DisplayPasskeycallback;
    callbackConfigP->phyUpdateCb = Phyupdatecallback;
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

/* function testing different ProteusIII_SPI functions */
static void ProteusIII_SPI_test_function()
{
    bool ret = false;
    uint8_t btmac[6];

    /* initialize the module ProteusIII_SPI */
    ProteusIII_SPI_CallbackConfig_t callbackConfiguration;
    SetCallbacks(&callbackConfiguration);

    ret = ProteusIII_SPI_Init(ProteusIII_SPI_ConfigIndex_Mode2_activehigh, 8000000, ProteusIII_SPI_PIN_RESET, ProteusIII_SPI_PIN_WAKEUP, ProteusIII_SPI_PIN_BOOT, callbackConfiguration);
    Debug_out("ProteusIII_SPI init", ret);

    if (ret == true)
    {
        ret = ProteusIII_SPI_PinReset();
        Debug_out("Reset", ret);
        delay(100);

        ret = ProteusIII_SPI_GetBTMAC(btmac);
        Debug_out("ProteusIII_SPI_GetBTMAC",ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "BTMAC 0x%02x%02x%02x%02x%02x%02x\n" COLOR_RESET,btmac[5],btmac[4],btmac[3],btmac[2],btmac[1],btmac[0]);
        }
        delay(100);

        uint16_t name_len = 0;
        uint8_t devicename[32];
        ret = ProteusIII_SPI_GetDeviceName(devicename,&name_len);
        devicename[name_len] = '\0';
        Debug_out("ProteusIII_SPI_GetDeviceName",ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "Devicename %s\n" COLOR_RESET,devicename);
        }
        delay(100);

#if 0
        devicename[0] = 'B';
        name_len = 1;
        ret = ProteusIII_SPI_SetDeviceName(devicename,name_len);
        /* module reset automatically when usersettings are changed*/
        delay(ProteusIII_SPI_BOOT_DURATION);
        Debug_out("ProteusIII_SPI_SetDeviceName",ret);
        delay(500);

        ret = ProteusIII_SPI_GetDeviceName(devicename,&name_len);
        devicename[name_len] = '\0';
        Debug_out("ProteusIII_SPI_GetDeviceName",ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "Devicename %s\n" COLOR_RESET,devicename);
        }
        delay(500);

        ret = ProteusIII_SPI_FactoryReset();
        Debug_out("ProteusIII_SPI_FactoryReset",ret);
        delay(500);

        ret = ProteusIII_SPI_GetDeviceName(devicename,&name_len);
        devicename[name_len] = '\0';
        Debug_out("ProteusIII_SPI_GetDeviceName",ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "Devicename %s\n" COLOR_RESET,devicename);
        }
        delay(500);
#endif

#if 0
        int8_t txpower=-50;
        ret = ProteusIII_SPI_GetTXPower(&txpower);
        Debug_out("ProteusIII_SPI_GetTXPower", ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "TXpower %d\n" COLOR_RESET,txpower);
        }
        delay(500);

        txpower = ProteusIII_SPI_TXPower_4;
        ret = ProteusIII_SPI_SetTXPower(txpower);
        /* module reset automatically when usersettings are changed*/
        delay(ProteusIII_SPI_BOOT_DURATION);
        Debug_out("ProteusIII_SPI_SetTXPower", ret);
        delay(500);

        ret = ProteusIII_SPI_GetTXPower(&txpower);
        Debug_out("ProteusIII_SPI_GetTXPower", ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "TXpower %d\n" COLOR_RESET,txpower);
        }
        delay(500);
#endif

#if 0
        /* Config Local */
        ProteusIII_SPI_GPIOConfigBlock_t configLocalGPIO[ProteusIII_SPI_AMOUNT_GPIO_PINS];

        /* config GPIO 1 as output high*/
        configLocalGPIO[0].length = 0x03;
        configLocalGPIO[0].GPIO_ID = ProteusIII_SPI_GPIO_1;
        configLocalGPIO[0].InputOutput = ProteusIII_SPI_GPIO_IO_Output;
        configLocalGPIO[0].value = ProteusIII_SPI_GPIO_Output_High;

        /* config GPIO 2 as input pullup*/
        configLocalGPIO[1].length = 0x03;
        configLocalGPIO[1].GPIO_ID = ProteusIII_SPI_GPIO_2;
        configLocalGPIO[1].InputOutput = ProteusIII_SPI_GPIO_IO_Input;
        configLocalGPIO[1].value = ProteusIII_SPI_GPIO_Input_PullUp;

        ret = ProteusIII_SPI_GPIOLocalWriteConfig(configLocalGPIO, 2*sizeof(ProteusIII_SPI_GPIOConfigBlock_t));
        Debug_out("ProteusIII_SPI_GPIO_Local_Write_Config",ret);
        delay(2000);

        /* Read config local */
        uint16_t configLocalLength = 0;
        ret = ProteusIII_SPI_GPIOLocalReadConfig(configLocalGPIO, &configLocalLength);
        Debug_out("ProteusIII_SPI_GPIOLocalReadConfig",ret);

        /* Write local */
        ProteusIII_SPI_GPIOControlBlock_t controlLocalGPIO;

        /* set GPIO 1 to low */
        controlLocalGPIO.length = 0x02;
        controlLocalGPIO.GPIO_ID = ProteusIII_SPI_GPIO_1;
        controlLocalGPIO.value = ProteusIII_SPI_GPIO_Output_Low;

        ret = ProteusIII_SPI_GPIOLocalWrite(&controlLocalGPIO, sizeof(ProteusIII_SPI_GPIOControlBlock_t));
        Debug_out("ProteusIII_SPI_GPIOLocalWrite - Set GPIO 1 to low", ret);
        delay(500);

        /* Read local pin */
        /* Read GPIO 1 and 2 */
        ProteusIII_SPI_GPIOControlBlock_t readLocalGPIO[2];
        uint16_t readLocalPinLength = 0;

        uint8_t GPIOToReadLocal[2];
        GPIOToReadLocal[0] = ProteusIII_SPI_GPIO_1;
        GPIOToReadLocal[1] = ProteusIII_SPI_GPIO_2;

        ret = ProteusIII_SPI_GPIOLocalRead(GPIOToReadLocal, 2, readLocalGPIO, &readLocalPinLength);
        Debug_out("ProteusIII_SPI_GPIOLocalRead", ret);
        printf("GPIO %x: %x \n", readLocalGPIO[0].GPIO_ID, readLocalGPIO[0].value);
        printf("GPIO %x: %x \n", readLocalGPIO[1].GPIO_ID, readLocalGPIO[1].value);

#endif // 1
#if 0

        /* Connect to remote ProteusIII_SPI */
        uint8_t btMacRemote[6] = {0x05, 0x00, 0x00, 0xDA, 0x18, 0x00};
        ret = ProteusIII_SPI_Connect(btMacRemote);
        Debug_out("Try to connect to remote Proteus", ret);

        if(ret)
        {
            /* Wait for connection to be established */
            uint8_t maxWaitInSeconds = 6;
            uint8_t secondsWait = 0;
            bool connectionEstablished = true;
            while(ProteusIII_SPI_State_BLE_Channel_Open != ProteusIII_SPI_GetDriverState())
            {
                if(maxWaitInSeconds <= secondsWait++)
                {
                    fprintf(stdout, "Could not establish connection\n");
                    connectionEstablished = false;
                    break;
                }
                delay(1000);
            }

            if(connectionEstablished)
            {
                /* Config remote */
                ProteusIII_SPI_GPIOConfigBlock_t configRemoteGPIO[ProteusIII_SPI_AMOUNT_GPIO_PINS];
                /* config GPIO 1 as output high */
                configRemoteGPIO[0].length = 0x03;
                configRemoteGPIO[0].GPIO_ID = ProteusIII_SPI_GPIO_1;
                configRemoteGPIO[0].InputOutput = ProteusIII_SPI_GPIO_IO_Output;
                configRemoteGPIO[0].value = ProteusIII_SPI_GPIO_Output_High;

                /* config GPIO 2 as input pulldown */
                configRemoteGPIO[1].length = 0x03;
                configRemoteGPIO[1].GPIO_ID = ProteusIII_SPI_GPIO_2;
                configRemoteGPIO[1].InputOutput = ProteusIII_SPI_GPIO_IO_Input;
                configRemoteGPIO[1].value = ProteusIII_SPI_GPIO_Input_PullUp;

                ret = ProteusIII_SPI_GPIORemoteWriteConfig(configRemoteGPIO, 2*sizeof(ProteusIII_SPI_GPIOConfigBlock_t));
                Debug_out("ProteusIII_SPI_GPIORemoteWriteConfig",ret);

                uint16_t configRemoteLength = 0;
                ret = ProteusIII_SPI_GPIORemoteReadConfig(configRemoteGPIO,&configRemoteLength);
                Debug_out("ProteusIII_SPI_GPIORemoteReadConfig", ret);
                delay(1000);

                /* Write remote */
                ProteusIII_SPI_GPIOControlBlock_t controlRemoteGPIO;

                /* set GPIO 1 to low */
                controlRemoteGPIO.length = 0x02;
                controlRemoteGPIO.GPIO_ID = ProteusIII_SPI_GPIO_1;
                controlRemoteGPIO.value = ProteusIII_SPI_GPIO_Output_Low;

                ret = ProteusIII_SPI_GPIORemoteWrite(&controlRemoteGPIO, sizeof(ProteusIII_SPI_GPIOControlBlock_t));
                Debug_out("ProteusIII_SPI_GPIORemoteWrite - Set GPIO 1 to low", ret);
                delay(500);

                /* Read local pin */
                /* Read GPIO 1 and 2 */
                ProteusIII_SPI_GPIOControlBlock_t readRemoteGPIO[2];
                uint16_t readRemotePinLength = 0;

                uint8_t GPIOToReadRemote[2];
                GPIOToReadRemote[0] = ProteusIII_SPI_GPIO_1;
                GPIOToReadRemote[1] = ProteusIII_SPI_GPIO_2;

                ret = ProteusIII_SPI_GPIORemoteRead(GPIOToReadRemote, 2, readRemoteGPIO, &readRemotePinLength);
                Debug_out("ProteusIII_SPI_GPIORemoteRead", ret);
                if(ret)
                {
                    printf("GPIO %x: %x \n", readRemoteGPIO[0].GPIO_ID, readRemoteGPIO[0].value);
                    printf("GPIO %x: %x \n", readRemoteGPIO[1].GPIO_ID, readRemoteGPIO[1].value);
                }

                ret = ProteusIII_SPI_Disconnect();
                Debug_out("Disconnect connection to remote Proteus", ret);
            }
        }
#endif

#if 0
        ret = ProteusIII_SPI_Sleep();
        Debug_out("ProteusIII_SPI_Sleep", ret);
        delay(500);
        ret = ProteusIII_SPI_PinWakeup();
        Debug_out("ProteusIII_SPI_PinWakeup", ret);
        delay(500);
        ret = ProteusIII_SPI_GetDeviceName(devicename,&name_len);
        devicename[name_len] = '\0';
        Debug_out("ProteusIII_SPI_GetDeviceName",ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "Devicename %s\n" COLOR_RESET,devicename);
        }
        delay(500);
#endif
    }
    ret = ProteusIII_SPI_Deinit();
    Debug_out("ProteusIII_SPI deinit", ret);
}

/* this function waits for incoming connections, transmits data and disconnects */
static void ProteusIII_SPI_wait4connect_function()
{
    bool ret = false;
    uint8_t btmac[6];

    /* initialize the module ProteusIII_SPI */
    ProteusIII_SPI_CallbackConfig_t callbackConfiguration;
    SetCallbacks(&callbackConfiguration);

    ret = ProteusIII_SPI_Init(ProteusIII_SPI_ConfigIndex_Mode2_activehigh, 8000000, ProteusIII_SPI_PIN_RESET, ProteusIII_SPI_PIN_WAKEUP, ProteusIII_SPI_PIN_BOOT, callbackConfiguration);
    Debug_out("ProteusIII_SPI init", ret);

    if (ret == true)
    {
        ret = ProteusIII_SPI_Reset();
        Debug_out("Reset", ret);
        delay(500);

        ret = ProteusIII_SPI_GetBTMAC(btmac);
        Debug_out("ProteusIII_SPI_GetBTMAC",ret);
        if(ret)
        {
            fprintf (stdout, COLOR_CYAN "BTMAC 0x%02x%02x%02x%02x%02x%02x\n" COLOR_RESET,btmac[5],btmac[4],btmac[3],btmac[2],btmac[1],btmac[0]);
        }
        delay(500);

#if 0
        ret = ProteusIII_SPI_SetSecFlags(ProteusIII_SPI_SecFlags_StaticPassKey);
        Debug_out("ProteusIII_SPI_SetSecFlag",ret);
        delay(100);
#endif

        fprintf (stdout, "Waiting for incoming connections\n");
        while(1)
        {
            if( ProteusIII_SPI_State_BLE_Channel_Open == ProteusIII_SPI_GetDriverState())
            {
                /* send data and disconnect */
                uint8_t test_data[5] = {'T','e','s','t','0'};
				int i = 0;
				for(i=0; i<5; i++)
				{
					test_data[4]++;
                    ret = ProteusIII_SPI_Transmit(test_data, 5);
                    Debug_out("ProteusIII_SPI_Transmit",ret);
                    delay(500);
                }

                ret = ProteusIII_SPI_Disconnect();
                Debug_out("ProteusIII_SPI_Disconnect",ret);
                delay(500);
                break;
            }
            delay(1000);
        }
    }
    ret = ProteusIII_SPI_Deinit();
    Debug_out("ProteusIII_SPI deinit", ret);
}

/* this function scans for available ProteusIII_SPI modules and connects to the first */
static void ProteusIII_SPI_scanNconnect_function()
{
    bool ret = false;
    ProteusIII_SPI_GetDevices_t devices;

    /* initialize the module ProteusIII_SPI */
    ProteusIII_SPI_CallbackConfig_t callbackConfiguration;
    SetCallbacks(&callbackConfiguration);

    ret = ProteusIII_SPI_Init(ProteusIII_SPI_ConfigIndex_Mode2_activehigh, 8000000, ProteusIII_SPI_PIN_RESET, ProteusIII_SPI_PIN_WAKEUP, ProteusIII_SPI_PIN_BOOT, callbackConfiguration);
    Debug_out("ProteusIII_SPI init", ret);

    if (ret == true)
    {
        ret = ProteusIII_SPI_PinReset();
        Debug_out("Reset", ret);
        delay(500);
#if 0
        ret = ProteusIII_SPI_SetSecFlag(ProteusIII_SPI_SecFlags_NONE);
        Debug_out("ProteusIII_SPI_SetSecFlag",ret);
        delay(100);
#endif // 0

        while(1)
        {
            ret = ProteusIII_SPI_Scanstart();
            Debug_out("ProteusIII_SPI_Scanstart",ret);
            delay(1500);

            ret = ProteusIII_SPI_Scanstop();
            Debug_out("ProteusIII_SPI_Scanstop",ret);
            delay(500);

            ret = ProteusIII_SPI_GetDevices(&devices);
            Debug_out("ProteusIII_SPI_GetDevices",ret);
            delay(100);

            int i;
            for(i=0; i<devices.numberofdevices; i++)
            {
                fprintf (stdout, COLOR_CYAN "Found BTMAC 0x%02x%02x%02x%02x%02x%02x\n" COLOR_RESET,devices.devices[i].btmac[5],devices.devices[i].btmac[4],devices.devices[i].btmac[3],devices.devices[i].btmac[2],devices.devices[i].btmac[1],devices.devices[i].btmac[0]);
            }

            if(devices.numberofdevices > 0)
            {
                fprintf (stdout, COLOR_CYAN "Connect to BTMAC 0x%02x%02x%02x%02x%02x%02x\n" COLOR_RESET,devices.devices[0].btmac[5],devices.devices[0].btmac[4],devices.devices[0].btmac[3],devices.devices[0].btmac[2],devices.devices[0].btmac[1],devices.devices[0].btmac[0]);

                ret = ProteusIII_SPI_Connect(devices.devices[0].btmac);
                Debug_out("ProteusIII_SPI_Connect",ret);
                delay(500);

                if(ret == true)
                {
                    while(1)
                    {
                        /* Wait until channel is open */
                        if( ProteusIII_SPI_State_BLE_Channel_Open == ProteusIII_SPI_GetDriverState())
                        {
                            ret = ProteusIII_SPI_PhyUpdate(ProteusIII_SPI_Phy_2MBit);
                            Debug_out("ProteusIII_SPI_PhyUpdate",ret);
                            delay(1000);

                            /* send data and disconnect */
                            uint8_t test_data[5] = {'T','e','s','t','0'};
                            int i = 0;
                            for(i=0; i<5; i++)
                            {
                                test_data[4]++;
                                ret = ProteusIII_SPI_Transmit(test_data, 5);
                                Debug_out("ProteusIII_SPI_Transmit",ret);
                            }

                            ret = ProteusIII_SPI_Disconnect();
                            Debug_out("ProteusIII_SPI_Disconnect",ret);
                            delay(100);
                            break;
                        }
                        delay(1000);
                    }
                }
            }
            delay(1000);
        }
    }

    ret = ProteusIII_SPI_Deinit();
    Debug_out("ProteusIII_SPI deinit", ret);
}

