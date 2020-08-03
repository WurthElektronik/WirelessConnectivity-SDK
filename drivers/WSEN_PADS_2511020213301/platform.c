/**
 ***************************************************************************************************
 * This file is part of WE sensors SDK:
 * https://www.we-online.com/sensors
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

#include "platform.h"
#include <wiringPiI2C.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


static int i2c_handle = 0; // global handle to the  i2c interface


/**
* @brief  Initialize the I2C Interface
* @param  I2C address
* @retval Error Code
*/
int8_t I2CInit(int address)
{
    if((i2c_handle = wiringPiI2CSetup(address)) < 0)
    {
        fprintf(stdout, "wiringPiI2CSetup Error\n");
        return WE_FAIL;
    }

    return WE_SUCCESS;
}

/**
* @brief   Read data starting from the addressed register
* @param  -RegAdr : the register adresse to read from
*         -NumByteToRead : number of bytes to read
*         -pointer Data : the address store the data
* @retval Error Code
*/
int8_t ReadReg(uint8_t RegAdr, int NumByteToRead, uint8_t *Data)
{
    int ret = 0;

    if(NumByteToRead > 1)
    {

        /*Write the start address to initiate the read*/
        ret = wiringPiI2CWrite(i2c_handle, RegAdr);
        if(ret == -1)
        {
            return WE_FAIL;
        }
        else
        {
            /* Read the necessary number of bytes to an array */
            read(i2c_handle, Data, NumByteToRead);
        }
    }
    else
    {
        ret = wiringPiI2CReadReg8(i2c_handle, RegAdr);
        if(ret == -1)
        {
            return WE_FAIL;
        }
        else
        {
            Data[0] = (uint8_t)ret;
        }
    }

    return WE_SUCCESS;
}


/**
* @brief  Write data strarting from the addressed register
* @param  -RegAdr : Adress to write in
*         -NumByteToWrite : number of bytes to write
*         -pointer Data : Address of the data to be written
* @retval Error Code
*/
int8_t WriteReg(int RegAdr, int NumByteToWrite, uint8_t *Data)
{
    if(0x00 != wiringPiI2CWriteReg8(i2c_handle, RegAdr, Data[0]))
    {
		return WE_FAIL;
    }
    else
    {
        int i;
        for(i = 1; i < NumByteToWrite; i++)
        {
			if(0x00 != wiringPiI2CWrite(i2c_handle,Data[i]))
            {
                return WE_FAIL;
            }
        }
    }

    return WE_SUCCESS;
}

/**         EOF         */
