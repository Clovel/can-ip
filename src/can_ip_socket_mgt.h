/**
 * @brief CAN over IP socket management functions
 * 
 * @file can_ip_socket_mgt.c
 */

#ifndef CAN_IP_SOCKET_MGT_H
#define CAN_IP_SOCKET_MGT_H

/* Includes -------------------------------------------- */
#include "can_ip_error_codes.h"
#include "can_ip.h"

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Socket management functions ------------------------- */
cipErrorCode_t CIP_initCanSocket(const cipID_t pID);
cipErrorCode_t CIP_closeSocket(const cipID_t pID);

#endif /* CAN_IP_SOCKET_MGT_H */
