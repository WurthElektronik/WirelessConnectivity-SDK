/*
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK:
 *
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
 * COPYRIGHT (c) 2025 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 */

/**
 * @file endian_utils.h
 * @brief This is a helper header to support the endianness of different hosts using the WE Wireless Connectivity SDK.
 */

#ifndef ENDIAN_UTILS_H
#define ENDIAN_UTILS_H

#include <stdint.h>

/**
 * @brief Specify the endianness of the host.
 * 
 * @attention This can be overriden depending on the host, however depending on the platform 
 * this will result in the correct value by default.
 */
#ifndef IS_LITTLE_ENDIAN
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define IS_LITTLE_ENDIAN 0
#else
#define IS_LITTLE_ENDIAN 1
#endif
#endif

/**
 * @brief Converts a 16-bit value from host byte order to little-endian format.
 *
 * @param value The 16-bit integer in host byte order.
 * 
 * @return The 16-bit integer in little-endian byte order.
 */
static inline uint16_t host_to_le16(uint16_t value)
{
#if IS_LITTLE_ENDIAN
    return value;
#else
    return (value >> 8) | (value << 8);
#endif
}

/**
 * @brief Converts a 16-bit value from little-endian format to host byte order.
 *
 * @param value The 16-bit integer in little-endian byte order.
 * 
 * @return The 16-bit integer in host byte order.
 */
static inline uint16_t host_from_le16(uint16_t value)
{
#if IS_LITTLE_ENDIAN
    return value;
#else
    return (value >> 8) | (value << 8);
#endif
}

/**
 * @brief Converts a 32-bit value from host byte order to little-endian format.
 *
 * @param value The 32-bit integer in host byte order.
 * @return The 32-bit integer in little-endian byte order.
 */
static inline uint32_t host_to_le32(uint32_t value)
{
#if IS_LITTLE_ENDIAN
    return value;
#else
    return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) | ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
#endif
}

/**
 * @brief Converts a 32-bit value from little-endian format to host byte order.
 *
 * @param value The 32-bit integer in little-endian byte order.
 * @return The 32-bit integer in host byte order.
 */
static inline uint32_t host_from_le32(uint32_t value)
{
#if IS_LITTLE_ENDIAN
    return value;
#else
    return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) | ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
#endif
}

/**
 * @brief Converts a 64-bit value from host byte order to little-endian format.
 *
 * @param value The 64-bit integer in host byte order.
 * @return The 64-bit integer in little-endian byte order.
 */
static inline uint64_t host_to_le64(uint64_t value)
{
#if IS_LITTLE_ENDIAN
    return value;
#else
    return ((value >> 56) & 0x00000000000000FFULL) | ((value >> 40) & 0x000000000000FF00ULL) | ((value >> 24) & 0x0000000000FF0000ULL) | ((value >> 8) & 0x00000000FF000000ULL) | ((value << 8) & 0x000000FF00000000ULL) | ((value << 24) & 0x0000FF0000000000ULL) | ((value << 40) & 0x00FF000000000000ULL) | ((value << 56) & 0xFF00000000000000ULL);
#endif
}

/**
 * @brief Converts a 64-bit value from little-endian format to host byte order.
 *
 * @param value The 64-bit integer in little-endian byte order.
 * @return The 64-bit integer in host byte order.
 */
static inline uint64_t host_from_le64(uint64_t value)
{
#if IS_LITTLE_ENDIAN
    return value;
#else
    return ((value >> 56) & 0x00000000000000FFULL) | ((value >> 40) & 0x000000000000FF00ULL) | ((value >> 24) & 0x0000000000FF0000ULL) | ((value >> 8) & 0x00000000FF000000ULL) | ((value << 8) & 0x000000FF00000000ULL) | ((value << 24) & 0x0000FF0000000000ULL) | ((value << 40) & 0x00FF000000000000ULL) | ((value << 56) & 0xFF00000000000000ULL);
#endif
}

#endif // ENDIAN_UTILS_H
