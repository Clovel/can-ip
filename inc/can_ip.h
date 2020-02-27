/**
 * @brief CAN over IP API header
 * 
 * @file can_ip.h
 */

#ifndef CAN_IP_H
#define CAN_IP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes -------------------------------------------- */
#include "can_ip_error_codes.h"

#include <stdint.h>  /* TODO : Delete this and use custom types */
#include <stdbool.h> /* TODO : Delete this and use custom types */

/* Defines --------------------------------------------- */
#define CAN_MESSAGE_MAX_SIZE 8U

/* Define this beforehand if you want 
 * several CAN over IP modules. 
 */
#ifndef CAN_IP_MAX_NB_MODULES
#define CAN_IP_MAX_NB_MODULES 1U
#endif /* CAN_IP_MAX_NB_MODULES */

/* Type definitions ------------------------------------ */
typedef struct _cipMessage {
    uint32_t id;
    uint8_t  size;
    uint8_t  data[CAN_MESSAGE_MAX_SIZE];
    uint32_t flags;
} cipMessage_t;

typedef cipMessage_t canMessage_t;

typedef enum _cipModes {
    CAN_IP_MODE_UNKNOWN = 0U,
    CAN_IP_MODE_NORMAL  = 1U,
    CAN_IP_MODE_FD      = 2U
} cipMode_t;

typedef cipMode_t canMode_t;

typedef uint8_t cipID_t;
typedef int cipPort_t;

/* CAN over IP interface ------------------------------- */
/**
 * @brief CAN over IP module creation
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_createModule(const cipID_t pID);

/**
 * @brief CAN over IP initialisation
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_init(const cipID_t pID, const cipMode_t pCIPMode, const cipPort_t pPort);

/**
 * @brief CAN over IP check for initialisation
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_isInitialized(const cipID_t pID, bool * const pIsInitialized);

/**
 * @brief CAN over IP reset
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_reset(const cipID_t pID, const cipMode_t pCIPMode);

/**
 * @brief CAN over IP stop
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_stop(const cipID_t pID);

/**
 * @brief CAN over IP restart
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_restart(const cipID_t pID);

/** 
 * @brief CAN over IP send
 * Use this function to send a CAN message
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_send(const cipID_t pID, const cipMessage_t * const pMsg);

/**
 * @brief CAN over IP recieve
 * Use this function to get a CAN message
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return error_code
 */
cipErrorCode_t CIP_recv(const cipID_t pID, cipMessage_t * const pMsg, ssize_t * const pReadBytes);

#ifndef CIP_THREADED_PROCESS
/**
 * @brief CAN over IP process
 * This function can only be called from the exterior 
 * if CIP is compiled to use a non-threaded process
 * 
 * @return error code
 */
int CIP_process(const cipID_t pID);
#endif /* CIP_THREADED_PROCESS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CAN_IP_H */
