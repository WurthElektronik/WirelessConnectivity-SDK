/**
 ***************************************************************************************************
 * This file is part of Sensors SDK:
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

#ifndef _WE_PLATTFORM_H
#define _WE_PLATTFORM_H

/**         Includes         */

#include <stdint.h>
#include <stdbool.h>

/* colors */
#define COLOR_RESET   		"\033[0m"
#define COLOR_BLACK   		"\033[30m"      /* Black */
#define COLOR_RED     		"\033[31m"      /* Red */
#define COLOR_GREEN   		"\033[32m"      /* Green */
#define COLOR_YELLOW  		"\033[33m"      /* Yellow */
#define COLOR_BLUE    		"\033[34m"      /* Blue */
#define COLOR_MAGENTA 		"\033[35m"      /* Magenta */
#define COLOR_CYAN    		"\033[36m"      /* Cyan */
#define COLOR_WHITE   		"\033[37m"      /* White */
#define COLOR_BOLDBLACK   	"\033[1m\033[30m"      /* Bold Black */
#define COLOR_BOLDRED     	"\033[1m\033[31m"      /* Bold Red */
#define COLOR_BOLDGREEN   	"\033[1m\033[32m"      /* Bold Green */
#define COLOR_BOLDYELLOW  	"\033[1m\033[33m"      /* Bold Yellow */
#define COLOR_BOLDBLUE    	"\033[1m\033[34m"      /* Bold Blue */
#define COLOR_BOLDMAGENTA 	"\033[1m\033[35m"      /* Bold Magenta */
#define COLOR_BOLDCYAN    	"\033[1m\033[36m"      /* Bold Cyan */
#define COLOR_BOLDWHITE   	"\033[1m\033[37m"      /* Bold White */

#define WE_SUCCESS  0
#define WE_FAIL     1


/**         Functions definition         */

int8_t I2CInit(int address);
int8_t ReadReg(uint8_t RegAdr, int NumByteToRead, uint8_t *Data);
int8_t WriteReg(int RegAdr, int NumByteToWrite, uint8_t *Data);

void Debug_out(char *str, bool status);

extern void delay(unsigned int sleepForMs);

#endif /* _WE_PLATTFORM_H */
/**         EOF         */