#include <stdio.h>
#include <stdlib.h>

#include "../drivers/WE-common.h"
#include "../drivers/global/global.h"
#include "../drivers/Calypso/Calypso.h"
#include "../drivers/Calypso/ATCommands/ATDevice.h"
#include "../drivers/Calypso/ATCommands/ATWLAN.h"
#include "../drivers/Calypso/ATCommands/ATSocket.h"
#include "../drivers/Calypso/ATCommands/ATFile.h"
#include "../drivers/Calypso/ATCommands/ATMQTT.h"
#include "../drivers/Calypso/ATCommands/events.h"
#include "UseCaseSockets.h"


static void Calypso_test_function(void);
static void Calypso_test_ATDevice_function(void);
static void Calypso_test_ATWLAN_function(void);
static void Calypso_test_ATFile_function(void);
static void Calypso_test_ATMQTT_function(void);

pthread_t thread_main;
static bool AbortMainLoop;
void *main_Thread(void *pArgs)
{
    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_MAIN_THREAD);

#if 1
    /* function to test Calypso driver */
    Calypso_test_function();
#else
    /*Sample use case for secure TCP connection*/
    UseCaseSockets_TCP(true, true);
#endif

    return 0 ;

}

/* the main function simply starts the MainThread */
int main ()
{
    AbortMainLoop = false;

    if(pthread_create(&thread_main, NULL, &main_Thread, NULL))
    {
        fprintf(stdout, "Failed to start thread_main");
        return 1;
    }


    if(pthread_join(thread_main, NULL))
    {
        fprintf(stdout, "Failed to start thread_main");
        return 2;
    }
    return 0;
}

static void Calypso_test_function()
{
    Calypso_test_ATDevice_function();
}


static void Calypso_test_ATDevice_function()
{

    if(Calypso_Init(921600, Calypso_ParityBit_EVEN, NULL))
    {
        bool ret = false;
        ret = ATDevice_reboot();
        Debug_out("AT+reboot", ret);
        delay(1000);

        ret = ATDevice_stop(250);
        Debug_out("AT+stop", ret);
        delay(500);

        ret = ATDevice_start();
        Debug_out("AT+start", ret);
        delay(500);

        ATDevice_Version_t version;
        size_t response_length = 0;
        ret = ATDevice_get(ATDEVICE_ID_general,ATDEVICE_GENERAL_version, &version, &response_length);
        Debug_out("AT+get=general,version", ret);
        if(ret)
        {
            printf("chipid: %s\n", version.chipId);
            printf("FWVersion: %s\n", version.FWVersion);
            printf("NWPVersion: %s\n", version.NWPVersion);
            printf("PHYVersion: %s\n", version.PHYVersion);
            printf("ROMVersion: %s\n", version.ROMVersion);
        }

        ATDevice_Time_t time;
        ret = ATDevice_get(ATDEVICE_ID_general, ATDEVICE_GENERAL_time, &time, &response_length);
        Debug_out("AT+get=general,time", ret);
        if(ret)
        {
            printf("date(dd:mm:yy): %u.%u.%u time(hh:mm:ss): %u:%u:%u \n", time.day, time.month, time.year, time.hour, time.minute, time.second);
        }

        uint32_t baudrate = 0;
        ret = ATDevice_get(ATDEVICE_ID_UART, ATDEVICE_UART_baudrate, &baudrate, &response_length);
        Debug_out("AT+get=UART,baudrate", ret);
        if(ret)
        {
            printf("baudrate: %u\n", baudrate);
        }


        delay(100);
        time.hour++;
        time.year = 2019;
        ret = ATDevice_set(ATDEVICE_ID_general, ATDEVICE_GENERAL_time, &time);
        Debug_out("AT+set=general,time", ret);
        delay(100);

        ret = ATDevice_get(ATDEVICE_ID_general, ATDEVICE_GENERAL_time, &time, &response_length);
        Debug_out("AT+get=general,time", ret);

        ret = ATDevice_sleep(2);
        Debug_out("AT+sleep", ret);

        delay(3000);
    }

    Calypso_Deinit();

}

static void Calypso_test_ATWLAN_function(void)
{

    if(Calypso_Init(921600, Calypso_ParityBit_EVEN, NULL))
    {
        bool ret = false;

        ATWLAN_ScanEntry_t scanEntries[15];
        uint8_t numEntries;

        ATWLAN_scan(0, 15, scanEntries, &numEntries);
        delay(200);
        ret = ATWLAN_scan(0, 15, scanEntries, &numEntries);
        Debug_out("AT+wlanScan", ret);

        delay(200);
        const char *ssid = "Calypso-Pruefrouter";
        const char *key = "123456789ABC";
        ATWLAN_ConnectionArguments_t connectArgs;
        memset(&connectArgs, 0, sizeof(connectArgs));
        strcpy(connectArgs.SSID, ssid);
        connectArgs.securityParams.securityType = ATWLAN_SECURITY_TYPE_WPA_WPA2;
        strcpy(connectArgs.securityParams.securityKey, key);

        ret = ATWLAN_connect(connectArgs);
        Debug_out("AT+wlanConnect", ret);


        delay(2000);
        ret = ATWLAN_disconnect();
        Debug_out("AT+wlanDisconnect", ret);

        delay(500);
        uint8_t profileIndex = 0;
        ret = ATWLAN_profileAdd(connectArgs, 15, &profileIndex);
        Debug_out("AT+wlanProfileadd", ret);

        delay(500);
        ATDevice_reboot();

        delay(1000);
        uint8_t priority = 0;
        ATWLAN_Profile_t profile;
        memset(&priority, 0, sizeof(priority));

        ret = ATWLAN_profileGet(profileIndex, &profile);
        Debug_out("AT+wlanProfileget", ret);

        delay(500);
        ret = ATWLAN_profileDel(profileIndex);
        Debug_out("AT+wlanProfiledel", ret);

        uint8_t buffer[128];
        size_t length;
        ret = ATWLAN_get(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_COUNTRY_CODE, buffer, &length);
        Debug_out("AT+wlanget", ret);

        ret = ATWLAN_set(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_COUNTRY_CODE, "EU");
        Debug_out("AT+wlanset", ret);

        ret = ATWLAN_get(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_COUNTRY_CODE, buffer, &length);
        Debug_out("AT+wlanget", ret);
        printf("Country code: %s", buffer);


        ret = ATWLAN_get(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_SCAN_PARAMS, buffer, &length);
        Debug_out("AT+wlanget", ret);


        ret = ATWLAN_get(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_STA_TX_POWER, buffer, &length);
        printf("STA TX POW: %d \n", buffer[0]);
        Debug_out("AT+wlanget", ret);

        length = 1;
        ret = ATWLAN_set(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_STA_TX_POWER, &length);
        Debug_out("AT+wlanset", ret);

        ret = ATWLAN_get(ATWLAN_SET_ID_general, ATWLAN_SET_GENERAL_STA_TX_POWER, buffer, &length);
        printf("STA TX POW: %d \n", buffer[0]);
        Debug_out("AT+wlanget", ret);


        ret = ATWLAN_get(ATWLAN_SET_ID_P2P, ATWLAN_SET_P2P_CHANNEL_N_REGS, buffer, &length);
        Debug_out("AT+wlanget", ret);



        ret = ATWLAN_get(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_SSID, buffer, &length);
        printf("AP SSID: %s \n", buffer);
        Debug_out("AT+wlanget", ret);

        ret = ATWLAN_set(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_SSID, "Calypso-moduleSeS");
        Debug_out("AT+wlanset", ret);

        ret = ATWLAN_get(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_SSID, buffer, &length);
        printf("AP SSID: %s \n", buffer);
        Debug_out("AT+wlanget", ret);


        ret = ATWLAN_get(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_MAX_STATIONS, buffer, &length);
        printf("AP MAX stations: %d \n", buffer[0]);
        Debug_out("AT+wlanget", ret);

        length = 4;
        ret = ATWLAN_set(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_MAX_STATIONS, &length);
        Debug_out("AT+wlanset", ret);

        ret = ATWLAN_get(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_MAX_STATIONS, buffer, &length);
        printf("AP MAX stations: %d \n", buffer[0]);
        Debug_out("AT+wlanget", ret);


        ret = ATWLAN_get(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_SECURITY, buffer, &length);
        printf("AP Security: %d \n", buffer[0]);
        Debug_out("AT+wlanget", ret);

        ATWLAN_SecurityType_t secType = ATWLAN_SECURITY_TYPE_OPEN;
        ret = ATWLAN_set(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_SECURITY, &secType);
        Debug_out("AT+wlanset", ret);

        ret = ATWLAN_get(ATWLAN_SET_ID_AP, ATWLAN_SET_AP_SECURITY, buffer, &length);
        printf("AP Security: %d \n", buffer[0]);
        Debug_out("AT+wlanget", ret);

        uint8_t outputOption;
        uint32_t outputValue;

        ret = ATWLAN_policyGet(ATWLAN_POLICY_ID_PM, &outputOption, &outputValue);
        Debug_out("AT+wlanPolicyGet\n", ret);

        outputValue = 5;
        ret = ATWLAN_policySet(ATWLAN_POLICY_ID_PM, ATWLAN_POLICY_PM_long_sleep, &outputValue);
        Debug_out("AT+wlanPolicySet\n", ret);

        outputValue = 0;
        ret = ATWLAN_policyGet(ATWLAN_POLICY_ID_PM, &outputOption, &outputValue);
        Debug_out("AT+wlanPolicyGet\n", ret);

    }

    Calypso_Deinit();
}

static void Calypso_test_ATFile_function(void)
{
    bool ret = false;
    if(Calypso_Init(921600, Calypso_ParityBit_EVEN, NULL))
    {
        uint32_t fileID;
        uint32_t secureToken;
        const char *fileName = "testfile";
        char *fileContent = "Hello World";


        ret = ATFile_open(fileName, (ATFILE_OPEN_CREATE|ATFILE_OPEN_WRITE), FILE_MIN_SIZE, &fileID, &secureToken);

        if(ret)
        {
            uint16_t writtenBytes = 0;
            ATFile_write(fileID, 0, Calypso_DataFormat_Binary, false, strlen(fileContent), fileContent, &writtenBytes);
        }
        ret = ATFile_close(fileID, NULL, NULL);


        ret = ATFile_open(fileName, (ATFILE_OPEN_READ), FILE_MIN_SIZE, &fileID, &secureToken);

            char fileContentReadBack[32];
            uint16_t readBytes = 0;
            Calypso_DataFormat_t format;
        if(ret)
        {
            ATFile_read(fileID, 0, Calypso_DataFormat_Binary, strlen(fileContent), &format, &readBytes, fileContentReadBack);
        }
        ret = ATFile_close(fileID, NULL, NULL);


        ret = ATFile_del(fileName, secureToken);


    }

    Calypso_Deinit();
}

static void Calypso_test_ATMQTT_function(void)
{
    bool ret = false;

    const char *serverAddress = "mqtt.address";
    if(Calypso_Init(921600, Calypso_ParityBit_EVEN, NULL))
    {

        ATMQTT_ServerInfo_t serverInfo;
        strcpy(serverInfo.address, serverAddress);
        serverInfo.port = 1234;

        ATMQTT_securityParams_t securityParams;

        securityParams.securityMethod = ATMQTT_SECURITY_METHOD_SSLV3;
        strcpy(securityParams.CAFile, STRING_EMPTY);
        strcpy(securityParams.certificateFile, STRING_EMPTY);
        strcpy(securityParams.DHKey, STRING_EMPTY);
        strcpy(securityParams.privateKeyFile, STRING_EMPTY);

        securityParams.cipher = ATMQTT_CIPHER_SSL_RSA_WITH_RC4_128_MD5;

        ATMQTT_connectionParams_t connectionParams;
        connectionParams.format = Calypso_DataFormat_Binary;
        connectionParams.protocolVersion = ATMQTT_PROTOCOL_v3_1;
        connectionParams.blockingSend = 0;

        ret = ATDevice_reboot();
        Debug_out("AT+reboot", ret);
        delay(5000);

        uint8_t mqttIndex = 0;
        ret = ATMQTT_create("testClient", (ATMQTT_CREATE_FLAGS_IP4), serverInfo, securityParams, connectionParams, &mqttIndex);
        Debug_out("AT+mqttCreate", ret);

        ret = ATMQTT_connect(mqttIndex);
        Debug_out("AT+mqttConnect", ret);
        delay(2000);

        char *message = "10.4°C";
        ret = ATMQTT_publish(mqttIndex, "kitchen/temp", ATMQTT_QOS_0, 0, strlen(message), message);
        Debug_out("AT+mqttPublish", ret);

        ATMQTT_subscribeTopic_t topics[4];
        strcpy(topics[0].topicString, "kitchen/temp");
        topics[0].QoS = ATMQTT_QOS_0;

        strcpy(topics[1].topicString, "kitchen/humidity");
        topics[1].QoS = ATMQTT_QOS_0;

        ret = ATMQTT_subscribe(mqttIndex, 2, topics);
        Debug_out("AT+mqttSubscribe", ret);

        ret = ATMQTT_unsubscribe(mqttIndex, 1, "kitchen/temp", NULL, NULL, NULL);
        Debug_out("AT+mqttUnsubscribe", ret);

        ret = ATMQTT_disconnect(mqttIndex);
        Debug_out("AT+mqttDisconnect", ret);

        ret = ATMQTT_delete(mqttIndex);
        Debug_out("AT+mqttDelete", ret);
    }
    Calypso_Deinit();
}
