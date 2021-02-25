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
 * COPYRIGHT (c) 2021 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <gpiod.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../drivers/WE-common.h"
#include "global.h"

#ifndef _global_libgpiod_pin_defined
#define _global_libgpiod_pin_defined
#endif

#ifdef _global_wiringPi_pin_defined
#error "Pin interface already defined"
#endif

/* tested with libgpiod v1.2 (without bias support) and v1.6.2 (with bias support) */

/* libgpiod supports bias (pull-up, pull-down, no-pull and as-is)
 * starting with version 1.5
 * current version in debian stable buster is only 1.2; debian testing has v1.6.x already.
 *
 * If libgpiod v1.5 or higher is installed and to be used, uncomment the #define or
 * set it in the build procecess to enable pull-up/-down.
 */
#define LIBGPIO_V1_5
#define MAX_USED_PINS 6

typedef struct Pin_Config
{
    int Pin_number;
    struct gpiod_line *Pin_line;
} Pin_Config;

/**************************************
 *         Static variables           *
 **************************************/
static struct gpiod_chip *chip = NULL;

static bool Pin_openGPIO();
static bool Pin_closeGPIO();
static struct gpiod_line* Pin_getLine(int pinNumber);
static bool Pin_requestLine(int pinNumber);
static bool Pin_releaseLine(int pinNumber);

static int amountPinsInitialized = 0;
static Pin_Config pinConfiguration [MAX_USED_PINS];

static bool Pin_openGPIO()
{
    if(NULL == chip)
    {
        chip = gpiod_chip_open("/dev/gpiochip0");
        if(NULL == chip)
        {
            Debug_out("Could not open gpio device", false);
            return false;
        }

        for(int i = 0; i < MAX_USED_PINS; i++)
        {
            pinConfiguration[i].Pin_number = -1;
            pinConfiguration[i].Pin_line = NULL;
        }

        printf("libgpiod version: %s\n", gpiod_version_string());
    }

    return true;
}

static bool Pin_closeGPIO()
{
    if(NULL != chip)
    {

        for(int i = 0; i < MAX_USED_PINS; i++)
        {
            Pin_releaseLine(pinConfiguration[i].Pin_number);
        }

        /* This will also free all the lines internally allocated */
        gpiod_chip_close(chip);
        chip = NULL;
    }
    return true;
}

static struct gpiod_line* Pin_getLine(int pinNumber)
{
    for(int i = 0; i < amountPinsInitialized; i++)
    {
        if(pinConfiguration[i].Pin_number == pinNumber)
        {
            return pinConfiguration[i].Pin_line;
        }
    }

    return NULL;
}

static bool Pin_requestLine(int pinNumber)
{

    if(NULL == chip)
    {
        Debug_out("Pin_requestLine: No gpio device open", false);
        return false;
    }

    if(NULL != Pin_getLine(pinNumber))
    {
        /* Pin already requested, that's ok */
        return true;
    }

    struct gpiod_line_request_config gpio_config;
    memset(&gpio_config,0, sizeof(gpio_config));
    gpio_config.consumer = "WE_wicon";
    gpio_config.request_type = GPIOD_LINE_REQUEST_DIRECTION_INPUT;


    for(int i = 0; i < MAX_USED_PINS; i++)
    {
        /* found empty space for new pin  */
        if(pinConfiguration[i].Pin_number == -1)
        {
            pinConfiguration[i].Pin_line = gpiod_chip_get_line(chip, pinNumber);
            if(NULL == pinConfiguration[i].Pin_line)
            {
                Debug_out("Pin_requestLine: Could not get gpio line", false);
                printf("error when requesting pin %i: %s\n", pinNumber, strerror(errno));
                return false;
            }

            /* Request as input - out will be ignored */
            int rv = gpiod_line_request(pinConfiguration[i].Pin_line, &gpio_config, SetPin_Out_Low);
            if(-1 == rv)
            {
                Debug_out("Pin_requestLine: Could not request gpio line", false);
                printf("error when requesting pin %i: %s\n", pinNumber, strerror(errno));
                pinConfiguration[i].Pin_line = NULL;
                return false;
            }

            pinConfiguration[i].Pin_number = pinNumber;
            return true;
        }

    }

    Debug_out("Pin_requestLine: Pin configuration could not be found", false);
    return false;
}

static bool Pin_releaseLine(int pinNumber)
{
    if(NULL == chip)
    {
        Debug_out("Pin_releaseLine: No gpio device open", false);
        return false;
    }

    if(-1 == pinNumber)
    {
        /* pinNumber is not a valid number so just assume it's released.
         * This will happen when pins are already deinitilazed/never has been initialized, but the close function is called again.
         */
        return true;
    }

    for(int i = 0; i < MAX_USED_PINS; i++)
    {
        if(pinConfiguration[i].Pin_number == pinNumber)
        {
            if(NULL == pinConfiguration[i].Pin_line)
            {
                Debug_out("Pin_releaseLine: Pin is currently not held", false);
                pinConfiguration[i].Pin_number = -1;
                return false;
            }
            gpiod_line_release(pinConfiguration[i].Pin_line);

            pinConfiguration[i].Pin_line = NULL;
            pinConfiguration[i].Pin_number = -1;

            return true;
        }
    }

    Debug_out("Pin_releaseLine: Pin configuration could not be found", false);
    return false;
}

/**************************************
 *         Global functions           *
 **************************************/

bool InitPin(int pin_number)
{

    if(MAX_USED_PINS <= amountPinsInitialized)
    {
        Debug_out("Max used pins already in use", false);
        return false;
    }

    if(Pin_openGPIO())
    {
        if(false == Pin_requestLine(pin_number))
        {
            return false;
        }

        amountPinsInitialized++;
    }
    else
    {
        return false;
    }

    return true;
}

bool DeinitPin(int pin_number)
{
    if(0 < amountPinsInitialized)
    {
        if(false == Pin_releaseLine(pin_number))
        {
            return false;
        }
        amountPinsInitialized--;
    }

    if(0 == amountPinsInitialized )
    {
        return Pin_closeGPIO();
    }

    return true;
}


bool SetPin(int pin_number, SetPin_InputOutput_t inout, SetPin_Pull_t pull, SetPin_Out_t out)
{

    if(NULL == chip)
    {
        Debug_out("SetPin: No gpio device open", false);
        return false;
    }

    struct gpiod_line_request_config gpio_config;
    memset(&gpio_config,0, sizeof(gpio_config));
    gpio_config.consumer = "WE_wicon";
    switch(inout)
    {
        case SetPin_InputOutput_Input:
        {
            gpio_config.request_type = GPIOD_LINE_REQUEST_DIRECTION_INPUT;
            break;
        }
        case SetPin_InputOutput_Output:
        {
            gpio_config.request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
            break;
        }
        default:
        {
            return false;
        }
    }


    switch(pull)
    {
        case SetPin_Pull_No:
        {
#ifdef LIBGPIO_V1_5
            gpio_config.flags = GPIOD_LINE_REQUEST_FLAG_BIAS_DISABLE;
#endif
            break;
        }

        case SetPin_Pull_Up:
        {
#ifdef LIBGPIO_V1_5
            gpio_config.flags = GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP;
#endif
            break;
        }


        case SetPin_Pull_Down:
        {
#ifdef LIBGPIO_V1_5
            gpio_config.flags = GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN;
#endif
            break;
        }

        default:
        {
            return false;
            break;
        }
    }


    struct gpiod_line *line = Pin_getLine(pin_number);
    if(NULL == line)
    {
        return false;
    }

/* When LIBGPIOD is v1.5 or higher, config can be updated without releasing the line.
 * For versions below, release the line and request again with new configuration */
#ifdef LIBGPIO_V1_5
    int rv = gpiod_line_set_config(line, gpio_config.request_type, gpio_config.flags, out);

    if(-1 == rv)
    {
        Debug_out("SetPin: Could not update gpio line", false);
        printf("error when setting pin %i: %s\n", pin_number, strerror(errno));
        return false;
    }
#else
    gpiod_line_release(line);

    int rv = gpiod_line_request(line, &gpio_config, out);
    if(-1 == rv)
    {
        Debug_out("SetPin: Could not request gpio line", false);
        printf("error when setting pin %i: %s\n", pin_number, strerror(errno));
        return false;
    }
#endif
    return true;
}

SetPin_Out_t GetPinLevel(int pin_number)
{
    if(NULL == chip)
    {
        Debug_out("GetPin: No gpio device open", false);
        printf("error when getting pin %i\n", pin_number);
        return -1;
    }

    struct gpiod_line *line = Pin_getLine(pin_number);
     if(NULL == line)
    {
        return false;
    }

    int value = gpiod_line_get_value(line);
    if(-1 == value)
    {
        Debug_out("GetPin: Could not read pin level", false);
        printf("error when getting pin %i: %s\n", pin_number,strerror(errno));
    }

    return value;
}

