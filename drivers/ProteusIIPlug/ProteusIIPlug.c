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

#include <errno.h>
#include "string.h"


#include "ProteusIIPlug.h"
#include "../ProteusII/ProteusII.h"
#include "../../drivers/WE-common.h"
#include "../../drivers/global/global.h"

/**************************************
 *     Static function declarations   *
 **************************************/

/**************************************
 *          Static variables          *
 **************************************/

/**************************************
 *         Static functions           *
 **************************************/

/**************************************
 *         Global functions           *
 **************************************/

/*
 *Initialize the ProteusIIPlug interface for serial interface
 *
 *input:
 * -baudrate:  baudrate of the interface
 * -rp:        Reset Pin
 * -wp:        Wake-up Pin
 * -bp:        Boot Pin
 * -RXcb:      RX callback function called when data was received via radio
 * -Ccb:       Connection callback called when connection is setup
 * -DCcb:      Disconnection callback called when connection was shut down
 * -COcb:      Channelopen callback called when channel is open for data transmission
 * -Scb:       Security callback called when security is setup
 * -PKcb:      Passkey callback called when passkey has to be entered
 *
 *Caution: the parameter baudrate must match the configured UserSettings of the ProteusIIPlug
 *         -the baudrate parameter must match to perform a successful FTDI communication
 *          *updating this parameter during runtime may lead to communication errors
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool ProteusIIPlug_Init(int baudrate, int rp,void(*RXcb)(uint8_t*,uint16_t,uint8_t*,int8_t),void(*Ccb)(uint8_t*),void(*DCcb)(),void(*COcb)(uint8_t*,uint16_t),void(*Scb)(uint8_t*,ProteusIIPlug_Security_t),void(*PKcb)(uint8_t*),void(*PUcb)(uint8_t*,uint8_t,uint8_t))
{
    return ProteusII_Init(baudrate, rp, PIN_INVALID, PIN_INVALID, RXcb, Ccb, DCcb, COcb, Scb, PKcb, PUcb);
}

/*
 *Deinitialize the ProteusIIPlug interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ProteusIIPlug_Deinit()
{
    return ProteusII_Deinit();
}

/*
 *Wakeup the ProteusIIPlug from sleep by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool ProteusIIPlug_PinWakeup()
{
    return ProteusII_PinWakeup();
}

/*
 *Reset the ProteusIIPlug by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIIPlug_PinReset()
{
    return ProteusII_PinReset();
}

/*
 *Reset the ProteusIIPlug by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIIPlug_Reset()
{
    return ProteusII_Reset();
}

/*
 *Disconnect the ProteusIIPlug connection if open
 *
 *return true if disconnection succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Disconnect()
{
    return ProteusII_Disconnect();
}

/*
 *Put the ProteusIIPlug into sleep mode
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Sleep()
{
    return ProteusII_Sleep();
}

/*
 *Transmit data if a connection is open
 *
 *input:
 * -PayloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Transmit(uint8_t *PayloadP, uint16_t length)
{
    return ProteusII_Transmit(PayloadP, length);
}


/*
 *Factory reset the module
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_FactoryReset()
{
    return ProteusII_FactoryReset();
}

/*
 *Set a special user setting
 *
 *input:
 * -us:     user setting to be updated
 * -value:  pointer to the new settings value
 * -length: length of the value
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Set(ProteusIIPlug_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length)
{
    return ProteusII_Set(userSetting, ValueP, length);
}

/*
 *Set the BLE device name
 *
 *input:
 * -deviceNameP: pointer to the device name
 * -nameLength:  length of the device name
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength)
{
    return ProteusII_SetDeviceName(deviceNameP, nameLength);
}

/*
 *Set the BLE advertising timeout
 *
 *input:
 * -advTimeout: advertising timeout
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetAdvertisingTimeout(uint16_t advTimeout)
{
    return ProteusII_SetAdvertisingTimeout(advTimeout);
}
/*
 *Set the CFG flags
 *
 *input:
 * -cfgflags: CFG flags
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetCFGFlags(uint16_t cfgflags)
{
    return ProteusII_SetCFGFlags(cfgflags);
}

/*
 *Set the BLE connection timing
 *
 *input:
 * -connectionTiming: connection timing
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetConnectionTiming(ProteusIIPlug_ConnectionTiming_t connectionTiming)
{
    return ProteusII_SetConnectionTiming(connectionTiming);
}

/*
 *Set the BLE scan timing
 *
 *input:
 * -scanTiming: scan timing
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetScanTiming(ProteusIIPlug_ScanTiming_t scanTiming)
{
    return ProteusII_SetScanTiming(scanTiming);
}

/*
 *Set the BLE TX power
 *
 *input:
 * -txpower: TX power
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetTXPower(ProteusIIPlug_TXPower_t txpower)
{
    return ProteusII_SetTXPower(txpower);
}

/*
 *Set the BLE security flags
 *
 *input:
 * -secflags: security flags
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetSecFlags(ProteusIIPlug_SecFlags_t secflags)
{
    return ProteusII_SetSecFlags(secflags);
}

/*
 *Set the UART baudrate index
 *
 *input:
 * -baudrateindex: UART baudrate index
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIPlug_SetBaudrateIndex(ProteusIIPlug_BaudRateIndex_t baudrateIndex)
{
    return ProteusII_SetBaudrateIndex(baudrateIndex);
}

/*
 *Set the BLE static passkey
 *
 *input:
 * -staticPasskeyP: pointer to the static passkey
 *
 *note: module resets automatically so that settings take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_SetStaticPasskey(uint8_t *staticPasskeyP)
{
    return ProteusII_SetStaticPasskey(staticPasskeyP);
}

/*
 *Request the current user settings
 *
 *input:
 * -userSetting: user setting to be requested
 *
 *output:
 * -responseP: pointer of the memory to put the requested content
 * -response_lengthP: length of the requested content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Get(ProteusIIPlug_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP)
{
    return ProteusII_Get(userSetting, ResponseP, Response_LengthP);
}

/*
 *Request the 3 byte firmware version
 *
 *output:
 * -versionP: pointer to the 3 byte firmware version
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetFWVersion(uint8_t *versionP)
{
    return ProteusII_GetFWVersion(versionP);
}

/*
 *Request the current BLE device name
 *
 *output:
 * -deviceNameP: pointer to device name
 * -nameLengthP: pointer to the length of the device name
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP)
{
    return ProteusII_GetDeviceName(deviceNameP, nameLengthP);
}
/*
 *Request the 6 digit MAC
 *
 *output:
 * -macP: pointer to the MAC
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetMAC(uint8_t *MacP)
{
    return ProteusII_GetMAC(MacP);
}

/*
 *Request the 6 digit Bluetooth MAC
 *
 *output:
 * -macP: pointer to the Bluetooth MAC
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetBTMAC(uint8_t *MacP)
{
    return ProteusII_GetBTMAC(MacP);
}

/*
 *Request the advertising timeout
 *
 *output:
 * -advTimeoutP: pointer to the advertising timeout
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetAdvertisingTimeout(uint16_t *advTimeoutP)
{
    return ProteusII_GetAdvertisingTimeout(advTimeoutP);
}

/*
 *Request the connection timing
 *
 *output:
 * -connectionTimingP: pointer to the  connection timing
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetConnectionTiming(ProteusIIPlug_ConnectionTiming_t *connectionTimingP)
{
    return ProteusII_GetConnectionTiming(connectionTimingP);
}

/*
 *Request the scan timing
 *
 *output:
 * -scanTimingP: pointer to the  scan timing
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetScanTiming(ProteusIIPlug_ScanTiming_t *scanTimingP)
{
    return ProteusII_GetScanTiming(scanTimingP);
}

/*
 *Request the TX power
 *
 *output:
 * -txpowerP: pointer to the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetTXPower(ProteusIIPlug_TXPower_t *txpowerP)
{
    return ProteusII_GetTXPower(txpowerP);
}

/*
 *Request the security flags
 *
 *output:
 * -secflagsP: pointer to the security flags
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetSecFlags(ProteusIIPlug_SecFlags_t *secflagsP)
{
    return ProteusII_GetSecFlags(secflagsP);
}

/*
 *Request the UART baudrate index
 *
 *output:
 * -baudrateIndexP: pointer to the UART baudrate index
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIPlug_GetBaudrateIndex(ProteusIIPlug_BaudRateIndex_t *baudrateIndexP)
{
    return ProteusII_GetBaudrateIndex(baudrateIndexP);
}

/*
 *Request the CFG flags
 *
 *output:
 * -cfgflags: pointer to the CFG flags
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetCFGFlags(uint16_t *cfgflags)
{
    return ProteusII_GetCFGFlags(cfgflags);
}

/*
 *Request the module state
 *
 *output:
 * -BLE_roleP:   pointer to the BLE role of the module
 * -BLE_actionP: pointer to the BLE action of the module
 * -InfoP:       pointer to the state info of the module
 * -LengthP:     pointer to the length of the state info of the module
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetState(ProteusIIPlug_BLE_Role_t *BLE_roleP, ProteusIIPlug_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP)
{
    return ProteusII_GetState(BLE_roleP, BLE_actionP, InfoP, LengthP);
}

/*
 *Request the current state of the driver
 *
 *
 *return driver state
 */
ProteusIIPlug_States_t ProteusIIPlug_GetDriverState()
{
    return ProteusII_GetDriverState();
}

/*
 *Start scan
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Scanstart()
{
    return ProteusII_Scanstart();
}
/*
 *Stop a scan
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_Scanstop()
{
    return ProteusII_Scanstop();
}

/*
 *Request the scan results
 *
 *output:
 * -devicesP: pointer to scan result struct
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIPlug_GetDevices(ProteusIIPlug_GetDevices_t* devicesP)
{
    return ProteusII_GetDevices(devicesP);
}

/*
 *Connect to the BLE device with the corresponding BTMAC
 *
 *input:
 * -btmac: pointer to btmac
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIPlug_Connect(uint8_t *btmac)
{
    return ProteusII_Connect(btmac);
}

/*
 *Answer on a passkey request with a passkey to setup a connection
 *
 *input:
 * -passkey: pointer to passkey
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIPlug_Passkey(uint8_t* passkey)
{
    return ProteusII_Passkey(passkey);
}


/*
 *Update the phy during an open connection
 *
 *input:
 * -phy: new phy
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIPlug_PhyUpdate(uint8_t phy)
{
    return ProteusII_PhyUpdate(phy);
}
