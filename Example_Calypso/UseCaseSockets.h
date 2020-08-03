#ifndef _AT_USECASESOCKET_DEFINED
#define _AT_USECASESOCKET_DEFINED

/* ###########################################################################
* Includes:
*/

#include <stdbool.h>
#include <stdint.h>
#include "../drivers/Calypso/Calypso.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/


/*
* Constant Macros.
* ########################### */

/* #############################
* Function Macros:
*/




/*
* Function Macros.
* ########################## */

/* #############################
* Typedefs:
*/

typedef enum AppState_t
{
    AppState_Reset,
    AppState_Idle,
    AppState_Disconnected,
    AppState_Connected,
    AppState_TCPConnectionAccepted,
    AppState_TCPConnected,
    AppState_SocketRcvd,
    AppState_Closed,
} AppState_t;

/*
* Typedefs.
* ########################## */

/*
* Macros & Typedefs.
* ######################################################################## */

/* ##########################################################################
* Declarations:
*/

/* #############################
* Exported Variables:
*/


/*
* Exported Variables.
* ########################## */

/* #############################
* Exported Functions:
*/

extern void UseCaseSockets_UDPUnsecure();
extern void UseCaseSockets_TCP(bool isServer, bool isSecure);

/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */


#endif /* _AT_USECASESOCKET_DEFINED*/
