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
#include <time.h>
#include "string.h"

#include <sched.h>

#include "../../drivers/WE-common.h"
#include "global.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef _global_defined
#define _global_defined

/**************************************
 *     Static function declarations   *
 **************************************/

/**************************************
 *          Static variables          *
 **************************************/

/**************************************
 *         Global functions           *
 **************************************/

void Debug_out(char* str,bool success)
{
    if(success)
    {
        /* green OK */
        fprintf (stdout, COLOR_BOLDGREEN "OK   " COLOR_RESET);
    }
    else
    {
        /* red NOK */
        fprintf (stdout, COLOR_BOLDRED "NOK  " COLOR_RESET);
    }
    fprintf (stdout, str);
    fprintf (stdout,"\n");
}

int setThreadPrio(const int prio)
{
    struct sched_param sched ;
    memset(&sched, 0, sizeof(sched)) ;

    if(prio > sched_get_priority_max (SCHED_RR))
        sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
    else
        sched.sched_priority = prio ;

    return sched_setscheduler(0, SCHED_RR, &sched) ;
}

void delay(unsigned int sleepFor)
{
    struct timespec sleeper, dummy ;

    sleeper.tv_sec  = (time_t)(sleepFor / 1000) ;
    sleeper.tv_nsec = (long)(sleepFor % 1000) * 1000000 ;

    nanosleep(&sleeper, &dummy) ;
}

void delay_us(unsigned int sleepFor)
{
    struct timespec sleeper, dummy ;

    sleeper.tv_sec  = (time_t)(sleepFor / 1000000) ;
    sleeper.tv_nsec = (long)(sleepFor % 1000) * 1000 ;

    nanosleep(&sleeper, &dummy) ;
}


/*
 *Request the 3 byte driver version
 *
 *output:
 * -version: pointer to the 3 byte driver version
 *
 *return true if request succeeded
 *       false otherwise
 */
bool GetDriverVersion(uint8_t* version)
{
    uint8_t help[3] = WIRELESS_CONNECTIVITY_SDK_VERSION;
    memcpy(version,help,3);
    return true;
}

#endif // _global_defined
#ifdef __cplusplus
}
#endif
