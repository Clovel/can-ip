/**
 * @brief CAN over IP error codes
 * 
 * @file can_ip_error_codes.h
 */

#ifndef CAN_IP_ERROR_CODES_H
#define CAN_IP_ERROR_CODES_H

/* Includes -------------------------------------------- */

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */
typedef enum _cipErrorCodes {
    CAN_IP_ERROR_UNKNOWN      = 0U,
    CAN_IP_ERROR_NONE         = 1,
    CAN_IP_ERROR_ARG          = 2,
    CAN_IP_ERROR_SYS          = 3,
    CAN_IP_ERROR_NET          = 4,
    CAN_IP_ERROR_ALREADY_INIT = 5,
    CAN_IP_ERROR_NOT_INIT     = 6,
    CAN_IP_ERROR_STOPPED      = 7
} cipErrorCode_t;

#endif /* CAN_IP_ERROR_CODES_H */
