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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <errno.h>
#include "string.h"

#include "../../drivers/WE-common.h"
#include "global.h"

#ifndef _global_wiringPi_pin_defined
#define _global_wiringPi_pin_defined
#endif

#ifdef _global_libgpiod_pin_defined
#error "Pin interface already defined"
#endif

/**************************************
 *         Static variables           *
 **************************************/
static bool wiringPiIsSetup = false;


/**************************************
 *         Global functions           *
 **************************************/

bool InitPin(int pin_number)
{
    if(!wiringPiIsSetup)
    {
        wiringPiSetup();
        wiringPiIsSetup = true;
    }

    return true;
}

bool DeinitPin(int pin_number)
{
    return true;
}


bool SetPin( int pin_number, SetPin_InputOutput_t inout, SetPin_Pull_t pull, SetPin_Out_t out)
{
    switch(inout)
    {
        case SetPin_InputOutput_Input:
        {
            switch(pull)
            {
                case SetPin_Pull_Up:
                {
                    pullUpDnControl(pin_number, PUD_UP);
                }
                break;

                case SetPin_Pull_Down:
                {
                    pullUpDnControl(pin_number, PUD_DOWN);
                }
                break;

                case SetPin_Pull_No:
                {
                    pullUpDnControl(pin_number, PUD_OFF);
                }
                break;

                default:
                {
                    return false;
                }
                break;
            }
            pinMode (pin_number, INPUT) ;
        }
        break;

        case SetPin_InputOutput_Output:
        {
            switch(out)
            {
                case SetPin_Out_Low:
                {
                    digitalWrite (pin_number, LOW) ;
                }
                break;

                case SetPin_Out_High:
                {
                    digitalWrite (pin_number, HIGH) ;
                }
                break;

                default:
                {
                    return false;
                }
                break;
            }
            pinMode (pin_number, OUTPUT) ;
            pullUpDnControl(pin_number, PUD_OFF);
        }
        break;

        default:
        {
            return false;
        }
        break;
    }

    return true;
}


SetPin_Out_t GetPinLevel(int pin_number)
{
    return digitalRead(pin_number);
}
