#ifndef _AT_FILE_DEFINED
#define _AT_FILE_DEFINED

/* ###########################################################################
* Includes:
*/

#include <stdbool.h>
#include <stdint.h>
#include "../Calypso.h"

/*
* Includes.
* ######################################################################### */

/* ###########################################################################
* Macros & Typedefs:
*/

/* #############################
* Constant Macros:
*/

#define FILENAME_MAX_LENGTH     (uint8_t) 180
#define FILE_MIN_SIZE           (uint16_t)4096
#define FILE_FAILSAFE_MIN_SIZE  (uint16_t)8192

#define ATFILE_OPEN_CREATE                  (uint32_t)(1 << 0)
#define ATFILE_OPEN_READ                    (uint32_t)(1 << 1)
#define ATFILE_OPEN_WRITE                   (uint32_t)(1 << 2)
#define ATFILE_OPEN_OVERWRITE               (uint32_t)(1 << 3)
#define ATFILE_OPEN_CREATE_FAILSAFE         (uint32_t)(1 << 4)
#define ATFILE_OPEN_CREATE_SECURE           (uint32_t)(1 << 5)
#define ATFILE_OPEN_CREATE_NOSIGNATURE      (uint32_t)(1 << 6)
#define ATFILE_OPEN_CREATE_STATIC_TOKEN     (uint32_t)(1 << 7)
#define ATFILE_OPEN_CREATE_VENDOR_TOKEN     (uint32_t)(1 << 8)
#define ATFILE_OPEN_CREATE_PUBLIC_WRITE     (uint32_t)(1 << 9)
#define ATFILE_OPEN_CREATE_PUBLIC_READ      (uint32_t)(1 << 10)
#define ATFILE_OPEN_NUM_OF_BITS             11


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

extern bool ATFile_open(const char *fileName, uint32_t options, uint16_t fileSize, uint32_t *fileID, uint32_t *secureToken);
extern bool ATFile_close(uint32_t fileID, char *certFileName, char *signature);
extern bool ATFile_del(const char *fileName, uint32_t secureToken);
extern bool ATFile_read(uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, uint16_t bytesToRead, Calypso_DataFormat_t *pOutFormat, uint16_t *byteRead, char *data);
extern bool ATFile_write(uint32_t fileID, uint16_t offset, Calypso_DataFormat_t format, bool encodeToBase64, uint16_t bytesToWrite, char *data, uint16_t *writtenBytes);

/*
* Exported Functions.
* ########################## */

/*
* Declarations.
* ######################################################################### */


#endif /* _AT_FILE_DEFINED*/
