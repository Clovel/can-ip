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

/* Extern variables ------------------------------------ */
extern cipInternalStruct_t gCIPInternalVars;


cipErrorCode_t CIP_send(const cipID_t pID,
    const uint32_t pCANID,
    const uint8_t pSize,
    const uint8_t * const pData,
    const uint32_t pFlags)
{
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

    /* Build CIP message */
    cipMessage_t lMsg;
    memset(lMsg.data, 0, CAN_MESSAGE_MAX_SIZE);
    lMsg.id    = pCANID;
    lMsg.size  = pSize;
    lMsg.flags = pFlags;
    for(uint8_t i = 0U; (i < lMsg.size) && (i < CAN_MESSAGE_MAX_SIZE) && (NULL != pData); i++) {
        lMsg.data[i] = pData[i];
    }
    
    /* Set the random ID in the message */
    lMsg.randID = gCIPInternalVars.randID;

    ssize_t lSentBytes = 0;

    errno = 0;
    lSentBytes = sendto(gCIPInternalVars.canSocket, (const void *)&lMsg, sizeof(cipMessage_t), 0, 
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
