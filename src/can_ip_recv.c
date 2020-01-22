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

int CIP_recv(const cipID_t pID, cipMessage_t * const pMsg) {
    (void)pID;

    if(NULL == pMsg) {
        return CAN_IP_ERROR_ARG;
    }

    ssize_t lReadBytes = 0;
    socklen_t lAddrLen = sAddrLen;

    lReadBytes = recvfrom(gCIPInternalVars.canSocket, (void *)pMsg, sizeof(cipMessage_t), 0, 
        (struct sockaddr *)&gCIPInternalVars.socketInAddress, &lAddrLen);
    if(0 > lReadBytes) {
        return CAN_IP_ERROR_NET;
    }

    return CAN_IP_ERROR_NONE;
}
