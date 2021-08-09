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

#include <errno.h>
#include "string.h"


#include "ProteusIIIPlug.h"
#include "../ProteusIII/ProteusIII.h"
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
 *Initialize the ProteusIIIPlug interface for serial interface
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
 *Caution: the parameter baudrate must match the configured UserSettings of the ProteusIIIPlug
 *         -the baudrate parameter must match to perform a successful FTDI communication
 *          *updating this parameter during runtime may lead to communication errors
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_Init(int baudrate, int rp, ProteusIIIPlug_CallbackConfig_t callbackConfig)
{
    return ProteusIII_Init(baudrate, rp, PIN_INVALID, PIN_INVALID, callbackConfig);
}

/*
 *Deinitialize the ProteusIIIPlug interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_Deinit()
{
    return ProteusIII_Deinit();
}

/*
 *Wakeup the ProteusIIIPlug from sleep by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_PinWakeup()
{
    return ProteusIII_PinWakeup();
}

/*
 *Reset the ProteusIIIPlug by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_PinReset()
{
    return ProteusIII_PinReset();
}

/*
 *Reset the ProteusIIIPlug by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_Reset()
{
    return ProteusIII_Reset();
}

/*
 *Disconnect the ProteusIIIPlug connection if open
 *
 *return true if disconnection succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_Disconnect()
{
    return ProteusIII_Disconnect();
}

/*
 *Put the ProteusIIIPlug into sleep mode
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_Sleep()
{
    return ProteusIII_Sleep();
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
ProteusIIIPlug_Transmit(uint8_t *PayloadP, uint16_t length)
{
    return ProteusIII_Transmit(PayloadP, length);
}


/*
 *Factory reset the module
 *
 *return true if succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_FactoryReset()
{
    return ProteusIII_FactoryReset();
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
ProteusIIIPlug_Set(ProteusIIIPlug_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length)
{
    return ProteusIII_Set(userSetting, ValueP, length);
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
ProteusIIIPlug_SetDeviceName(uint8_t *deviceNameP, uint8_t nameLength)
{
    return ProteusIII_SetDeviceName(deviceNameP, nameLength);
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
ProteusIIIPlug_SetAdvertisingTimeout(uint16_t advTimeout)
{
    return ProteusIII_SetAdvertisingTimeout(advTimeout);
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
ProteusIIIPlug_SetCFGFlags(uint16_t cfgflags)
{
    return ProteusIII_SetCFGFlags(cfgflags);
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
ProteusIIIPlug_SetConnectionTiming(ProteusIIIPlug_ConnectionTiming_t connectionTiming)
{
    return ProteusIII_SetConnectionTiming(connectionTiming);
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
ProteusIIIPlug_SetScanTiming(ProteusIIIPlug_ScanTiming_t scanTiming)
{
    return ProteusIII_SetScanTiming(scanTiming);
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
ProteusIIIPlug_SetTXPower(ProteusIIIPlug_TXPower_t txpower)
{
    return ProteusIII_SetTXPower(txpower);
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
ProteusIIIPlug_SetSecFlags(ProteusIIIPlug_SecFlags_t secflags)
{
    return ProteusIII_SetSecFlags(secflags);
}

/*
 *Set the UART baudrate index
 *
 *input:
 * -baudrate: UART baudrate
 * -parity: parity bit
 * -flowcontrolEnable: enable/disable flow conrol
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_SetBaudrateIndex(ProteusIIIPlug_BaudRate_t baudrate, ProteusIIIPlug_UartParity_t parity, bool flowcontrolEnable)
{
    return ProteusIII_SetBaudrateIndex(baudrate, parity, flowcontrolEnable);
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
ProteusIIIPlug_SetStaticPasskey(uint8_t *staticPasskeyP)
{
    return ProteusIII_SetStaticPasskey(staticPasskeyP);
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
ProteusIIIPlug_Get(ProteusIIIPlug_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP)
{
    return ProteusIII_Get(userSetting, ResponseP, Response_LengthP);
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
ProteusIIIPlug_GetFWVersion(uint8_t *versionP)
{
    return ProteusIII_GetFWVersion(versionP);
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
ProteusIIIPlug_GetDeviceName(uint8_t *deviceNameP, uint16_t *nameLengthP)
{
    return ProteusIII_GetDeviceName(deviceNameP, nameLengthP);
}
/*
 *Request the 8 digit MAC
 *
 *output:
 * -macP: pointer to the MAC
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_GetMAC(uint8_t *MacP)
{
    return ProteusIII_GetMAC(MacP);
}

/*
 *Request the 6 digit Bluetooth MAC
 *
 *output:
 * -BTMacP: pointer to the Bluetooth MAC
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_GetBTMAC(uint8_t *BTMacP)
{
    return ProteusIII_GetBTMAC(BTMacP);
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
ProteusIIIPlug_GetAdvertisingTimeout(uint16_t *advTimeoutP)
{
    return ProteusIII_GetAdvertisingTimeout(advTimeoutP);
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
ProteusIIIPlug_GetConnectionTiming(ProteusIIIPlug_ConnectionTiming_t *connectionTimingP)
{
    return ProteusIII_GetConnectionTiming(connectionTimingP);
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
ProteusIIIPlug_GetScanTiming(ProteusIIIPlug_ScanTiming_t *scanTimingP)
{
    return ProteusIII_GetScanTiming(scanTimingP);
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
ProteusIIIPlug_GetTXPower(ProteusIIIPlug_TXPower_t *txpowerP)
{
    return ProteusIII_GetTXPower(txpowerP);
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
ProteusIIIPlug_GetSecFlags(ProteusIIIPlug_SecFlags_t *secflagsP)
{
    return ProteusIII_GetSecFlags(secflagsP);
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
bool ProteusIIIPlug_GetBaudrateIndex(ProteusIIIPlug_BaudRate_t *baudrateP, ProteusIIIPlug_UartParity_t *parityP, bool *flowcontrolEnableP)
{
    return ProteusIII_GetBaudrateIndex(baudrateP, parityP, flowcontrolEnableP);
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
ProteusIIIPlug_GetCFGFlags(uint16_t *cfgflags)
{
    return ProteusIII_GetCFGFlags(cfgflags);
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
ProteusIIIPlug_GetState(ProteusIIIPlug_BLE_Role_t *BLE_roleP, ProteusIIIPlug_BLE_Action_t *BLE_actionP, uint8_t *InfoP, uint8_t *LengthP)
{
    return ProteusIII_GetState(BLE_roleP, BLE_actionP, InfoP, LengthP);
}

/*
 *Request the current state of the driver
 *
 *
 *return driver state
 */
ProteusIIIPlug_States_t ProteusIIIPlug_GetDriverState()
{
    return ProteusIII_GetDriverState();
}

/*
 *Start scan
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_Scanstart()
{
    return ProteusIII_Scanstart();
}
/*
 *Stop a scan
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ProteusIIIPlug_Scanstop()
{
    return ProteusIII_Scanstop();
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
ProteusIIIPlug_GetDevices(ProteusIIIPlug_GetDevices_t* devicesP)
{
    return ProteusIII_GetDevices(devicesP);
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
bool ProteusIIIPlug_Connect(uint8_t *btmac)
{
    return ProteusIII_Connect(btmac);
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
bool ProteusIIIPlug_Passkey(uint8_t* passkey)
{
    return ProteusIII_Passkey(passkey);
}

/*
 *Answer on a numeric comparison request
 *
 *input:
 * -keyIsOk: boolean to confirm if the key shown is correct
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_NumericCompareConfirm(bool keyIsOk)
{
    return ProteusIII_NumericCompareConfirm(keyIsOk);
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
bool ProteusIIIPlug_PhyUpdate(ProteusIIIPlug_Phy_t phy)
{
    return ProteusIII_PhyUpdate(phy);
}

/*
 *Configure the remote GPIO of the module
 *
 *input:
 * -configP: pointer to one or more pin configurations
 * -number_of_configs: number of entries in configP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_GPIORemoteWriteConfig(ProteusIIIPlug_GPIOConfigBlock_t* configP, uint16_t number_of_configs)
{
    return ProteusIII_GPIORemoteWriteConfig(configP, number_of_configs);
}

/*
 *Read the remote GPIO configuration of the module
 *
 *output:
 * -configP: pointer to one or more pin configurations
 * -number_of_configsP: pointer to number of entries in configP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_GPIORemoteReadConfig(ProteusIIIPlug_GPIOConfigBlock_t* configP, uint16_t *number_of_configsP)
{
    return ProteusIII_GPIORemoteReadConfig(configP, number_of_configsP);
}

/*
 *Set the output value of the remote pin. Pin has to be configured first.
 *See ProteusIIIPlug_GPIORemoteWriteConfig
 *
 *input:
 * -controlP: pointer to one or more pin controls
 * -number_of_controls: number of entries in controlP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_GPIORemoteWrite(ProteusIIIPlug_GPIOControlBlock_t* controlP, uint16_t number_of_controls)
{
    return ProteusIII_GPIORemoteWrite(controlP, number_of_controls);
}

/*
 *Read the input of the pins. Pin has to be configured first.
 *See ProteusIIIPlug_GPIORemoteWriteConfig
 *
 *input:
 * -GPIOToReadP: One or more pins to read.
 * -amountGPIOToRead: amount of pins to read and therefore length of GPIOToReadP
 *output:
 * -controlP: Pointer to controlBlock
 * -number_of_controlsP: pointer to number of entries in controlP array
 *return true if request succeeded
 *       false otherwise
 */
bool ProteusIIIPlug_GPIORemoteRead(uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ProteusIIIPlug_GPIOControlBlock_t* controlP, uint16_t* number_of_controlsP)
{
    return ProteusIII_GPIORemoteRead(GPIOToReadP, amountGPIOToRead, controlP, number_of_controlsP);
}

