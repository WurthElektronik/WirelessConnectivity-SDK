#include <stdio.h>
#include <stdlib.h>

#include "../drivers/global/global.h"
#include "../drivers/Calypso/Calypso.h"
#include "../drivers/Calypso/ATCommands/ATDevice.h"
#include "../drivers/Calypso/ATCommands/ATWLAN.h"
#include "../drivers/Calypso/ATCommands/ATSocket.h"
#include "../drivers/Calypso/ATCommands/events.h"
#include "UseCaseSockets.h"


#define EVENT_BUFFER_SIZE    2048

static void eventCallbackSocket(char *pEventCmd);

static AppState_t appState;

char eventbuffer[EVENT_BUFFER_SIZE];
char eventArguments[EVENT_BUFFER_SIZE];
char *pEventBuffer;


const char *ssid = "Calypso-Pruefrouter";
const char *key = "123456789ABC";
const char *ipAddressDest = "192.168.1.101";
char *keyFileName = "calypsosslkey.der";
char *certFileName = "calypsosslcert";

void UseCaseSockets_UDPUnsecure()
{

    appState = AppState_Reset;
    bool ret = false;
    bool running = true;
    uint8_t socketid;

    if(Calypso_Init(921600, Serial_ParityBit_EVEN, eventCallbackSocket))
    {
        while(running)
        {
            switch(appState)
            {
            case AppState_Reset:
            {
                ATDevice_reboot();
                appState = AppState_Idle;
                break;
            }

            case AppState_Disconnected:
            {

                ATWLAN_ConnectionArguments_t connectArgs;
                memset(&connectArgs, 0, sizeof(connectArgs));

                strcpy(connectArgs.SSID, ssid);

                connectArgs.securityParams.securityType = ATWLAN_SECURITY_TYPE_WPA_WPA2;
                strcpy(connectArgs.securityParams.securityKey, key);

                ret = ATWLAN_connect(connectArgs);
                Debug_out("AT+wlanConnect", ret);

                appState = AppState_Idle;

                break;
            }
            case AppState_Connected:
            {
                ATSocket_Family_t family = ATSocket_Family_INET;
                ATSocket_Type_t type = ATSocket_Type_Datagram;
                ATSocket_Protocol_t protocol = ATSocket_Protocol_UDP;

                ret = ATSocket_create(family, type, protocol, &socketid);
                Debug_out("AT+socket", ret);

                ATSocket_Descriptor_t socketSrc;
                socketSrc.family = ATSocket_Family_INET;
                socketSrc.port = 8888;
                ATEvent_NetappIP4Aquired_t *ip4Acquired = (ATEvent_NetappIP4Aquired_t *)eventArguments;
                strcpy(socketSrc.address, ip4Acquired->address);

                printf("IP acquired: %s\n", socketSrc.address);

                ATSocket_Descriptor_t socketDest;
                socketDest.family = ATSocket_Family_INET;
                socketDest.port = 8888;
                strcpy(socketDest.address, ipAddressDest);

                if(ret)
                {
                    ret = ATSocket_bind(socketid, socketSrc);
                    Debug_out("AT+bind", ret);
                }

                if(ret)
                {
                    char *message = "Hello World";
                    ret = ATSocket_sendTo(socketid, socketDest, Calypso_DataFormat_Binary, false, strlen(message), message);
                    Debug_out("AT+sendTo", ret);

                    ret = ATSocket_sendTo(socketid, socketDest, Calypso_DataFormat_Base64, true, strlen(message), message);
                    Debug_out("AT+sendTo", ret);

                }

                if(ret)
                {
                    ret = ATSocket_recvFrom(socketid, socketDest, Calypso_DataFormat_Binary, 127);
                }

                appState = AppState_Idle;
                break;
            }
            case AppState_SocketRcvd:
            {
                ATEvent_SocketRcvdFrom_t *data = (ATEvent_SocketRcvdFrom_t *)eventArguments;
                printf("socket %d: %d bytes data received: %s\n", data->socketID, data->length, data->data);

                ret = ATSocket_close(socketid);
                Debug_out("AT+close", ret);
                appState = AppState_Closed;
                break;
            }
            case AppState_Closed:
            {
                running = false;
                break;
            }
            case AppState_Idle:
            {
                delay(1);
                break;
            }
            case AppState_TCPConnected:
            case AppState_TCPConnectionAccepted:
            {
                ret = ATSocket_close(socketid);
                Debug_out("AT+close", ret);
                appState = AppState_Closed;
                break;
            }
            }
        }
    }

    Calypso_Deinit();

}

void UseCaseSockets_TCP(bool isServer, bool isSecure)
{
    appState = AppState_Reset;
    bool ret = false;
    bool running = true;
    uint8_t socketIdOpen = 0;
    uint8_t socketIdConnection = 0;

    if(Calypso_Init(921600, Serial_ParityBit_EVEN, eventCallbackSocket))
    {
        while(running)
        {
            switch(appState)
            {
            case AppState_Reset:
            {
                ATDevice_reboot();
                appState = AppState_Idle;
                break;
            }
            case AppState_Idle:
            {
//                delay(1);
                break;
            }
            case AppState_Disconnected:
            {

                ATWLAN_ConnectionArguments_t connectArgs;
                memset(&connectArgs, 0, sizeof(connectArgs));

                strcpy(connectArgs.SSID, ssid);

                connectArgs.securityParams.securityType = ATWLAN_SECURITY_TYPE_WPA_WPA2;
                strcpy(connectArgs.securityParams.securityKey, key);

                ret = ATWLAN_connect(connectArgs);
                Debug_out("AT+wlanConnect", ret);

                appState = AppState_Idle;

                break;
            }
            case AppState_Connected:
            {
                ATSocket_Family_t family = ATSocket_Family_INET;
                ATSocket_Type_t type = ATSocket_Type_Stream;
                ATSocket_Protocol_t protocol = ATSocket_Protocol_TCP;

                ret = ATSocket_create(family, type, protocol, &socketIdOpen);
                Debug_out("AT+socket", ret);

                if(isSecure)
                {
                    ATSocket_SockOptSecMethod_t secMethod = ATSocket_SockOptSecMethod_SSLv3_TLSv1_2;
                    ret = ATSocket_setSockOpt(socketIdOpen, ATSocket_SockOpt_Socket, ATSocket_SockOptSocket_SecMethod, &secMethod);

                    if(isServer && ret)
                    {

                        ret = ATSocket_setSockOpt(socketIdOpen, ATSocket_SockOpt_Socket, ATSocket_SockOptSocket_SecureFilesPrivateKeyFileName, keyFileName);
                        if(ret)
                        {
                            ret = ATSocket_setSockOpt(socketIdOpen, ATSocket_SockOpt_Socket, ATSocket_SockOptSocket_SecureFilesCertificateFileName, certFileName);
                        }
                    }
                }

                if(isServer)
                {
                    ATSocket_Descriptor_t socketSrc;
                    socketSrc.family = ATSocket_Family_INET;
                    socketSrc.port = 8888;
                    ATEvent_NetappIP4Aquired_t *ip4Acquired = (ATEvent_NetappIP4Aquired_t *)eventArguments;
                    strcpy(socketSrc.address, ip4Acquired->address);

                    printf("IP acquired: %s\n", socketSrc.address);

                    if(ret)
                    {
                        ret = ATSocket_bind(socketIdOpen, socketSrc);
                        Debug_out("AT+bind", ret);
                    }

                    if(ret)
                    {
                        ret = ATSocket_listen(socketIdOpen, 10);
                        Debug_out("AT+listen", ret);
                    }

                    if(ret)
                    {
                        ret = ATSocket_accept(socketIdOpen, socketSrc.family);
                        Debug_out("AT+accept", ret);
                    }
                }
                else
                {
                    ATSocket_Descriptor_t socketDest;
                    socketDest.family = ATSocket_Family_INET;
                    socketDest.port = 8888;
                    strcpy(socketDest.address, ipAddressDest);

                    if(ret)
                    {
                        ret = ATSocket_connect(socketIdOpen,socketDest);
                    }
                }
                appState = AppState_Idle;
                break;
            }
            case AppState_TCPConnected:
            {
                if(isServer)
                {
                    ret = ATSocket_close(socketIdOpen);
                    Debug_out("AT+close", ret);
                    appState = AppState_Closed;
                }
                else
                {
                    char *message = "Hello World";
                    ret = ATSocket_send(socketIdOpen, Calypso_DataFormat_Binary, false, strlen(message), message);
                    Debug_out("ATSocket_send", ret);

                    if(ret)
                    {
                        ATSocket_recv(socketIdOpen, Calypso_DataFormat_Binary, 32);
                    }
                    appState = AppState_Idle;
                }
                break;
            }

            case AppState_TCPConnectionAccepted:
            {
                if(isServer)
                {
                    ATEvent_SocketTCPAccept_t *tcpAcceptedParams = (ATEvent_SocketTCPAccept_t *)eventArguments;
                    socketIdConnection = tcpAcceptedParams->socketID;

                    char *message = "Hello World";
                    ret = ATSocket_send(socketIdConnection, Calypso_DataFormat_Binary, false, strlen(message), message);
                    Debug_out("ATSocket_send", ret);

                    if(ret)
                    {
                        ATSocket_recv(socketIdConnection, Calypso_DataFormat_Binary, 32);
                    }
                    appState = AppState_Idle;
                }
                break;
            }
            case AppState_SocketRcvd:
            {
                ATEvent_SocketRcvd_t *data = (ATEvent_SocketRcvd_t *)eventArguments;
                printf("socket %d: %d bytes data received: %s\n", data->socketID, data->length, data->data);

                ret = ATSocket_close(socketIdConnection);
                Debug_out("AT+close", ret);
                appState = AppState_Closed;
                break;
            }
            case AppState_Closed:
            {
                running = false;
                break;
            }
            }
        }
    }

    Calypso_Deinit();

}

static void eventCallbackSocket(char *pEventCmd)
{
    strcpy(eventbuffer, pEventCmd);
    pEventBuffer = eventbuffer;

    ATEvent_t event;

    ATEvent_parseEventName(&pEventBuffer, &event);

    switch(event)
    {
    case ATEvent_Startup:
    {
        ATEvent_parseEventArgumentValues(&pEventBuffer, event, &eventArguments[0]);
        appState = AppState_Disconnected;
        break;
    }

    case ATEvent_NetappIP4Aquired:
    {
        if(AppState_Disconnected || AppState_Idle)
        {
        ATEvent_parseEventArgumentValues(&pEventBuffer, event, &eventArguments[0]);
        appState = AppState_Connected;
        }

        break;
    }

    case ATEvent_SocketTCPAccept:
    {
        ATEvent_parseEventArgumentValues(&pEventBuffer, event, &eventArguments[0]);
        appState = AppState_TCPConnectionAccepted;
        break;
    }

    case ATEvent_SocketTCPConnect:
    {
        ATEvent_parseEventArgumentValues(&pEventBuffer, event, &eventArguments[0]);
        appState = AppState_TCPConnected;
        break;
    }

    case ATEvent_SocketRcvd:
    {

        ATEvent_parseEventArgumentValues(&pEventBuffer, event, &eventArguments[0]);

        appState = AppState_SocketRcvd;
        break;
    }

    default:
    {
        //appState = AppState_Idle;
        break;
    }
    }
}
