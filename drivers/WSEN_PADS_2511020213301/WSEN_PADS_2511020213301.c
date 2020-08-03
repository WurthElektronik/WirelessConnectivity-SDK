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
 
#include "WSEN_PADS_2511020213301.h"
#include <stdio.h>


/**
* @brief  Read the device ID
* @param  no parameter.
* @retval Device ID
*/
uint8_t PADS_getDeviceID()
{
    uint8_t deviceID = 0;
    if(ReadReg( (uint8_t)PADS_DEVICE_ID_REG, 1, &deviceID))
        return WE_FAIL;

    return deviceID; /* expected value is PADS_DEVICE_ID_VALUE */

}

/**
* @brief  Set the output data rate of the sensor
* @param  output data rate
* @retval Error code
*/
int8_t PADS_setOutputDataRate(PADS_output_data_rate_t odr)
{

    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG , 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

    ctrl_reg_1.outputDataRate = odr;

    if(WriteReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the output data rate of the sensor
* @param  no parameter.
* @retval Output Data Rate
*/
uint8_t PADS_getOutputDataRate()
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

    return ctrl_reg_1.outputDataRate;
}

/**
* @brief  Set the low pass filter state
* @param  filter state
* @retval Error code
*/
int8_t PADS_setLowPassFilter(PADS_state_t filterStatus)
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG , 1, (uint8_t *)&ctrl_reg_1))
		return WE_FAIL;

    ctrl_reg_1.enLowPassFilter = filterStatus;

    if(WriteReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Get the status of the low pass filter
* @param  no parameter.
* @retval Low pass filter state
*/
uint8_t PADS_getLowPassFilterStatus()
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
            return WE_FAIL;

     return ctrl_reg_1.enLowPassFilter;
}

/**
* @brief  Set the low pass filter configuration
* @param  configuration parameter
* @retval Error code
*/
int8_t PADS_setLowPassFilterConf(PADS_filter_conf_t conf)
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG , 1, (uint8_t *)&ctrl_reg_1))
		return WE_FAIL;

    ctrl_reg_1.lowPassFilterConfig = conf;

    if(WriteReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

   return WE_SUCCESS;
}


/**
* @brief  Read the low pass filter configuration
* @param  no parameter.
* @retval Low pass filter configuration
*/
uint8_t PADS_getLowPassFilterConf()
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

    return ctrl_reg_1.lowPassFilterConfig;
}

/**
* @brief  Set the block data update state
* @param  BDU state
* @retval Error code
*/
int8_t PADS_setBlockDataUpdate(PADS_state_t bdu)
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG , 1, (uint8_t *)&ctrl_reg_1))
		return WE_FAIL;

    ctrl_reg_1.blockDataUpdate = bdu;

    if(WriteReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief Read the block data update state
* @param  no parameter.
* @retval Block data update state
*/
uint8_t PADS_getBlockDataUpdateState()
{
    PADS_ctrl_1_t ctrl_reg_1;

    if(ReadReg((uint8_t)PADS_CTRL_1_REG, 1, (uint8_t *)&ctrl_reg_1))
		return WE_FAIL;

    return (ctrl_reg_1.blockDataUpdate);
}

/**
* @brief  Set the power mode of the sensor [Low noise , low current]
* @param  Power mode
* @retval Error code
*/
int8_t  PADS_setPowerMode(PADS_pwr_mode_t mode)
{
	PADS_ctrl_2_t ctrl_reg_2;

	if(ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

	ctrl_reg_2.lowNoiseMode = mode;

	if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

	return WE_SUCCESS;
}

/**
* @brief  Read the power mode [Low noise , low current]
* @param  no parameter.
* @retval Power mode
*/
uint8_t PADS_getPowerMode()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return ctrl_reg_2.lowNoiseMode;
}

/**
* @brief  Set the Auto increment feature
* @param  state
* @retval Error code
*/
int8_t PADS_setAutoIncrement(PADS_state_t autoInc)
{
    PADS_ctrl_2_t ctrl_reg_2;

    if (ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    ctrl_reg_2.autoAddIncr = autoInc;

    if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1,(uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the auto increment state
* @param  no parameter.
* @retval auto Increment state
*/
uint8_t PADS_getAutoIncrementState()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return ctrl_reg_2.autoAddIncr;
 }

/**
* @brief  Set memory reboot state
* @param  reboot state
* @retval Error code
*/
int8_t PADS_rebootMemory(PADS_state_t reboot)
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    ctrl_reg_2.boot = reboot;

    if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1,(uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the memory reboot state
* @param  no parameter.
* @retval Reboot memory state
*/
uint8_t PADS_getRebootMemoryState()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return  ctrl_reg_2.boot;
}

/**
* @brief  Read the boot state
* @param  no parameter.
* @retval Boot state
*/
uint8_t PADS_getBootStatus()
{
    PADS_intSource_t int_source_reg;

    if(ReadReg((uint8_t)PADS_INT_SOURCE_REG, 1, (uint8_t *)&int_source_reg))
		return WE_FAIL;

    return int_source_reg.bootOn;
}

/**
* @brief  Set the interrupt active level
* @param  active level
* @retval Error code
*/
int8_t PADS_setIntActiveLevel(PADS_interrupt_level_t intLvl)
{
    PADS_ctrl_2_t ctrl_reg_2;

    if (ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    ctrl_reg_2.intActiveLevel = intLvl;

   if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
        return WE_FAIL;

   return WE_SUCCESS;
}

/**
* @brief  Read the interrupt active level [High,Low]
* @param  no parameter.
* @retval Interrupt active level
*/
uint8_t PADS_getIntActiveLevel()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return ctrl_reg_2.intActiveLevel;
}

/**
* @brief  Select the interrupt pin type [Push-pull,open_drain]
* @param  pin type
* @retval Error code
*/
int8_t  PADS_setIntPinType(PADS_interrupt_pin_t intr)
{
    PADS_ctrl_2_t ctrl_reg_2;

    if (ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    ctrl_reg_2.openDrainOnINTpin = intr;

    if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the interrupt type [Push-pull,open_drain]
* @param  no parameter.
* @retval interrupt pin type
*/
uint8_t PADS_getIntPinType()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return ctrl_reg_2.openDrainOnINTpin;
}

/**
* @brief  Set the single conversion mode[enabled,disabled]
* @param  mode
* @retval Error code
*/
int8_t  PADS_setSingleConvMode(PADS_state_t mode)
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    ctrl_reg_2.oneShotbit = mode;

    if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the Single Conversion Mode state [enabled,disabled]
* @param  no parameter.
* @retval Single Conversion Mode state
*/
uint8_t PADS_getSingleConvModeState()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return ctrl_reg_2.oneShotbit;
}

/**
* @brief  Set software reset [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setSwreset (PADS_state_t mode)
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG , 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    ctrl_reg_2.softwareReset = mode;

    if(WriteReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the software reset state [enabled,disabled]
* @param  no parameter.
* @retval software reset state
*/
uint8_t PADS_getSwresetState()
{
    PADS_ctrl_2_t ctrl_reg_2;

    if(ReadReg((uint8_t)PADS_CTRL_2_REG, 1, (uint8_t *)&ctrl_reg_2))
		return WE_FAIL;

    return (ctrl_reg_2.softwareReset);
}

/**
* @brief Set the fifo full interrupt  [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setFifoFullInt(PADS_state_t fullState)
{
    PADS_ctrl_3_t ctrl_reg_3;

    if(ReadReg((uint8_t)PADS_CTRL_3_REG , 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    ctrl_reg_3.fifoFullInt = fullState;

    if(WriteReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Set the fifo threshold level interrupt  [enabled,disabled]
* @param  state
* @retval Error code [Error,NoError]
*/
int8_t PADS_setFifoWtmInt(PADS_state_t WtmState)
{
    PADS_ctrl_3_t ctrl_reg_3;

    if(ReadReg((uint8_t)PADS_CTRL_3_REG , 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    ctrl_reg_3.fifoThresholdInt = WtmState;


    if(WriteReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Enable the fifo overrun interrupt   [enabled,disabled]
* @param  a variable , its Type is state
* @retval Error code [Error,NoError]
*/
int8_t PADS_setFifoOvrInterrupt(PADS_state_t OvrState)
{
    PADS_ctrl_3_t ctrl_reg_3;

    if(ReadReg((uint8_t)PADS_CTRL_3_REG , 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    ctrl_reg_3.fifoOverrunInt = OvrState;

    if(WriteReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the fifo full state [enabled,disabled]
* @param  no parameter.
* @retval fifo full state
*/
uint8_t PADS_getFifoFullState()
{
    PADS_fifo_status2_t fifo_status2;

    if(ReadReg((uint8_t)PADS_FIFO_STATUS2_REG, 1, (uint8_t *)&fifo_status2))
		return WE_FAIL;

    return (fifo_status2.fifoFull);
}

/**
* @brief  Read if the fifo level has exceeded the user defined watermark[enabled,disabled]
* @param  no parameter.
* @retval Fifo threshold level state
*/
uint8_t PADS_getFifoWtmState()
{
    PADS_fifo_status2_t fifo_status2;

    if(ReadReg((uint8_t)PADS_FIFO_STATUS2_REG, 1, (uint8_t *)&fifo_status2))
		return WE_FAIL;

    return (fifo_status2.fifoWtm);
}

/**
* @brief  Read the fifo overrun  state [enabled,disabled]
* @param  no parameter.
* @retval fifo overrun state
*/
uint8_t PADS_getFifoOvrState()
{
    PADS_fifo_status2_t fifo_status2;

    if(ReadReg((uint8_t)PADS_FIFO_STATUS2_REG, 1, (uint8_t *)&fifo_status2))
		return WE_FAIL;

    return (fifo_status2.fifoOverrun);
}


/**
* @brief  Set the data ready signal interrupt    [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setDrdyInt(PADS_state_t DRDYStatus)
{

    PADS_ctrl_3_t ctrl_reg_3;

    if (ReadReg((uint8_t)PADS_CTRL_3_REG , 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    ctrl_reg_3.dataReadyInt = DRDYStatus;


    if(WriteReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the data-ready signal interrupt state [enabled,disabled]
* @param  no parameter.
* @retval Data-ready signal interrupt state
*/
uint8_t PADS_getDrdyIntState()
{
    PADS_ctrl_3_t ctrl_reg_3;

    if(ReadReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return (ctrl_reg_3.dataReadyInt);
}

/**
* @brief  Set the interrupt contorl event
* @param  event
* @retval Error code
*/
int8_t PADS_setIntEventCtrl(PADS_interrupt_event_t ctr)
{

    PADS_ctrl_3_t ctrl_reg_3;

    if (ReadReg((uint8_t)PADS_CTRL_3_REG , 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    ctrl_reg_3.intEventCtrl = ctr;

    if(WriteReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the interrupt contorl event
* @param  no parameter.
* @retval Interrupt contol event
*/
uint8_t PADS_getIntEventCtrlState()
{
    PADS_ctrl_3_t ctrl_reg_3;

    if(ReadReg((uint8_t)PADS_CTRL_3_REG, 1, (uint8_t *)&ctrl_reg_3))
		return WE_FAIL;

    return (ctrl_reg_3.intEventCtrl);
}


/**
* @brief  Check if the temperature data are available
* @param  no parameter.
* @retval temperature data State
*/
uint8_t PADS_getTempStatus()
{
    PADS_status_t status_reg;

    if(ReadReg((uint8_t)PADS_STATUS_REG, 1, (uint8_t *)&status_reg))
		return WE_FAIL;

    return (status_reg.tempDataAvailable);
}

/**
* @brief  Check if the pressure data are available
* @param  no parameter.
* @retval Pressure data State
*/
uint8_t PADS_getPresStatus()
{
    PADS_status_t status_reg;

    if(ReadReg((uint8_t)PADS_STATUS_REG, 1, (uint8_t *)&status_reg))
		return WE_FAIL;

    return (status_reg.presDataAvailable);
 }

/**
* @brief  Check if the temperature data register has been overwritten
* @param  no parameter.
* @retval temperature overwritten State
*/
uint8_t PADS_getTemperOverrunStatus()
{
    PADS_status_t status_reg;

    if(ReadReg((uint8_t)PADS_STATUS_REG, 1, (uint8_t *)&status_reg))
		return WE_FAIL;

    return (status_reg.tempDataOverrun);
}

/**
* @brief  Check if the pressure data register has been overwritten
* @param  no parameter.
* @retval pressure overwritten State
*/
uint8_t PADS_getPressureOverrunStatus()
{
    PADS_status_t status_reg;

    if(ReadReg((uint8_t)PADS_STATUS_REG, 1, (uint8_t *)&status_reg))
		return WE_FAIL;

    return (status_reg.presDataOverrun );
}

/**
* @brief Set autorefp function   [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setAutoRefp(PADS_state_t autorep)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
		return WE_FAIL;

    interrupt_Configuration_reg.autoRefp = autorep;


    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the autorefp function state [enabled,disabled]
* @param  no parameter.
* @retval autorefp function state
*/
uint8_t PADS_getAutoRefpState()
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if(ReadReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
		return WE_FAIL;

    return (interrupt_Configuration_reg.autoRefp );
}


/**
* @brief  Reset the autorefp function
* @param  state
* @retval Error code
*/
int8_t PADS_resetAutoRefp(PADS_state_t autorep)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    interrupt_Configuration_reg.resetAutoRefp = autorep;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;
}


/**
* @brief Set the autozero function [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setAutoZeroFunction(PADS_state_t autozero)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;
	
    interrupt_Configuration_reg.autoZeroFunc = autozero;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the autozero function state [enabled,disabled]
* @param  no parameter.
* @retval autozero function state
*/
uint8_t PADS_getAutoZeroState()
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if(ReadReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
		return WE_FAIL;

    return (interrupt_Configuration_reg.autoZeroFunc );
}

/**
* @brief  Reset autozero function
* @param  state
* @retval Error code
*/
int8_t PADS_resetAutoZeroFunction (PADS_state_t autozeroRst)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    interrupt_Configuration_reg.resetAutoZeroFunc = autozeroRst;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Set the differential pressure interrupt [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t  PADS_setDiffPresInt(PADS_state_t diffEn)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    interrupt_Configuration_reg.diffInt = diffEn;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;

}

/**
* @brief  Read the differential interrupt state [enabled,disabled]
* @param  no parameter.
* @retval differential interrupt state
*/
uint8_t PADS_getDiffPresIntState()
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if(ReadReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return (interrupt_Configuration_reg.diffInt );
}

/**
* @brief  Set latched Interrupt  [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setLatchInt(PADS_state_t state)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    interrupt_Configuration_reg.latchedInt = state;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the state of the latch Interrupt [enabled,disabled]
* @param  no parameter.
* @retval latch Interrupt state
*/
uint8_t PADS_getLatchIntState()
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if(ReadReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return (interrupt_Configuration_reg.latchedInt);
}

/**
* @brief  Set the interrupt on pressure low event [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setLowPresInt(PADS_state_t state)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    interrupt_Configuration_reg.lowPresInt = state;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the state of the interrupt on pressure low event [enabled,disabled]
* @param  no parameter.
* @retval state of the interrupt on pressure low event
*/
uint8_t PADS_getLowPresIntState()
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if(ReadReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return (interrupt_Configuration_reg.lowPresInt);
}

/**
* @brief  Set the interrupt on pressure High event [enabled,disabled]
* @param  state
* @retval Error code
*/
int8_t PADS_setHighPresInt(PADS_state_t state)
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if (ReadReg((uint8_t)PADS_INT_CFG_REG , 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    interrupt_Configuration_reg.highPresInt  = state;

    if(WriteReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the state of the interrupt on pressure High event [enabled,disabled]
* @param  no parameter.
* @retval state of the interrupt on pressure High event
*/
uint8_t PADS_getHighPresIntState()
{
    PADS_interrupt_configuration_t interrupt_Configuration_reg;

    if(ReadReg((uint8_t)PADS_INT_CFG_REG, 1, (uint8_t *)&interrupt_Configuration_reg))
            return WE_FAIL;

     return (interrupt_Configuration_reg.highPresInt);
}


/**
* @brief  Set the LSB pressure threshold value
* @param  value
* @retval Error code
*/

int8_t PADS_setPresThrLSB(uint8_t thr)
{
    uint8_t thresholdLSB = thr;

    if(WriteReg((uint8_t)PADS_THR_P_L_REG, 1, &thresholdLSB))
		return WE_FAIL;

	return WE_SUCCESS;
}

/**
* @brief  Set the MSB pressure threshold value
* @param  value
* @retval Error code
*/
int8_t  PADS_setPresThrMSB(uint8_t thr)
{
    uint8_t thresholdMSB = thr;

    if(WriteReg((uint8_t)PADS_THR_P_H_REG, 1, &thresholdMSB))
		return WE_FAIL;

	return WE_SUCCESS;
}

/**
* @brief  Read LSB threshold pressure value
* @param  no parameter.
* @retval threshold pressure LSB value
*/
uint8_t PADS_getPresThrLSB()
{
    uint8_t thresholdLSB = 0;

    if(ReadReg( (uint8_t)PADS_THR_P_L_REG, 1, &thresholdLSB))
        return WE_FAIL;

    return thresholdLSB;
}

/**
* @brief  Read MSB threshold pressure value
* @param  no parameter.
* @retval threshold pressure MSB value
*/
uint8_t PADS_getPresThrMSB()
{
    uint8_t thresholdMSB = 0;

    if(ReadReg( (uint8_t)PADS_THR_P_H_REG, 1, &thresholdMSB))
        return WE_FAIL;

    return thresholdMSB;
}

/**
* @brief  Disable/enable the I2C Interface
* @param  state
* @retval Error code
*/
int8_t  PADS_disI2C(PADS_state_t I2CStatus)
{
    PADS_interface_ctrl_t interface_ctrl;

    if (ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG , 1, (uint8_t *)&interface_ctrl))
        return WE_FAIL;

    interface_ctrl.disableI2C  = I2CStatus;

    if(WriteReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
        return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the I2C interface state [enabled;disabled]
* @param  no parameter.
* @retval I2C interface state
*/
uint8_t PADS_getI2CState()
{
    PADS_interface_ctrl_t interface_ctrl;

    if(ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
        return WE_FAIL;

    return (interface_ctrl.disableI2C);
}


/**
* @brief  Disable/enable the internal pull-down on Interrupt pin
* @param  state
* @retval Error code
*/
int8_t  PADS_disPullDownIntPin(PADS_state_t pullDownState)
{
    PADS_interface_ctrl_t interface_ctrl;
    if (ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG , 1, (uint8_t *)&interface_ctrl))
        return WE_FAIL;

    interface_ctrl.disPullDownOnIntPin  = pullDownState;

    if(WriteReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
        return WE_FAIL;

    return WE_SUCCESS;
  }

/**
* @brief  Read the state of the pull down on the interrupt pin [enabled;disabled]
* @param  no parameter.
* @retval state of the pull down on the interrupt pin
*/
uint8_t PADS_getPullDownIntPinState()
{
    PADS_interface_ctrl_t interface_ctrl;

    if(ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    return (interface_ctrl.disPullDownOnIntPin);
}


/**
* @brief  Set internal pull-up on the SAO pin
* @param  state
* @retval Error code
*/
int8_t PADS_setSAOPullUp(PADS_state_t SaoStatus)
{
    PADS_interface_ctrl_t interface_ctrl;
    if (ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG , 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    interface_ctrl.pullUpOnSAOpin = SaoStatus;

    if(WriteReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the state of the pull up on the SAO pin [enabled;disabled]
* @param  no parameter.
* @retval state of the pull up on the SAO pin
*/
uint8_t PADS_getSAOPullUpState()
{
    PADS_interface_ctrl_t interface_ctrl;

    if(ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    return (interface_ctrl.pullUpOnSAOpin);

}
/**
* @brief  Set internal pull-up on the SDA pin
* @param  state
* @retval Error code
*/
int8_t PADS_setSDAPullUp(PADS_state_t SdaStatus)
{
    PADS_interface_ctrl_t interface_ctrl;
    if (ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG , 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    interface_ctrl.pullUpOnSDApin  = SdaStatus;

    if(WriteReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the state of the pull up on the SDA pin [enabled;disabled]
* @param  no parameter.
* @retval state of the pull up on the SDA pin
*/
uint8_t PADS_getSDAPullUpState()
{
    PADS_interface_ctrl_t interface_ctrl;

    if(ReadReg((uint8_t)PADS_INTERFACE_CTRL_REG, 1, (uint8_t *)&interface_ctrl))
		return WE_FAIL;

    return (interface_ctrl.pullUpOnSDApin);
}

/**
* @brief  Set the Fifo mode
* @param  config
* @retval Error code
*/
int8_t  PADS_setFifoMode(PADS_fifo_mode_t fifoMode)
{
    PADS_fifo_ctrl_t fifo_ctrl_reg;

    if (ReadReg((uint8_t)PADS_FIFO_CTRL_REG , 1, (uint8_t *)&fifo_ctrl_reg))
		return WE_FAIL;

    fifo_ctrl_reg.fifoMode = fifoMode;

    if(WriteReg((uint8_t)PADS_FIFO_CTRL_REG, 1, (uint8_t *)&fifo_ctrl_reg))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the Fifo mode
* @param  no parameter.
* @retval Fifo mode Configuration
*/
uint8_t PADS_getFifoMode()
{
    PADS_fifo_ctrl_t fifo_ctrl_reg;

    if(ReadReg((uint8_t)PADS_FIFO_CTRL_REG, 1, (uint8_t *)&fifo_ctrl_reg))
		return WE_FAIL;

    return (fifo_ctrl_reg.fifoMode);
}


/**
* @brief  Set stop on user defined fifo threshold level
* @param  state
* @retval Error code
*/
int8_t PADS_setStopOnThr(PADS_state_t state)
{
    PADS_fifo_ctrl_t fifo_ctrl;

    if (ReadReg((uint8_t)PADS_FIFO_CTRL_REG , 1, (uint8_t *)&fifo_ctrl))
		return WE_FAIL;

    fifo_ctrl.stopOnThreshold = state;

    if(WriteReg((uint8_t)PADS_FIFO_CTRL_REG, 1, (uint8_t *)&fifo_ctrl))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the state of the StopOnThresholLevel State
* @param  no parameter.
* @retval state of the StopOnThresholLevel State
*/
uint8_t PADS_getStopOnThrStatus()
{
    PADS_fifo_ctrl_t fifo_ctrl;

    if(ReadReg((uint8_t)PADS_FIFO_CTRL_REG, 1, (uint8_t *)&fifo_ctrl))
		return WE_FAIL;

    return (fifo_ctrl.stopOnThreshold);
}


/**
* @brief  Set the fifo threshold level
* @param  level
* @retval Error code
*/
int8_t PADS_setFifoThr(uint8_t fifoWtm)
{
    PADS_fifo_wtm_t fifo_wtm;

    if (ReadReg((uint8_t)PADS_FIFO_WTM_REG , 1, (uint8_t *)&fifo_wtm))
		return WE_FAIL;

    fifo_wtm.fifoThreshold = fifoWtm;

    if(WriteReg((uint8_t)PADS_FIFO_WTM_REG, 1, (uint8_t *)&fifo_wtm))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the fifo threshold level
* @param  no parameter.
* @retval fifo threshold level
*/
uint8_t PADS_getFifoThr()
{
    PADS_fifo_wtm_t fifo_wtm;

    if(ReadReg((uint8_t)PADS_FIFO_WTM_REG, 1, (uint8_t *)&fifo_wtm))
		return WE_FAIL;

    return (fifo_wtm.fifoThreshold);
}

/**
* @brief  Read the fifo fill level
* @param  no parameter.
* @retval fifo filling level
*/
uint8_t PADS_getFifoFillLevel()
{
    uint8_t fifoLevel;

    if(ReadReg((uint8_t)PADS_FIFO_STATUS1_REG, 1, &fifoLevel))
		return WE_FAIL;

    return (fifoLevel);
}

/**
* @brief  Read the LSB of pressure reference value
* @param  no parameter.
* @retval LSB pressure refernce value
*/
uint8_t PADS_getPresRefLSB()
{
    uint8_t lowReferenceValue;

    if(ReadReg((uint8_t)PADS_REF_P_H_REG, 1, &lowReferenceValue))
		return WE_FAIL;

    return lowReferenceValue;
}

/**
* @brief  Read the MSB of pressure reference value
* @param  no parameter.
* @retval MSB pressure refernce value
*/
uint8_t PADS_getPresRefMSB()
{
    uint8_t highReferenceValue;

    if(ReadReg((uint8_t)PADS_REF_P_H_REG, 1, &highReferenceValue))
		return WE_FAIL;

    return highReferenceValue;
}


/**
* @brief  Set LSB part of the pressure offset value
* @param  a variable,its Type is uint8_t
* @retval Error code
*/
int8_t PADS_setPresOffsetLSB(uint8_t offSet)
{
    uint8_t pressureOffsetLsb = offSet;

    if(WriteReg((uint8_t)PADS_OPC_P_L_REG, 1, &pressureOffsetLsb))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the LSB part of the pressure offset value
* @param  no parameter.
* @retval LSB part of the pressure offset value
*/
uint8_t PADS_getPresOffsetLSB()
{
    uint8_t pressureOffsetLsb;

    if(ReadReg((uint8_t)PADS_OPC_P_L_REG, 1, &pressureOffsetLsb))
		return WE_FAIL;

    return (pressureOffsetLsb);
}

/**
* @brief  Set MSB part of the pressure offset value
* @param  a variable,its Type is uint8_t
* @retval Error code [Error,NoError]
*/
int8_t PADS_setPresOffsetMSB(uint8_t highPresureOffsetValue)
{
    uint8_t pressureOffsetMsb = highPresureOffsetValue;

    if(WriteReg((uint8_t)PADS_OPC_P_H_REG, 1, &pressureOffsetMsb))
		return WE_FAIL;

    return WE_SUCCESS;
}

/**
* @brief  Read the MSB part of the pressure offset value
* @param  no parameter.
* @retval MSB part of the pressure offset value
*/
uint8_t PADS_getPresOffsetMSB()
{
    uint8_t pressureOffsetMsb;

    if(ReadReg((uint8_t)PADS_OPC_P_H_REG, 1, &pressureOffsetMsb))
		return WE_FAIL;

    return (pressureOffsetMsb);
}

/**
* @brief Read the state of the interrupts [Not active , active]
* @param  no parameter.
* @retval state of the interrupt generation
*/
uint8_t PADS_getIntStatus()
{
    PADS_intSource_t int_source;

    if(ReadReg((uint8_t)PADS_INT_SOURCE_REG, 1, (uint8_t *)&int_source))
		return WE_FAIL;

    return (int_source.intStatus);
}

/**
* @brief  Read the state of the differntial low pressure interrupt [Not active , active]
* @param  no parameter.
* @retval state of the differential low pressure interrupt
*/
uint8_t PADS_getLowPresIntStatus()
{
    PADS_intSource_t int_source;

    if(ReadReg((uint8_t)PADS_INT_SOURCE_REG, 1, (uint8_t *)&int_source))
		return WE_FAIL;

    return (int_source.diffPresLowEvent);
}

/**
* @brief  Read the state of the differential  high pressure interrupt [Not active , active]
* @param  no parameter.
* @retval state of the differential high pressure interrup
*/
uint8_t PADS_getHighPresIntStatus()
{
    PADS_intSource_t int_source;

    if(ReadReg((uint8_t)PADS_INT_SOURCE_REG, 1, (uint8_t *)&int_source))
		return WE_FAIL;

    return (int_source.diffPresHighEvent);
}

/**
* @brief  Read the raw measured pressure value
* @param  no parameter.
* @retval Pressure Measurement
*/
int32_t PADS_getRAWPressure ()
{
    int32_t pressureValue = 0;
    uint8_t tmp[3] = {0};

    if(ReadReg((uint8_t)PADS_DATA_P_XL_REG, 3, tmp))
		return WE_FAIL;

    pressureValue = (int32_t)(tmp[2] << 16);
    pressureValue |= (int32_t)(tmp[1] << 8);
    pressureValue |= (int32_t)(tmp[0]);

    return pressureValue;
 }



/**
* @brief  Read the measured pressure value in kPa
* @param  no parameter.
* @retval Pressure Measurement
*/
float PADS_getPressure ()
{

    float pressureValue = (float)PADS_getRAWPressure();
    pressureValue = pressureValue/40960;
    return pressureValue;
 }


 /**
* @brief  Read the raw measured temperature value
* @param  no parameter.
* @retval Temperature Measurement
*/
int16_t PADS_getRAWTemperature()
{
    int16_t temperatureValue = 0;
    uint8_t tmp[2] = {0};

    if(ReadReg((uint8_t)PADS_DATA_T_L_REG, 2, tmp))
		return WE_FAIL;

    temperatureValue = (int16_t)(tmp[1] << 8);
    temperatureValue |= (int16_t) tmp[0];

    return temperatureValue;
}

/**
* @brief  Read the measured temperature value in °C
* @param  no parameter.
* @retval Temperature Measurement
*/
float PADS_getTemperature()
{

    float temperatureValue = (float)PADS_getRAWTemperature();
    temperatureValue = temperatureValue / 100 ;
    return temperatureValue;
}

/**
* @brief  Read the raw pressure value from Fifo
* @param  no parameter.
* @retval Fifo Pressure Measurement
*/
int32_t PADS_getFifoRAWPressure()
{
    int32_t pressureValue = 0;
    uint8_t tmp[3] = {0};

    if(ReadReg((uint8_t)PADS_FIFO_DATA_P_XL_REG,3, tmp))
		return WE_FAIL;

    pressureValue = (int32_t)(tmp[2] << 16);
    pressureValue |= (int32_t)(tmp[1] << 8);
    pressureValue |= (int32_t)(tmp[0]);

    return pressureValue;
 }
/**
* @brief  Read the pressure value from Fifo in kPa
* @param  no parameter.
* @retval Fifo Pressure Measurement
*/
float PADS_getFifoPressure()
{

    float pressureValue = (float)PADS_getFifoRAWPressure() ;
    pressureValue = pressureValue/40960 ;
    return pressureValue;
 }


/**
* @brief  Read the raw temperature value from fifo
* @param  no parameter.
* @retval Fifo Temperature Measurement
*/
int16_t PADS_getFifoRAWTemperature()
{
    uint16_t temperatureValue = 0;
    uint8_t tmp[2] = {0};

    if(ReadReg((uint8_t)PADS_FIFO_DATA_T_L_REG, 2, tmp))
		return WE_FAIL;

    temperatureValue = (int16_t)(tmp[1] << 8);
    temperatureValue |= (int16_t)tmp[0];

    return temperatureValue;
}

/**
* @brief  Read the temperature value from fifo in °C
* @param  no parameter.
* @retval Fifo Temperature Measurement
*/
float PADS_getFifoTemperature()
{

    float temperatureValue = (float)PADS_getFifoRAWTemperature() ;
    temperatureValue = temperatureValue / 100 ;

    return temperatureValue;
}

/**         EOF         */