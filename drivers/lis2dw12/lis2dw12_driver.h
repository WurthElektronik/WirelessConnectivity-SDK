/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIS2DW_DRIVER__H
#define __LIS2DW_DRIVER__H

#include <stdint.h>

#pragma message "Please choose the right interface USE_SPI or USE_I2C"
#define USE_SPI

#ifndef USE_I2C
#define USE_SPI
#endif

#if ((defined USE_I2C)&&(defined USE_SPI)) || ((!defined USE_I2C)&&(!defined USE_SPI))
#error "Define only one interface. Either SPI or I2C"
#endif

#include "../../drivers/WE-common.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/


//these could change accordingly with the architecture

#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES


#endif /*__ARCHDEP__TYPES*/

typedef uint8_t LIS2DW_IntPinConf_t;
typedef enum {
  LIS2DW_Axis_Z_Enable			=		0x20,
  LIS2DW_Axis_Y_Enable			=		0x40,
  LIS2DW_Axis_X_Enable			=		0x80,
} LIS2DW_Axis_t;
typedef enum {
  LIS2DW_AxisPriority_XYZ			=		0x00,
  LIS2DW_AxisPriority_YXZ			=		0x01,
  LIS2DW_AxisPriority_XZY			=		0x02,
  LIS2DW_AxisPriority_ZYX			=		0x03,
  LIS2DW_AxisPriority_XYZ2			=		0x04,
  LIS2DW_AxisPriority_YZX			=		0x05,
  LIS2DW_AxisPriority_ZXY			=		0x06,
  LIS2DW_AxisPriority_ZYX2			=		0x07,
} LIS2DW_AxisPriority_t;
typedef uint8_t LIS2DW_Int1Conf_t;


//define structure
#ifndef __SHARED__TYPES
#define __SHARED__TYPES

typedef enum {
  MEMS_SUCCESS				=		0x01,
  MEMS_ERROR				=		0x00
} status_t;

typedef enum {
  MEMS_ENABLE				=		0x01,
  MEMS_DISABLE				=		0x00
} State_t;

typedef struct {
  int16_t AXIS_X;
  int16_t AXIS_Y;
  int16_t AXIS_Z;
} AxesRaw_t;

#endif /*__SHARED__TYPES*/

typedef enum {
  LIS2DW_ODR_POWERDOWN          =       0x00,
  LIS2DW_ODR_12_5Hz_NP_1_6Hz_LP =		0x01,
  LIS2DW_ODR_12_5Hz_NP          =		0x02,
  LIS2DW_ODR_25Hz_NP		    =		0x03,
  LIS2DW_ODR_50Hz_NP		    =		0x04,
  LIS2DW_ODR_100Hz_NP		    =		0x05,
  LIS2DW_ODR_200Hz_NP		    =		0x06,
  LIS2DW_ODR_400Hz_NP_200Hz_LP	=		0x07,
  LIS2DW_ODR_800Hz_NP_200Hz_LP	=		0x08,
  LIS2DW_ODR_1600Hz_NP_200Hz_LP =		0x09
} LIS2DW_ODR_t;


typedef enum {
  LIS2DW_POWER_DOWN         =		0x00,
  LIS2DW_LOW_POWER 			=		0x01,
  LIS2DW_NORMAL			    =		0x02,
  LIS2DW_SINGLE_CONVERSION  =       0x03
} LIS2DW_Mode_t;

typedef enum {
  LIS2DW_LOW_POWER_1       =               0x00,
  LIS2DW_LOW_POWER_2       =               0x01,
  LIS2DW_LOW_POWER_3       =               0x02,
  LIS2DW_LOW_POWER_4       =               0x03
} LIS2DW_LowPowerMode_t;

typedef enum {
  LIS2DW_BW_FILT_ODR_2       =               0x00,
  LIS2DW_BW_FILT_ODR_4       =               0x01,
  LIS2DW_BW_FILT_ODR_10      = 		         0x02,
  LIS2DW_BW_FILT_ODR_20      =               0x03
} LIS2DW_BWFilt_t;


typedef enum {
  LIS2DW_FULLSCALE_2                   =               0x00,
  LIS2DW_FULLSCALE_4                   =               0x01,
  LIS2DW_FULLSCALE_8                   =               0x02,
  LIS2DW_FULLSCALE_16                  =               0x03
} LIS2DW_Fullscale_t;


typedef enum {
  LIS2DW_SELF_TEST_DISABLE             =               0x00,
  LIS2DW_SELF_TEST_POS                 =               0x01,
  LIS2DW_SELF_TEST_NEG                 =               0x02
} LIS2DW_SelfTest_t;


typedef enum {
  LIS2DW_SLP_MODE_SELECT_INT2          =               0x00,
  LIS2DW_SLP_MODE_SELECT_SPI           =               0x01
} LIS2DW_SLPModeSelect_t;

typedef enum {
  LIS2DW_FIFO_BYPASS_MODE              =               0x00,
  LIS2DW_FIFO_MODE                     =               0x01,
  LIS2DW_FIFO_STREAM_MODE              =               0x03,
  LIS2DW_FIFO_TRIGGER_MODE             =               0x04,
  LIS2DW_FIFO_CONTINUOUS_MODE          =               0x06
} LIS2DW_FifoMode_t;


typedef enum {
  LIS2DW_SPI_4_WIRE                    =               0x00,
  LIS2DW_SPI_3_WIRE                    =               0x01
} LIS2DW_SPIMode_t;


typedef enum {
  LIS2DW_Z_ENABLE                      =               0x20,
  LIS2DW_Z_DISABLE                     =               0x00,
  LIS2DW_Y_ENABLE                      =               0x40,
  LIS2DW_Y_DISABLE                     =               0x00,
  LIS2DW_X_ENABLE                      =               0x80,
  LIS2DW_X_DISABLE                     =               0x00
} LIS2DW_AXISenable_t;


/* Exported constants --------------------------------------------------------*/

#ifndef __SHARED__CONSTANTS
#define __SHARED__CONSTANTS

#define MEMS_SET                                        0x01
#define MEMS_RESET                                      0x00

#endif /*__SHARED__CONSTANTS*/


/**
 * Write to register
 */
#define LIS2DW_SPI_WRITE_MASK (0 << 7)
/**
 * The READ bit must be set when reading
 */
#define LIS2DW_SPI_READ_MASK (1 << 7)
/**
 * Multi byte transfers must assert this bit when writing the address.
 */
#define LIS2DW_SPI_MULTI_MASK (1 << 6)
/**
 * Opposite of LIS2DW_SPI_MULTI_MASK.
 */
#define LIS2DW_SPI_SINGLE_MASK (0 << 6)
/**
 * Mask of the address bits in the address byte during transfers.
 */
#define LIS2DW_SPI_ADDRESS_MASK (0x3F)

//Register Definition
#define LIS2DW_WHO_AM_I				0x0F  // device identification register

#define LIS2DW_WHO_AM_I_RESPONSE (0b01000100)

//STATUS REGISTER
#define LIS2DW_STATUS				    0x27
#define LIS2DW_STATUS_FIFO_THS		    BIT(7)
#define LIS2DW_STATUS_WU_IA			    BIT(6)
#define LIS2DW_STATUS_SLEEP_STATE	    BIT(5)
#define LIS2DW_STATUS_DOUBLE_TAP	    BIT(4)
#define LIS2DW_STATUS_SINGLE_TAP	    BIT(3)
#define LIS2DW_STATUS_6D_IA			    BIT(2)
#define LIS2DW_STATUS_FF_IA			    BIT(1)
#define LIS2DW_STATUS_DRDY			    BIT(0)


//CONTROL REGISTER 1
#define LIS2DW_CTRL1				    0x20
#define LIS2DW_CTRL1_ODR_BIT			BIT(4)
#define LIS2DW_CTRL1_MODE_BIT			BIT(2)
#define LIS2DW_CTRL1_LPM_BIT			BIT(0)

//CONTROL REGISTER 2
#define LIS2DW_CTRL2				0x21
#define LIS2DW_CTRL2_BOOT     		BIT(7)
#define LIS2DW_CTRL2_SOFTRESET		BIT(6)
#define LIS2DW_CTRL2_CS_PU_DISC		BIT(4)
#define LIS2DW_CTRL2_BDU			BIT(3)
#define LIS2DW_CTRL2_IF_ADD_INC		BIT(2)
#define LIS2DW_CTRL2_I2C_DISABLE	BIT(1)
#define LIS2DW_CTRL2_SIM			BIT(0)

//CONTROL REGISTER 3
#define LIS2DW_CTRL3				0x22
#define LIS2DW_CTRL3_ST				BIT(6)
#define LIS2DW_CTRL3_PP_OD			BIT(5)
#define LIS2DW_CTRL3_LIR			BIT(4)
#define LIS2DW_CTRL3_H_LACTIVE		BIT(3)
#define LIS2DW_CTRL3_SLP_MODE_SEL	BIT(0)

//CONTROL REGISTER 6
#define LIS2DW_CTRL6				0x25
#define LIS2DW_CTRL6_BW_FILT  		BIT(6)
#define LIS2DW_CTRL6_FS   			BIT(4)
#define LIS2DW_CTRL6_FDS           	BIT(3)
#define LIS2DW_CTRL6_LOW_NOISE		BIT(2)

//CONTROL REGISTER CTRL4_INT1_PAD_CTRL
#define LIS2DW_CTRL4_INT1_PAD_CTRL				    0x23
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_6D  		BIT(7)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_SINGLE_TAP  BIT(6)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_WU   		BIT(5)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_FF          BIT(4)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_TAP		    BIT(3)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_DIFF5		BIT(2)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_FTH		    BIT(1)
#define LIS2DW_CTRL4_INT1_PAD_CTRL_INT1_DRDY		BIT(0)

//CONTROL REGISTER CTRL5_INT2_PAD_CTRL
#define LIS2DW_CTRL5_INT2_PAD_CTRL 				    0x24
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_SLEEP_CHG  	BIT(6)
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_BOOT   		BIT(5)
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_DRDY_T      BIT(4)
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_OVR		    BIT(3)
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_DIFF5		BIT(2)
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_FTH		    BIT(1)
#define LIS2DW_CTRL5_INT2_PAD_CTRL_INT2_DRDY		BIT(0)

//OUT_T REGISTER
#define LIS2DW_OUT_T 0x26

//TEMPERATURE REGISTERS
#define LIS2DW_OUT_T_L  0x0D
#define LIS2DW_OUT_T_H  0x0E

//FIFO CONTROL REGISTER
#define LIS2DW_FIFO_CTRL                               0x2E
#define LIS2DW_FIFO_CTRL_FM                            BIT(5)
#define LIS2DW_FIFO_CTRL_FTH                           BIT(0)

//FIFO SAMPLES REGISTER
#define LIS2DW_FIFO_SAMPLES               0x2F
#define LIS2DW_FIFO_SAMPLES_FIFO_FTH      BIT(7)
#define LIS2DW_FIFO_SAMPLES_FIFO_OVR      BIT(6)
#define LIS2DW_FIFO_SAMPLES_DIFF          BIT(0)

//TAP_THS_X REGISTER, 4D configuration enable
#define LIS2DW_TAP_THS_X                  0x30
#define LIS2DW_TAP_THS_X_4D_EN            BIT(7)
#define LIS2DW_TAP_THS_X_6D_THS           BIT(5)
#define LIS2DW_TAP_THS_X_TAP_THSX         BIT(0)

//TAP_THS_Y REGISTER, axis priority
#define LIS2DW_TAP_THS_Y                    0x31
#define LIS2DW_TAP_THS_Y_TAP_PRIOR          BIT(5)
#define LIS2DW_TAP_THS_Y_TAP_THSY           BIT(0)

//TAP_THS_Z REGISTER, set axis
#define LIS2DW_TAP_THS_Z                      0x32
#define LIS2DW_TAP_THS_Z_TAP_X_EN             BIT(7)
#define LIS2DW_TAP_THS_Z_TAP_Y_EN             BIT(6)
#define LIS2DW_TAP_THS_Z_TAP_Z_EN             BIT(5)
#define LIS2DW_TAP_THS_Z_TAP_THSZ             BIT(0)

//INT_DUR REGISTER
#define LIS2DW_INT_DUR                      0x33
#define LIS2DW_INT_DUR_LATENCY              BIT(4)
#define LIS2DW_INT_DUR_QUIET                BIT(2)
#define LIS2DW_INT_DUR_SHOCK                BIT(0)

//WAKE_UP_THS REGISTER
#define LIS2DW_WAKE_UP_THS                      0x34
#define LIS2DW_WAKE_UP_THS_SINGLE_DOUBLE_TAP    BIT(7)
#define LIS2DW_WAKE_UP_THS_SLEEP_ON             BIT(6)
#define LIS2DW_WAKE_UP_THS_WK_THS               BIT(0)

//WAKE_UP_DUR REGISTER
#define LIS2DW_WAKE_UP_DUR                      0x35
#define LIS2DW_WAKE_UP_DUR_FF_DUR5              BIT(7)
#define LIS2DW_WAKE_UP_DUR_WAKE_DUR             BIT(5)
#define LIS2DW_WAKE_UP_DUR_SLEEP_DUR            BIT(0)

//WAKE_UP_DUR REGISTER
#define LIS2DW_FREE_FALL                      0x36
#define LIS2DW_FREE_FALL_FF_DUR               BIT(3)
#define LIS2DW_FREE_FALL_FF_THS               BIT(0)

//STATUS_DUP REGISTER
#define LIS2DW_STATUS_DUP                0x37
#define LIS2DW_STATUS_DUP_OVR   		 BIT(7)
#define LIS2DW_STATUS_DUP_DRDY_T		 BIT(6)
#define LIS2DW_STATUS_DUP_SLEEP_STATE_IA BIT(5)
#define LIS2DW_STATUS_DUP_DOUBLE_TAP	 BIT(4)
#define LIS2DW_STATUS_DUP_SINGLE_TAP	 BIT(3)
#define LIS2DW_STATUS_DUP_6D_IA		     BIT(2)
#define LIS2DW_STATUS_DUP_FF_IA		     BIT(1)
#define LIS2DW_STATUS_DUP_DRDY   		 BIT(0)

//WAKE_UP_SRC REGISTER
#define LIS2DW_WAKE_UP_SRC                0x38
#define LIS2DW_WAKE_UP_SRC_FF_IA         BIT(5)
#define LIS2DW_WAKE_UP_SRC_SLEEP_STATEIA BIT(4)
#define LIS2DW_WAKE_UP_SRC_WU_IA	      BIT(3)
#define LIS2DW_WAKE_UP_SRC_X_WU		  BIT(2)
#define LIS2DW_WAKE_UP_SRC_Y_WU		  BIT(1)
#define LIS2DW_WAKE_UP_SRC_Z_WU   		  BIT(0)

//TAP_SRC REGISTER
#define LIS2DW_TAP_SRC               0x39
#define LIS2DW_TAP_SRC_TAP_IA		 BIT(6)
#define LIS2DW_TAP_SRC_SINGLE_TAP    BIT(5)
#define LIS2DW_TAP_SRC_DOUBLE_TAP	 BIT(4)
#define LIS2DW_TAP_SRC_TAP_SIGN	     BIT(3)
#define LIS2DW_TAP_SRC_X_TAP		 BIT(2)
#define LIS2DW_TAP_SRC_Y_TAP		 BIT(1)
#define LIS2DW_TAP_SRC_Z_TAP   		 BIT(0)

//SIXD_SRC REGISTER
#define LIS2DW_SIXD_SRC              0x3A
#define LIS2DW_SIXD_SRC_6D_IA		 BIT(6)
#define LIS2DW_SIXD_SRC_ZH           BIT(5)
#define LIS2DW_SIXD_SRC_ZL	         BIT(4)
#define LIS2DW_SIXD_SRC_YH	         BIT(3)
#define LIS2DW_SIXD_SRC_YL		     BIT(2)
#define LIS2DW_SIXD_SRC_XH	         BIT(1)
#define LIS2DW_SIXD_SRC_XL   		 BIT(0)

//ALL_INT_SRC REGISTER
#define LIS2DW_ALL_INT_SRC                   0x3B
#define LIS2DW_ALL_INT_SRC_SLEEP_CHANGE_IA   BIT(5)
#define LIS2DW_ALL_INT_SRC_6D_IA	         BIT(4)
#define LIS2DW_ALL_INT_SRC_DOUBLE_TAP	     BIT(3)
#define LIS2DW_ALL_INT_SRC_SINGLE_TAP		 BIT(2)
#define LIS2DW_ALL_INT_SRC_WU_IA	         BIT(1)
#define LIS2DW_ALL_INT_SRC_FF_IA   		     BIT(0)

//ABS_INT_X REGISTER
#define LIS2DW_ABS_INT_X             0x3C
#define LIS2DW_ABS_INT_X_ABS_INT_X   BIT(0)

//ABS_INT_Y REGISTER
#define LIS2DW_ABS_INT_Y             0x3D
#define LIS2DW_ABS_INT_Y_ABS_INT_Y   BIT(0)

//ABS_INT_Z REGISTER
#define LIS2DW_ABS_INT_Z             0x3E
#define LIS2DW_ABS_INT_Z_ABS_INT_Z   BIT(0)

//ABS_INT_CFG REGISTER
#define LIS2DW_ABS_INT_CFG                     0x3F
#define LIS2DW_ABS_INT_CFG_DRDY_PULSED   	   BIT(7)
#define LIS2DW_ABS_INT_CFG_INT2_ON_INT1   	   BIT(6)
#define LIS2DW_ABS_INT_CFG_INTERRUPTS_ENABLE   BIT(5)
#define LIS2DW_ABS_INT_CFG_USR_OFF_ON_OUT	   BIT(4)
#define LIS2DW_ABS_INT_CFG_USR_OFF_ON_WU	   BIT(3)
#define LIS2DW_ABS_INT_CFG_USR_OFF_W		   BIT(2)
#define LIS2DW_ABS_INT_CFG_HP_REF_MODE	       BIT(1)
#define LIS2DW_ABS_INT_CFG_LPASS_ON6D   	   BIT(0)

//OUTPUT REGISTER
#define LIS2DW_OUT_X_L					0x28
#define LIS2DW_OUT_X_H					0x29
#define LIS2DW_OUT_Y_L					0x2A
#define LIS2DW_OUT_Y_H					0x2B
#define LIS2DW_OUT_Z_L					0x2C
#define LIS2DW_OUT_Z_H					0x2D

//Sensitivity
#define LIS2DW_Acc_Sensitivity_2g     1  /*!< accelerometer sensitivity with 2 g full scale [mg/LSB] */
#define LIS2DW_Acc_Sensitivity_4g     2  /*!< accelerometer sensitivity with 4 g full scale [mg/LSB] */
#define LIS2DW_Acc_Sensitivity_8g     4  /*!< accelerometer sensitivity with 8 g full scale [mg/LSB] */
#define LIS2DW_Acc_Sensitivity_16g    8  /*!< accelerometer sensitivity with 16 g full scale [mg/LSB] */

/* Exported macro ------------------------------------------------------------*/

#ifndef __SHARED__MACROS

#define __SHARED__MACROS
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#define BIT(x) ( (x) )

#endif /*__SHARED__MACROS*/

/* Exported functions --------------------------------------------------------*/
//Sensor Configuration Functions
status_t LIS2DW_SpiInit(int channel);
status_t LIS2DW_I2CInit(int addr);
status_t LIS2DW_SetODR(LIS2DW_ODR_t ov);
status_t LIS2DW_SetMode(LIS2DW_Mode_t md);
status_t LIS2DW_SetLPMode(LIS2DW_LowPowerMode_t lpm);
status_t LIS2DW_SetAxis(LIS2DW_Axis_t axis);
status_t LIS2DW_SetAxisPriority(LIS2DW_AxisPriority_t prio);
status_t LIS2DW_SetFullScale(LIS2DW_Fullscale_t fs);
status_t LIS2DW_SetBDU(State_t bdu);
status_t LIS2DW_SetSelfTest(LIS2DW_SelfTest_t st);
status_t LIS2DW_SetSLPModeSel(LIS2DW_SLPModeSelect_t ms);
status_t LIS2DW_SetSLPModeEnable();


//Filtering Functions
status_t LIS2DW_SetBWFilt(LIS2DW_BWFilt_t hpf);

//FIFO Functions
status_t LIS2DW_FIFOModeEnable(LIS2DW_FifoMode_t fm);

//Other Reading Functions
status_t LIS2DW_GetStatus(uint8_t* val);
status_t LIS2DW_GetStatusBit(uint8_t statusBIT, uint8_t* val);
status_t LIS2DW_GetAccAxesRaw(AxesRaw_t* buff);
status_t LIS2DW_ConvAccValue(AxesRaw_t* data);
status_t LIS2DW_GetTempRaw(int16_t* val);
status_t LIS2DW_ConvTempValue(int16_t* in, float *out);
status_t LIS2DW_GetWHO_AM_I(uint8_t* val);
status_t LIS2DW_GetDataAvailable(uint8_t* val);

#endif /* __LIS2DW_H */
