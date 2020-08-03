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


#include "ThyoneIPlug.h"
#include "../ThyoneI/ThyoneI.h"
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
 *Initialize the ThyoneIPlug interface for serial interface
 *
 *input:
 * -baudrate:  baudrate of the interface
 * -rp:        Reset Pin
 * -RXcb:      RX callback function called when data was received via radio
 *
 *Caution: the parameter baudrate must match the configured UserSettings of the ThyoneIPlug
 *         -the baudrate parameter must match to perform a successful FTDI communication
 *          *updating this parameter during runtime may lead to communication errors
 *
 *return true if initialization succeeded
 *       false otherwise
 */
extern bool ThyoneIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint16_t,uint32_t,int8_t))
{
    return ThyoneI_Init(baudrate, rp, PIN_INVALID, PIN_INVALID, RXcb);
}

/*
 *Deinitialize the ThyoneIPlug interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ThyoneIPlug_Deinit()
{
    return ThyoneI_Deinit();
}

/*
 *Wakeup the ThyoneIPlug from sleep by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool ThyoneIPlug_PinWakeup()
{
    return ThyoneI_PinWakeup();
}

/*
 *Reset the ThyoneIPlug by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ThyoneIPlug_PinReset()
{
    return ThyoneI_PinReset();
}

/*
 *Reset the ThyoneI by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ThyoneIPlug_Reset()
{
    return ThyoneI_Reset();
}

/*
 *Transmit data as broadcast
 *
 *input:
 * -PayloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool ThyoneIPlug_TransmitBroadcast(uint8_t *payloadP, uint16_t length)
{
    return ThyoneI_TransmitBroadcast(payloadP, length);
}

/*
 *Transmit data as multicast
 *
 *input:
 * -PayloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool ThyoneIPlug_TransmitMulticast(uint8_t *payloadP, uint16_t length)
{
    return ThyoneI_TransmitMulticast(payloadP, length);
}

/*
 *Transmit data as unicast
 *
 *input:
 * -PayloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool ThyoneIPlug_TransmitUnicast(uint8_t *payloadP, uint16_t length)
{
    return ThyoneI_TransmitUnicast(payloadP, length);
}

/*
 *Transmit data as multicast
 *
 *input:
 * -groupID : groupID to multicast
 * -PayloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool ThyoneIPlug_TransmitMulticastExtended(uint8_t groupID, uint8_t *payloadP, uint16_t length)
{
    return ThyoneI_TransmitMulticastExtended(groupID, payloadP, length);
}

/*
 *Transmit data as unicast
 *
 *input:
 * -address: address to sent to
 * -payloadP: pointer to the data to transmit
 * -length:   length of the data to transmit
 *
 *return true if succeeded
 *       false otherwise
 */
bool ThyoneIPlug_TransmitUnicastExtended(uint32_t address, uint8_t *payloadP, uint16_t length)
{
    return ThyoneI_TransmitUnicastExtended(address, payloadP, length);
}

/*
 *Factory reset the module
 *
 *return true if succeeded
 *       false otherwise
 */
bool ThyoneIPlug_FactoryReset()
{
    return ThyoneI_FactoryReset();
}

/*
 *Set a special user setting
 *
 *input:
 * -us:     user setting to be updated
 * -value:  pointer to the new settings value
 * -length: length of the value
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_Set(ThyoneIPlug_UserSettings_t userSetting, uint8_t *ValueP, uint8_t length)
{
    return ThyoneI_Set(userSetting, ValueP, length);
}

/*
 *Set the tx power
 *
 *input:
 * -txPower: transmit power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool
ThyoneIPlug_SetTXPower(ThyoneI_TXPower_t txPower)
{
    return ThyoneI_SetTXPower(txPower);
}

/*
 *Set the UART baudrate index
 *
 *input:
 * -baudrateindex: UART baudrate index
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetBaudrateIndex(ThyoneIPlug_BaudRateIndex_t baudrate, ThyoneIPlug_UartParity_t parity, bool flowcontrolEnable)
{
    return ThyoneI_SetBaudrateIndex(baudrate, parity, flowcontrolEnable);
}

/*
 *Set the encryption mode
 *
 *input:
 * -encryptionMode: encryptionMode
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetEncryptionMode(ThyoneIPlug_EncryptionMode_t encryptionMode)
{
    return ThyoneI_SetEncryptionMode(encryptionMode);
}

/*
 *Set the rf profile
 *
 *input:
 * -profile: rf profile
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetRfProfile(ThyoneIPlug_Profile_t profile)
{
    return ThyoneI_SetRfProfile(profile);
}

/*
 *Set the rf channel
 *
 *input:
 * -channel: rf channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetRFChannel(uint8_t channel)
{
    return ThyoneI_SetRFChannel(channel);
}

/*
 *Set the number of retries
 *
 *input:
 * -numRetries: number of retries
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetNumRetries(uint8_t numRetries)
{
    return ThyoneI_SetNumRetries(numRetries);
}

/*
 *Set the number of time slots
 *
 *input:
 * -numSlots: number of time slots
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetRpNumSlots(uint8_t numSlots)
{
    return ThyoneI_SetRpNumSlots(numSlots);
}

/*
 *Set the source address
 *
 *input:
 * -sourceAddress: source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetSourceAddress(uint32_t sourceAddress)
{
    return ThyoneI_SetSourceAddress(sourceAddress);
}

/*
 *Set the destination address
 *
 *input:
 * -destinationAddress: destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetDestinationAddress(uint32_t destinationAddress)
{
    return ThyoneI_SetDestinationAddress(destinationAddress);
}

/*
 *Set the group id
 *
 *input:
 * -groupid: groupID
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetGroupID(uint8_t groupId)
{
    return ThyoneI_SetGroupID(groupId);
}

/*
 *Set the encryption key
 *
 *input:
 * -keyP: pointer to 16-byte key
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetEncryptionKey(uint8_t *keyP)
{
    return ThyoneI_SetEncryptionKey(keyP);
}

/*
 *Set the time-to-live defining the maximum of hops if repeating
 *
 *input:
 * -ttl: time-to-live value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetTimeToLive(uint8_t ttl)
{
    return ThyoneI_SetTimeToLive(ttl);
}

/*
 *Set clear channel assessement mode
 *
 *input:
 * -ccaMode: clear channel assessment mode
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetCCAMode(uint8_t ccaMode)
{
    return ThyoneI_SetCCAMode(ccaMode);
}

/*
 *Set threshold for clear channel assessement
 *
 *input:
 * -ccaThreshold: threshold for clear channel assessement
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetCCAThreshold(uint8_t ccaThreshold)
{
    return ThyoneI_SetCCAThreshold(ccaThreshold);
}

/*
 *Set remote gpio config
 *
 *input:
 * -remoteConfig: remote gpio config
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetGPIOBlockRemoteConfig(uint8_t remoteConfig)
{
    return ThyoneI_SetGPIOBlockRemoteConfig(remoteConfig);
}

/*
 *Set module mode
 *
 *input:
 * -moduleMode: operation mode of the module
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_SetModuleMode(ThyoneIPlug_OperatingMode_t moduleMode)
{
    return ThyoneI_SetModuleMode(moduleMode);
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
bool ThyoneIPlug_Get(ThyoneIPlug_UserSettings_t userSetting, uint8_t *ResponseP, uint16_t *Response_LengthP)
{
    return ThyoneI_Get(userSetting, ResponseP, Response_LengthP);
}

/*
 *Request the 4 byte serial number
 *
 *output:
 * -versionP: pointer to the 4 byte serial number
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetSerialNumber(uint8_t *serialNumberP)
{
    return ThyoneI_GetSerialNumber(serialNumberP);
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
bool ThyoneIPlug_GetFWVersion(uint8_t *versionP)
{
    return ThyoneI_GetFWVersion(versionP);
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
bool ThyoneIPlug_GetTXPower(ThyoneIPlug_TXPower_t *txpowerP)
{
    return ThyoneI_GetTXPower(txpowerP);
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
bool ThyoneIPlug_GetBaudrateIndex(ThyoneIPlug_BaudRateIndex_t *baudrate, ThyoneIPlug_UartParity_t *parity, bool *flowcontrolEnable)
{
    return ThyoneI_GetBaudrateIndex(baudrate, parity, flowcontrolEnable);
}

/*
 *Get the encryption mode
 *
 *output:
 * -encryptionModeP: Pointer to encryptionMode
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetEncryptionMode(ThyoneIPlug_EncryptionMode_t *encryptionModeP)
{
    return ThyoneI_GetEncryptionMode(encryptionModeP);
}

/*
 *Get the rf profile
 *
 * *output:
 * -profileP: Pointer to rf profile
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetRfProfile(ThyoneIPlug_Profile_t *profileP)
{
    return ThyoneI_GetRfProfile(profileP);
}

/*
 *Get the rf channel
 *
 * *output:
 * -channelP: Pointer to rf channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetRFChannel(uint8_t *channelP)
{
    return ThyoneI_GetRFChannel(channelP);
}

/*
 *Get the number of retries
 *
 *output:
 * -numRetriesP: Pointer to number of retries
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetNumRetries(uint8_t *numRetriesP)
{
    return ThyoneI_GetNumRetries(numRetriesP);
}

/*
 *Get the number of time slots
 *
 *output:
 * -numSlotsP: pointer of number of time slots
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetRpNumSlots(uint8_t *numSlotsP)
{
    return ThyoneI_GetRpNumSlots(numSlotsP);
}

/*
 *Get the source address
 *
 *output:
 * -sourceAddressP: pointer to source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetSourceAddress(uint32_t *sourceAddressP)
{
    return ThyoneI_GetSourceAddress(sourceAddressP);
}

/*
 *Get the destination address
 *
 *output:
 * -destinationAddressP: pointer to destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetDestinationAddress(uint32_t *destinationAddressP)
{
    return ThyoneI_GetDestinationAddress(destinationAddressP);
}

/*
 *Get the group id
 *
 *output:
 * -groupIdP: pointer to groupID
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetGroupID(uint8_t *groupIdP)
{
    return ThyoneI_GetGroupID(groupIdP);
}

/*
 *Fet the time-to-live defining the maximum of hops if repeating
 *
 *output:
 * -ttlP: pointer to time-to-live
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetTimeToLive(uint8_t *ttlP)
{
    return ThyoneI_GetTimeToLive(ttlP);
}

/*
 *Get clear channel assessement mode
 *
 *output:
 * -ccaModeP: pointer to clear channel assessment mode
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetCCAMode(uint8_t *ccaModeP)
{
    return ThyoneI_GetCCAMode(ccaModeP);
}

/*
 *Get threshold for clear channel assessement
 *
 *output:
 * -ccaThreshold: threshold for clear channel assessement
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetCCAThreshold(uint8_t *ccaThresholdP)
{
    return ThyoneI_GetCCAThreshold(ccaThresholdP);
}

/*
 *Get remote gpio config
 *
 *output:
 * -remoteConfigP: pointer to remote gpio config
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetGPIOBlockRemoteConfig(uint8_t *remoteConfigP)
{
    return ThyoneI_GetGPIOBlockRemoteConfig(remoteConfigP);
}

/*
 *Get module mode
 *
 *output:
 * -moduleMode: operation mode of the module
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GetModuleMode(ThyoneIPlug_OperatingMode_t *moduleModeP)
{
    return ThyoneI_GetModuleMode(moduleModeP);
}

/*
 *Configure the remote GPIO of the module
 *
 *input:
 * -destAddress: Destination address of the remote Thyone-I device
 * -configP: pointer to one or more pin configurations
 * -configLength: length of data configP points to
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GPIORemoteSetConfig(uint32_t destAddress, ThyoneI_GPIOConfigBlock_t* configP, uint16_t configLength)
{
    return ThyoneI_GPIORemoteSetConfig(destAddress, configP, configLength);
}

/*
 *Read the remote GPIO configuration of the module
 *
 *output:
 * -destAddress: Destination address of the remote Thyone-I device
 * -configP: pointer to one or more pin configurations
 * -configLengthP: length of data configP points to
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GPIORemoteGetConfig(uint32_t destAddress, ThyoneI_GPIOConfigBlock_t* configP, uint16_t *configLengthP)
{
    return ThyoneI_GPIORemoteGetConfig(destAddress, configP, configLengthP);
}

/*
 *Set the output value of the remote pin. Pin has to be configured first.
 *See ThyoneI_GPIORemoteWriteConfig
 *
 *input:
 * -destAddress: Destination address of the remote Thyone-I device
 * -controlP: pointer to one or more pin controls
 * -configLength: length of data configP controlP to
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GPIORemoteWrite(uint32_t destAddress, ThyoneI_GPIOControlBlock_t* controlP, uint16_t controlLength)
{
    return ThyoneI_GPIORemoteWrite(destAddress, controlP, controlLength);
}

/*
 *Read the input of the pins. Pin has to be configured first.
 *See ThyoneI_GPIORemoteWriteConfig
 *
 *input:
 * -destAddress: Destination address of the remote Thyone-I device
 * -GPIOToReadP: One or more pins to read.
 * -amountGPIOToRead: amount of pins to read and therefore length of GPIOToReadP
 *output:
 * -controlP: Pointer to controlBlock
 * -controlLengthP: length of controlP
 *return true if request succeeded
 *       false otherwise
 */
bool ThyoneIPlug_GPIORemoteRead(uint32_t destAddress, uint8_t *GPIOToReadP, uint8_t amountGPIOToRead, ThyoneI_GPIOControlBlock_t* controlP, uint16_t* controlLengthP)
{
    return ThyoneI_GPIORemoteRead(destAddress, GPIOToReadP, amountGPIOToRead, controlP, controlLengthP);
}

