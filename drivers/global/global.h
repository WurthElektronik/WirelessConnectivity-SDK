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


#include <stdbool.h>
#include <stdint.h>

#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

typedef enum SetPin_InputOutput_t {
    SetPin_InputOutput_Input   = (uint8_t) 0,
    SetPin_InputOutput_Output  = (uint8_t) 1,
} SetPin_InputOutput_t;

typedef enum SetPin_Pull_t {
    SetPin_Pull_No   = (uint8_t) 0,
    SetPin_Pull_Up   = (uint8_t) 1,
    SetPin_Pull_Down = (uint8_t) 2,
} SetPin_Pull_t;

typedef enum SetPin_Out_t {
    SetPin_Out_Low  = (uint8_t) 0,
    SetPin_Out_High = (uint8_t) 1,
} SetPin_Out_t;

typedef enum Serial_ParityBit_t
{
    Serial_ParityBit_NONE = 0,
    Serial_ParityBit_EVEN,
    Serial_ParityBit_ODD,
}Serial_ParityBit_t;

typedef enum Serial_Mode_t
{
    Serial_Mode_0,
    Serial_Mode_1,
    Serial_Mode_2,
    Serial_Mode_3,
}Serial_Mode_t;

/*
 *Request the 3 byte driver version
 *
 *output:
 * -version: pointer to the 3 byte driver version
 *
 *return true if request succeeded
 *       false otherwise
 */
extern bool GetDriverVersion(uint8_t* version);


/* Switch pin to input/output high/low with/without pullup/pulldown
 * input
 * - pin_number: number of pin
 * - inout: input or output
 * - pull: pullup, pulldown or no pull
 * - out: output level high or low
 *
 * return: true, if success
 *        false, otherwise
*/
extern bool SetPin( int pin_number, SetPin_InputOutput_t inout, SetPin_Pull_t pull, SetPin_Out_t out);
/*Read out the pin level
 *input:
 *- pin_number: number of pin
 *
 * return: curren pin level of pin
 */
extern SetPin_Out_t GetPinLevel(int pin_number);

/*
 * Print the string
 *
 * input:
 * - str: String to print
 * - success: Variable indicating if action was ok
 */

void Debug_out(char* str,bool success);

/*
 * Sleepfunction for delay
 *
 * input:
 * - sleepFor: Time to delay in ms
 */
extern void delay(unsigned int sleepFor);


/*
 * Sleepfunction for delay
 *
 * input:
 * - sleepFor: Time to delay in micro seconds
 */
extern void delay_us(unsigned int sleepFor);

/*
 * Sets the priority for scheduling
 *
 *Priorities are not absolute but relative to other running threads
 * Thread with higher priorities are more likely to run
 *
 * input:
 * - prio: priority the thread is assigned to. Value can range from 1 to 99
 * return: 0, if success
 *        -1, otherwise
 *
 */
extern int setThreadPrio(const int prio);

/*
 * Initialize a pin
 *
 * input:
 * - pin_number: pin number
 *
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool InitPin(int pin_number);

/*
 * Deinitialize a pin
 *
 * input:
 * - pin_number: pin number
 *
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool DeinitPin(int pin_number);

/*
 * Initialize the serial interface
 *
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool InitSerial();

/*
 * Deinitialize the serial interface
 *
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool DeinitSerial();

/*
 * Open the serial interface
 *
 * input:
 * - baudrate: baudrate of the interface
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool OpenSerial(int baudrate);

/*
 * Open the SPI interface
 *
 * input:
 * - baudrate: baudrate of the interface
 * - mode    : mode of interface
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool OpenSPI(int datarate, Serial_Mode_t mode);

/*
 * Open the serial interface
 *
 * input:
 * - baudrate: baudrate of the interface
 * - parityBit: parity bit of interface
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool OpenSerialWithParity(int baudrate, Serial_ParityBit_t parityBit);

/*
 * Close the serial interface
 *
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool CloseSerial();

/*
 * Flush the serial interface
 *
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool FlushSerial();

/*
 * Read one byte from serial interface
 *
 * output:
 * - readBufferP: pointer to the buffer
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool ReadByte(uint8_t *readBufferP);

/*
 * Read N bytes from serial interface
 *
 * input:
 * - length     : length of the data to read
 *
 * output:
 * - dataP: pointer to the buffer
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool ReadBytesN(uint8_t *dataP, uint16_t length);

/**
 * Check if bytes are available in the serial interface
 *
 * return: true, if bytes are available
 *         false, otherwise
 *
 */

extern bool BytesAvailable();

/*
 * Send bytes via serial interface
 *
 * input:
 * - dataP: pointer to the buffer
 * -length: length of the data to transmit
 * return: true, if success
 *         false, otherwise
 *
 */
extern bool SendBytes(uint8_t* dataP, uint16_t length);

#endif /* GLOBAL_H_INCLUDED */
