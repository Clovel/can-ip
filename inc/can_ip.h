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

#include <sys/types.h>

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
    uint32_t randID; /**< Random ID of the message sender */
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

typedef int (*cipPutMessageFct_t)(const uint32_t, const uint8_t, const uint8_t * const, const uint32_t);

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
cipErrorCode_t CIP_init(const cipID_t pID, const cipMode_t pCIPMode, const cipPort_t pPort, const bool pThreaded);

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
cipErrorCode_t CIP_reset(const cipID_t pID, const cipMode_t pCIPMode, const bool pThreaded);

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
 * @param[in]   pCANID  CAN message ID.
 * @param[in]   pSize   CAN message size.
 * @param[in]   pData   CAN message data.
 * @param[in]   pFlags  CAN message flags.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_send(const cipID_t pID,
    const uint32_t pCANID,
    const uint8_t pSize,
    const uint8_t * const pData,
    const uint32_t pFlags);

/**
 * @brief CAN over IP check for available message
 * Use this function to check for an available CAN message
 * 
 * @param[in]   pID         ID of the driver used.
 * @param[out]  pMsgAvail   Output ptr
 * 
 * @return error_code
 */
cipErrorCode_t CIP_msgAvail(const cipID_t pID, bool * const pMsgAvail);

/**
 * @brief CAN over IP recieve
 * Use this function to get a CAN message
 * 
 * @param[in]   pID         ID of the driver used.
 * @param[out]  pMsg        Read message COB-ID
 * @param[out]  pLen        Read message length
 * @param[out]  pData       Read message payload
 * @param[out]  pFlags      Read message flags
 * @param[out]  pReadBytes  Number of bytes read from the socket
 * 
 * @return error_code
 */
cipErrorCode_t CIP_recv(const cipID_t pID, 
    uint32_t * const pCOBID,
    uint8_t * const pLen,
    uint8_t * const pData,
    uint32_t * const pFlags,
    ssize_t * const pReadBytes);

/**
 * @brief CAN over IP recieve
 * Use this function to get a CAN message
 * 
 * @param[in]   pID         ID of the driver used.
 * @param[out]  pMsg        Read message
 * @param[out]  pReadBytes  Number of bytes read from the socket
 * 
 * @return error_code
 */
cipErrorCode_t CIP_recvMsgStruct(const cipID_t pID, cipMessage_t * const pMsg, ssize_t * const pReadBytes);

/**
 * @brief Sets the function used to give a message to
 * the driver's caller's stack.
 * 
 * @param[in]   pID     ID of the driver used.
 * @param[in]   pFct    Function used to hand the message over to the caller.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_setPutMessageFunction(const cipID_t pID, const cipPutMessageFct_t pFct);

/**
 * @brief Print a CAN over IP message (long format)
 * 
 * @param[in]   pMsg    CAN Message to print
 */
void CIP_printMessage(const cipMessage_t * const pMsg);

/**
 * @brief Print a CAN over IP message (short format)
 * 
 * @param[in]   pMsg    CAN Message to print
 */
void CIP_printMessageShort(const cipMessage_t * const pMsg);

/**
 * @brief CAN over IP process
 * This function can only be called from the exterior 
 * if CIP is compiled to use a non-threaded process
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_process(const cipID_t pID);

/**
 * @brief Starts the receiving thread.
 * 
 * @param[in]   pID     ID of the driver used.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_startRxThread(const cipID_t pID);

/**
 * @brief Getter for the "Thread On" variable
 * 
 * @param[in]   pID     ID of the driver used.
 * @param[out]  pOn     Output ptr. true : thread is running, false : thread is off.
 * 
 * @return Error code
 */
cipErrorCode_t CIP_isRxThreadOn(const cipID_t pID, bool * const pOn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CAN_IP_H */
