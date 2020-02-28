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
#include <stdio.h>
#include <string.h>

/* errno */
#include <errno.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */
static const socklen_t sAddrLen = sizeof(struct sockaddr);

/* Extern variables ------------------------------------ */
extern cipInternalStruct_t gCIPInternalVars;


cipErrorCode_t CIP_send(const cipID_t pID, const cipMessage_t * const pMsg) {
    pthread_mutex_lock(&gCIPInternalVars.mutex);

    /* Check the ID */
    if(pID != gCIPInternalVars.cipInstanceID) {
        printf("[ERROR] <CIP_send> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check if the module is already initialized */
    if(!gCIPInternalVars.isInitialized) {
        printf("[ERROR] <CIP_rxThread> CAN-IP module %u is not initialized.\n", gCIPInternalVars.cipInstanceID);
        return CAN_IP_ERROR_NOT_INIT;
    }

    if(NULL == pMsg) {
        printf("[ERROR] <CIP_send> Message to send is NULL\n");
        return CAN_IP_ERROR_ARG;
    }

    ssize_t lSentBytes = 0;

    errno = 0;
    lSentBytes = sendto(gCIPInternalVars.canSocket, (const void *)pMsg, sizeof(cipMessage_t), 0, 
        (const struct sockaddr *)&gCIPInternalVars.socketInAddress, sizeof(gCIPInternalVars.socketInAddress));
    if(sizeof(cipMessage_t) != lSentBytes) {
        printf("[ERROR] <CIP_send> sendto failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }

    pthread_mutex_unlock(&gCIPInternalVars.mutex);

    return CAN_IP_ERROR_NONE;
}
