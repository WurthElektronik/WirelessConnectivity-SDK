
/* Includes ------------------------------------------------------------------*/
#include "lis2dw12_driver.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* tested with wiringPi version 2.38 */
#include <wiringPi.h>
#include <errno.h>

#include <wiringPiI2C.h>
#include <wiringPiSPI.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#define LIS2DW_CS_LOW()
#define LIS2DW_CS_HIGH()

#define DUMMY 0x00

static int spi_handle ;
static int spi_channel = 0;

static int i2c_handle = 0;

/*******************************************************************************
* Function Name		: LIS2DW_SpiInit
* Description		: Init function
* Input			: None
* Output		: None
* Return		: None
*******************************************************************************/
status_t LIS2DW_SpiInit(int channel)
{
#if (defined USE_SPI)
    int speed = 4000000; /* 4MHz */
    int spi_mode = 0;
    spi_channel = channel;

    if ((spi_handle = wiringPiSPISetupMode (spi_channel, speed, spi_mode)) < 0)
    {
        /* error */
        return MEMS_ERROR;
    }
    return MEMS_SUCCESS;
#else
    return MEMS_ERROR;
#endif
}



/*******************************************************************************
* Function Name		: LIS2DW_I2CInit
* Description		: Init function
* Input			: None
* Output		: None
* Return		: None
*******************************************************************************/
status_t LIS2DW_I2CInit(int addr)
{
#if (defined USE_I2C)

#if 0
    /* set the CS to high to enable the I2C communication of the LIS2DW */
    pinMode (LIS2DW_I2C_CSPIN, OUTPUT) ;
    pullUpDnControl(LIS2DW_I2C_CSPIN, PUD_OFF);
    digitalWrite (LIS2DW_I2C_CSPIN,  HIGH) ;
#endif

    if ((i2c_handle = wiringPiI2CSetup (addr)) < 0)
    {
        fprintf (stdout, "wiringPiI2CSetup Error\n");
        /* error */
        return MEMS_ERROR;
    }

    return MEMS_SUCCESS;
#else
    return MEMS_ERROR;
#endif
}


/*******************************************************************************
* Function Name		: LIS2DW_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions
* Input			: Register Address
* Output		: Data REad
* Return		: None
*******************************************************************************/
uint8_t LIS2DW_ReadReg(uint8_t Reg, uint8_t* Data)
{
#if (defined USE_SPI)
    uint8_t fulldata[2];

    /* apply read mask */
    Reg |= LIS2DW_SPI_READ_MASK;

    /* send two bytes, the REG and a dummy for reading the response */
    fulldata[0] = Reg;
    fulldata[1] = DUMMY;

    LIS2DW_CS_LOW();
    if (wiringPiSPIDataRW (spi_channel, fulldata, 2) == -1)
    {
        return MEMS_ERROR;
    }
    LIS2DW_CS_HIGH();

    /* read out the received response of the sensor */
    *Data = fulldata[1];

    return MEMS_SUCCESS;
#elif (defined USE_I2C)
    int ret = wiringPiI2CReadReg8(i2c_handle,Reg);
    if (ret == -1)
    {
        return MEMS_ERROR;
    }
    else
    {
        /* put value to array */
        *Data = (uint8_t)ret;
    }

    return MEMS_SUCCESS;
#else
    return MEMS_ERROR;
#endif
}


/*******************************************************************************
* Function Name		: LIS2DW_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: None
*******************************************************************************/
uint8_t LIS2DW_WriteReg(uint8_t Reg, uint8_t Data)
{
#if (defined USE_SPI)
   uint8_t fulldata[2];

    fulldata[0] = Reg;
    fulldata[1] = Data;

    LIS2DW_CS_LOW();
    /* send two bytes, the REG and the data */
    if (wiringPiSPIDataRW (spi_channel, fulldata, 2) == -1)
    {
        return MEMS_ERROR;
    }
    LIS2DW_CS_HIGH();

    return MEMS_SUCCESS;
#elif (defined USE_I2C)
    wiringPiI2CWriteReg8(i2c_handle, Reg, Data) ;
    return MEMS_SUCCESS;
#else
    return MEMS_ERROR;
#endif
}


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LIS2DW_GetWHO_AM_I
* Description    : Read identification code by WHO_AM_I register
* Input          : Char to empty by Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t LIS2DW_GetWHO_AM_I(uint8_t* val)
{

    if( !LIS2DW_ReadReg(LIS2DW_WHO_AM_I, val) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_GetStatus
* Description    : Read the status register
* Input          : Char to empty by status register buffer
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_GetStatus(uint8_t* val)
{

    if( !LIS2DW_ReadReg(LIS2DW_STATUS, val) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}



/*******************************************************************************
* Function Name  : LIS2DW_GetStatusBIT
* Description    : Read the status register BIT
* Input          : LIS2DW_STATUS_*
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_GetStatusBit(uint8_t statusBIT, uint8_t* val)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_STATUS, &value) )
        return MEMS_ERROR;

    if(statusBIT == LIS2DW_STATUS_FIFO_THS)
    {
        if(value &= LIS2DW_STATUS_FIFO_THS)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_WU_IA)
    {
        if(value &= LIS2DW_STATUS_WU_IA)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_SLEEP_STATE)
    {
        if(value &= LIS2DW_STATUS_SLEEP_STATE)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_DOUBLE_TAP)
    {
        if(value &= LIS2DW_STATUS_DOUBLE_TAP)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_SINGLE_TAP)
    {
        if(value &= LIS2DW_STATUS_SINGLE_TAP)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_6D_IA)
    {
        if(value &= LIS2DW_STATUS_6D_IA)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_FF_IA)
    {
        if(value &= LIS2DW_STATUS_FF_IA)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }

    if(statusBIT == LIS2DW_STATUS_DRDY)
    {
        if(value &= LIS2DW_STATUS_DRDY)
        {
            *val = MEMS_SET;
            return MEMS_SUCCESS;
        }
        else
        {
            *val = MEMS_RESET;
            return MEMS_SUCCESS;
        }
    }
    return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetODR
* Description    : Sets LIS2DW Output Data Rate
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetODR(LIS2DW_ODR_t ov)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL1, &value) )
        return MEMS_ERROR;

    value &= 0x0f;
    value |= ov<<LIS2DW_CTRL1_ODR_BIT;

    if( !LIS2DW_WriteReg(LIS2DW_CTRL1, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_GetTempRaw
* Description    : Read the Temperature Values Output Registers
* Input          : Buffer to empty
* Output         : Temperature Values Registers buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_GetTempRaw(int16_t* buff)
{
    int16_t value;
    uint8_t *valueL = (uint8_t *)(&value);
    uint8_t *valueH = ((uint8_t *)(&value)+1);

    if( !LIS2DW_ReadReg(LIS2DW_OUT_T_L, valueL) )
        return MEMS_ERROR;

    if( !LIS2DW_ReadReg(LIS2DW_OUT_T_H, valueH) )
        return MEMS_ERROR;

    *buff = (int16_t)( value );

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_ConvTempValue
* Description    :
* Input          : raw data to be converted
* Output         : converted data with value in °C units
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_ConvTempValue(int16_t* in, float *out)
{
    *out = (*in >> 4);      /* shift by 4 due to the register setup */
    *out  = (*out/16) + 25; /* 16LSB/°C, the raw value of 0 means 25°C  */

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetMode
* Description    : Sets LIS2DW Operating Mode
* Input          : Modality (LIS2DW_NORMAL, LIS2DW_LOW_POWER, LIS2DW_POWER_DOWN)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetMode(LIS2DW_Mode_t md)
{
    uint8_t value;
    static   uint8_t ODR_old_value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL1, &value) )
        return MEMS_ERROR;

    if((value & 0xF0)==0)
        value = value | (ODR_old_value & 0xF0); //if it comes from POWERDOWN

    switch(md)
    {

    case LIS2DW_POWER_DOWN:
        ODR_old_value = value;
        value &= 0x0F;
        break;

    case LIS2DW_NORMAL:
        value &= 0xF3;
        value |= 0x01 << LIS2DW_CTRL1_MODE_BIT;//high performance mode
        break;

    case LIS2DW_SINGLE_CONVERSION:
        value &= 0xF3;
        value |=  0x02 << LIS2DW_CTRL1_MODE_BIT;//single conversion mode
        break;

    case LIS2DW_LOW_POWER:
        value &= 0xF3;
        value |=  0x00 << LIS2DW_CTRL1_MODE_BIT;//low power mode
        break;

    default:
        return MEMS_ERROR;
    }

    if( !LIS2DW_WriteReg(LIS2DW_CTRL1, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_SetLPMode
* Description    : Sets LIS2DW Low Power Mode
* Input          : Modality (LIS2DW_LOW_POWER_1,LIS2DW_LOW_POWER_2,LIS2DW_LOW_POWER_3,LIS2DW_LOW_POWER_4)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetLPMode(LIS2DW_LowPowerMode_t lpm)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL1, &value) )
        return MEMS_ERROR;

    value &= 0xFC;
    value |= lpm;

    if( !LIS2DW_WriteReg(LIS2DW_CTRL1, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetFullScale
* Description    : Sets the LIS2DW FullScale
* Input          : LIS2DW_FULLSCALE_2/LIS2DW_FULLSCALE_4/LIS2DW_FULLSCALE_8/LIS2DW_FULLSCALE_16
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetFullScale(LIS2DW_Fullscale_t fs)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL6, &value) )
        return MEMS_ERROR;

    value &= 0xCF;
    value |= (fs<<LIS2DW_CTRL6_FS);

    if( !LIS2DW_WriteReg(LIS2DW_CTRL6, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetBDU(State_t bdu)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL2, &value) )
        return MEMS_ERROR;

    value &= 0xF7;
    value |= (bdu<<LIS2DW_CTRL2_BDU);

    if( !LIS2DW_WriteReg(LIS2DW_CTRL2, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetSelfTest
* Description    : Set Self Test Modality
* Input          : LIS2DW_SELF_TEST_DISABLE/ST_0/ST_1
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetSelfTest(LIS2DW_SelfTest_t st)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL3, &value) )
        return MEMS_ERROR;

    value &= 0x3F;
    value |= (st<<LIS2DW_CTRL3_ST);

    if( !LIS2DW_WriteReg(LIS2DW_CTRL3, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetSLPModeSel
* Description    : Set SLP Mode Select
* Input          :
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetSLPModeSel(LIS2DW_SLPModeSelect_t ms)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL3, &value) )
        return MEMS_ERROR;

    value &= 0xFD;
    value |= (ms<<LIS2DW_CTRL3_SLP_MODE_SEL);

    if( !LIS2DW_WriteReg(LIS2DW_CTRL3, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_SetSLPModeEnable
* Description    : Enable the SLP Mode
* Input          :
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetSLPModeEnable()
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL3, &value) )
        return MEMS_ERROR;

    value &= 0xFE;
    value |= 0x01;

    if( !LIS2DW_WriteReg(LIS2DW_CTRL3, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_SetBWFilt
* Description    : Set High Pass CUT OFF Freq
* Input          : HPFCF [0,3]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetBWFilt(LIS2DW_BWFilt_t hpf)
{
    uint8_t value;

    if (hpf > 3)
        return MEMS_ERROR;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL6, &value) )
        return MEMS_ERROR;

    value &= 0x3F;
    value |= (hpf<<LIS2DW_CTRL6_BW_FILT);

    if( !LIS2DW_WriteReg(LIS2DW_CTRL6, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;

}


/*******************************************************************************
* Function Name  : LIS2DW_SetAxis
* Description    : Enable/Disable LIS2DW Axis
* Input          : LIS2DW_TAP_THS_Z_*
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetAxis(LIS2DW_Axis_t axis)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_TAP_THS_Z, &value) )
        return MEMS_ERROR;
    value &= 0x1F;
    value |= (0xE0 & axis);

    if( !LIS2DW_WriteReg(LIS2DW_TAP_THS_Z, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_SetAxisPriority
* Description    : Set LIS2DW Axis Priority
* Input          : LIS2DW_AxisPriority_t
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetAxisPriority(LIS2DW_AxisPriority_t prio)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_TAP_THS_Y, &value) )
        return MEMS_ERROR;
    value &= 0x1F;
    value |= (0xE0 & (prio << LIS2DW_TAP_THS_Y_TAP_PRIOR));

    if( !LIS2DW_WriteReg(LIS2DW_TAP_THS_Y, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_FIFOModeEnable
* Description    : Sets Fifo Modality
* Input          : LIS2DW_FIFO_DISABLE, LIS2DW_FIFO_BYPASS_MODE, LIS2DW_FIFO_MODE,
				   LIS2DW_FIFO_STREAM_MODE, LIS2DW_FIFO_TRIGGER_MODE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_FIFOModeEnable(LIS2DW_FifoMode_t fm)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_FIFO_CTRL, &value) )
        return MEMS_ERROR;

    value &= 0x1f;
    value |= (fm<<LIS2DW_FIFO_CTRL_FM);                     //fifo mode configuration

    if( !LIS2DW_WriteReg(LIS2DW_FIFO_CTRL, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_FIFOSamples
* Description    : Read the FIFO samples register
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_FIFOSamples(uint8_t *unread_samples, State_t *threshold_reached, State_t *overrun)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_FIFO_SAMPLES, &value) )
        return MEMS_ERROR;

    *unread_samples = (value & 0x3F) >> LIS2DW_FIFO_SAMPLES_DIFF;
    *threshold_reached = (State_t)((value & 0x80) >> LIS2DW_FIFO_SAMPLES_FIFO_FTH);
    *overrun = (State_t)((value & 0x40) >> LIS2DW_FIFO_SAMPLES_FIFO_OVR);

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_GetDataAvailable
* Description    : Reads bits indicating if new data is available
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_GetDataAvailable(uint8_t* val)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL3, &value) )
        return MEMS_ERROR;

    value &= 0x01;
    *val = value;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_GetAccAxesRaw
* Description    : Read the Acceleration Values Output Registers
* Input          : buffer to empity by AxesRaw_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_GetAccAxesRaw(AxesRaw_t* buff)
{
    int16_t value;
    uint8_t *valueL = (uint8_t *)(&value);
    uint8_t *valueH = ((uint8_t *)(&value)+1);

    if( !LIS2DW_ReadReg(LIS2DW_OUT_X_L, valueL) )
        return MEMS_ERROR;

    if( !LIS2DW_ReadReg(LIS2DW_OUT_X_H, valueH) )
        return MEMS_ERROR;

    buff->AXIS_X = value;

    if( !LIS2DW_ReadReg(LIS2DW_OUT_Y_L, valueL) )
        return MEMS_ERROR;

    if( !LIS2DW_ReadReg(LIS2DW_OUT_Y_H, valueH) )
        return MEMS_ERROR;

    buff->AXIS_Y = value;

    if( !LIS2DW_ReadReg(LIS2DW_OUT_Z_L, valueL) )
        return MEMS_ERROR;

    if( !LIS2DW_ReadReg(LIS2DW_OUT_Z_H, valueH) )
        return MEMS_ERROR;

    buff->AXIS_Z = value;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DW_ConvAccValue
* Description    : Convert the raw acceleration data to value in mg units
* Input          : raw data to be converted
* Output         : converted data with value in mg units
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_ConvAccValue(AxesRaw_t* data)
{
    uint8_t crtl6;
    uint8_t sensitivity;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL6, &crtl6) )
        return MEMS_ERROR;

    /* switch the sensitivity value set in the CTRL6 */
    switch(crtl6 & 0x30)
    {
    case 0x00:
        sensitivity = LIS2DW_Acc_Sensitivity_2g;
        break;
    case 0x10:
        sensitivity = LIS2DW_Acc_Sensitivity_4g;
        break;
    case 0x20:
        sensitivity = LIS2DW_Acc_Sensitivity_8g;
        break;
    case 0x30:
        sensitivity = LIS2DW_Acc_Sensitivity_16g;
        break;
    }

    data->AXIS_X = (data->AXIS_X >> 4)*sensitivity;
    data->AXIS_Y = (data->AXIS_Y >> 4)*sensitivity;
    data->AXIS_Z = (data->AXIS_Z >> 4)*sensitivity;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_Boot
* Description    : Boot enables retrieving the correct trimming parameters from nonvolatile memory into registers where trimming parameters are stored.
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_Boot()
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL2, &value) )
        return MEMS_ERROR;

    value &= 0x7F;
    value |= 0x01<<LIS2DW_CTRL2_BOOT;

    if( !LIS2DW_WriteReg(LIS2DW_CTRL2, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SoftReset
* Description    : Soft reset acts as reset for all control registers, then goes to 0.
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SoftReset()
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL2, &value) )
        return MEMS_ERROR;

    value &= 0xBF;
    value |= 0x01<<LIS2DW_CTRL2_SOFTRESET;

    if( !LIS2DW_WriteReg(LIS2DW_CTRL2, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS2DW_SetSPIInterface
* Description    : Set SPI mode: 3 Wire Interface OR 4 Wire Interface
* Input          : LIS2DW_SPI_3_WIRE, LIS2DW_SPI_4_WIRE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DW_SetSPIInterface(LIS2DW_SPIMode_t spi)
{
    uint8_t value;

    if( !LIS2DW_ReadReg(LIS2DW_CTRL2, &value) )
        return MEMS_ERROR;

    value &= 0xFE;
    value |= spi<<LIS2DW_CTRL2_SIM;

    if( !LIS2DW_WriteReg(LIS2DW_CTRL2, value) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}
