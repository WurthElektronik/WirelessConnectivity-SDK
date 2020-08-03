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
#include <pthread.h>

#include <errno.h>
#include "string.h"

#include "ThebeIIPlug.h"
#include "../../drivers/ThebeII/ThebeII.h"
#include "../../drivers/WE-common.h"

#define MAX_PAYLOAD_LENGTH 128

/*
 *Initialize the ThebeIIPlug interface
 *
 *input:
 * -baudrate:  baudrate of the interface
 * -rp:        reset pin number
 * -RXcb:      RX callback function
 * -addrmode:  address mode of the ThebeIIPlug
 *
 *Caution: the parameters baudrate and addrmode must match the configured UserSettings of the ThebeIIPlug
 *         -the baudrate parameter must match to perform a successful UART communication
 *          *updating this parameter during runtime may lead to communication errors
 *         -the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in ThebeIIPlug_Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool ThebeIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), ThebeII_AddressMode_t addrmode)
{
    return ThebeII_Init(baudrate, rp, PIN_INVALID, PIN_INVALID, RXcb, addrmode);
}

/*
 *Deinitialize the ThebeIIPlug interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool ThebeIIPlug_Deinit()
{
    return ThebeII_Deinit();
}

/*
 *Reset the ThebeIIPlug by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ThebeIIPlug_PinReset()
{
    return ThebeII_PinReset();
}

/*
 *Reset the ThebeIIPlug by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool ThebeIIPlug_Reset()
{
    return ThebeII_Reset();
}

/*
 *Factory reset the ThebeIIPlug
 *
 *note: use them only in rare cases, since flash can be updated only a limited number times
 *
 *return true if factory reset succeeded
 *       false otherwise
 */
bool ThebeIIPlug_FactoryReset()
{
    return ThebeII_FactoryReset();
}

/*
 *Request the current ThebeIIPlug settings
 *
 *input:
 * -us: user setting to be requested
 *
 *output:
 * -response: pointer of the memory to put the request content
 * -response_length: length of the request content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_Get(ThebeIIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length)
{
    return ThebeII_Get(us, response, response_length);
}

/*
 *Set a special ThebeIIPlug setting
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
bool ThebeIIPlug_Set(ThebeIIPlug_UserSettings_t us, uint8_t* value, uint8_t length)
{
    return ThebeII_Set(us, value, length);
}


/*
 *Request the 3 byte firmware version
 *
 *output:
 * -fw: pointer to the 3 byte firmware version
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetFirmwareVersion(uint8_t* fw)
{
    return ThebeII_GetFirmwareVersion(fw);
}

/*
 *Request the 4 byte serial number
 *
 *output:
 * -sn: pointer to the 4 byte serial number
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetSerialNumber(uint8_t* sn)
{
    return ThebeII_GetSerialNumber(sn);
}

/*
 *Request the default TX power
 *
 *output:
 * -txpower: pointer to the TXpower
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetDefaultTXPower(uint8_t* txpower)
{
    return ThebeII_GetDefaultTXPower(txpower);
}

/*
 *Get the default destination address
 *
 *output:
 * -destaddr_lsb: LSB of the destination address
 * -destaddr_msb: MSB of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* destaddr_msb)
{
    return ThebeII_GetDefaultDestAddr(destaddr_lsb, destaddr_msb);
}

/*
 *Get the default destination address
 *
 *output:
 * -destnetid: destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetDefaultDestNetID(uint8_t* destnetid)
{
    return ThebeII_GetDefaultDestNetID(destnetid);
}

/*
 *Get the default source address
 *
 *output:
 * -srcaddr_lsb: LSB of the source address
 * -srcaddr_msb: MSB of the source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* destaddr_msb)
{
    return ThebeII_GetSourceAddr(srcaddr_lsb, destaddr_msb);
}

/*
 *Get the default source address
 *
 *output:
 * -srcnetid: source net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetSourceNetID(uint8_t* srcnetid)
{
    return ThebeII_GetSourceNetID(srcnetid);
}

/*
 *Get the default RF channel
 *
 *output:
 * -channel: channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetDefaultRFChannel(uint8_t* channel)
{
    return ThebeII_GetDefaultRFChannel(channel);
}

/*
 *Get the default RF profile
 *
 *output:
 * -profile: RF profile
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_GetDefaultRFProfile(uint8_t* profile)
{
    return ThebeII_GetDefaultRFProfile(profile);
}

/*
 *Set the default TX power
 *
 *input:
 * -txpower: TXpower
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThebeIIPlug_SetVolatile_TXPower for frequent adaption of the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetDefaultTXPower(uint8_t txpower)
{
    return ThebeII_SetDefaultTXPower(txpower);
}

/*
 *Set the default destination address
 *
 *input:
 * -destaddr_lsb: LSB of the destination address
 * -destaddr_msb: MSB of the destination address
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThebeIIPlug_SetVolatile_DestAddr for frequent adaption of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb)
{
    return ThebeII_SetDefaultDestAddr(destaddr_lsb, destaddr_msb);
}

/*
 *Set the default destination address
 *
 *input:
 * -destnetid: destination net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThebeIIPlug_SetVolatile_DestNetID for frequent adaption of the destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetDefaultDestNetID(uint8_t destnetid)
{
    return ThebeII_SetDefaultDestNetID( destnetid);
}

/*
 *Set the default source address
 *
 *input:
 * -srcaddr_lsb: LSB of the source address
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb)
{
    return ThebeII_SetSourceAddr(srcaddr_lsb, srcaddr_msb);
}

/*
 *Set the default source net id
 *
 *input:
 * -srcnetid: source net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetSourceNetID(uint8_t srcnetid)
{
    return ThebeII_SetSourceNetID(srcnetid);
}

/*
 *Set the default RF channel
 *
 *input:
 * -channel: channel
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use ThebeIIPlug_SetVolatile_Channel for frequent adaption of the channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetDefaultRFChannel(uint8_t channel)
{
    return ThebeII_SetDefaultRFChannel(channel);
}

/*
 *Set the default RF profile
 *
 *input:
 * -profile: RF profile
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetDefaultRFProfile(uint8_t profile)
{
    return ThebeII_SetDefaultRFProfile(profile);
}
/*
 *Set the volatile TX power
 *
 *input:
 * -power: new TX power value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetVolatile_TXPower(uint8_t power)
{
    return ThebeII_SetVolatile_TXPower(power);
}

/*
 *Set the volatile channel
 *
 *input:
 * -channel: new channel value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetVolatile_Channel(uint8_t channel)
{
    return ThebeII_SetVolatile_Channel(channel);
}

/*
 *Set the volatile destination net ID
 *
 *input:
 * -destnetid: new destination net ID
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetVolatile_DestNetID(uint8_t destnetid)
{
    return ThebeII_SetVolatile_DestNetID(destnetid);
}

/*
 *Set the volatile destination address
 *
 *input:
 * -destaddr_lsb: lsb of the new destination address value
 * -destaddr_msb: msb of the new destination address value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb)
{
    return ThebeII_SetVolatile_DestAddr(destaddr_lsb,destaddr_msb);
}

/*
 *Transmit data using the configured settings
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_Transmit(uint8_t* payload, uint8_t length)
{
    if(length > MAX_PAYLOAD_LENGTH)
    {
        fprintf(stdout, "Data exceeds maximal payload length\n");

        return false;
    }
    else
    {
        return ThebeII_Transmit(payload,length);
    }
}

/*
 *Transmit data
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 * -channel: channel to be used
 * -dest_network_id: destination network ID
 * -dest_address_lsb: destination address lsb
 * -dest_address_msb: destination address msb
 *
 *return true if request succeeded
 *       false otherwise
 */
bool ThebeIIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb)
{
    if(length > MAX_PAYLOAD_LENGTH)
    {
        fprintf(stdout, "Data exceeds maximal payload length\n");

        return false;
    }
    else
    {
        return ThebeII_Transmit_Extended(payload, length, channel, dest_network_id, dest_address_lsb, dest_address_msb);
    }
}

/*
 *Configure the ThebeIIPlug
 *
 *input:
 * -config: pointer to the configuration struct
 * -config_length: length of the configuration struct
 * -factory_reset: apply a factory reset before or not
 *
 *return true if request succeeded
 *       false otherwise
*/
bool ThebeIIPlug_Configure(ThebeIIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset)
{
    return ThebeII_Configure(config, config_length, factory_reset);
}
