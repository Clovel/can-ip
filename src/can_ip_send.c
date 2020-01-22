/**
 * @brief CAN over IP API send functions
 * 
 * @file can_ip_send.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"

/* C system */
#include <stddef.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */
static const socklen_t sAddrLen = sizeof(struct sockaddr);

/* Extern variables ------------------------------------ */
extern cipInternalStruct_t gCIPInternalVars;


int CIP_send(const cipID_t pID, const cipMessage_t * const pMsg) {
    (void)pID;

    if(NULL == pMsg) {
        return CAN_IP_ERROR_ARG;
    }

    ssize_t lSentBytes = 0;

    lSentBytes = sendto(gCIPInternalVars.canSocket, (const void *)pMsg, sizeof(cipMessage_t), 0, 
        (struct sockaddr *)&gCIPInternalVars.socketInAddress, sAddrLen);
    if(sizeof(cipMessage_t) != lSentBytes) {
        return CAN_IP_ERROR_NET;
    }

    return CAN_IP_ERROR_NONE;
}
