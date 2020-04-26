/**
 * @brief CAN over IP API receive functions
 * 
 * @file can_ip_recv.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"

/* C system */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>

/* errno */
#include <errno.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */
// static const socklen_t sAddrLen = sizeof(struct sockaddr);

/* Extern variables ------------------------------------ */
extern cipInternalStruct_t gCIP;

/* CAN reception functions ----------------------------- */
cipErrorCode_t CIP_msgAvail(const cipID_t pID, bool * const pMsgAvail) {
    pthread_mutex_lock(&gCIP.mutex);

    /* Check the argument pointer */
    if(NULL == pMsgAvail) {
        printf("[ERROR] <CIP_msgAvail> Output pointer argument is NULL\n");
        return CAN_IP_ERROR_ARG;
    }

    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_msgAvail> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check the socket if there are enough bytes to compose a CAN message */
    int lByteCount = 0;
    errno = 0;
    if(0 > ioctl(gCIP.canSocket, FIONREAD, &lByteCount)) {
        printf("[ERROR] <CIP_msgAvail> ioctl failed\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }

        *pMsgAvail = false;

        pthread_mutex_unlock(&gCIP.mutex);

        return CAN_IP_ERROR_NET;
    }

    pthread_mutex_unlock(&gCIP.mutex);

    *pMsgAvail = sizeof(cipMessage_t) <= (size_t)lByteCount;

    /* Check if the message is one of our own that looped back */
    if(*pMsgAvail) {
        struct sockaddr_in lSrcAddr;
        socklen_t lSrcAddrLen = sizeof(lSrcAddr);
        cipMessage_t lMsg;
        memset(&lMsg, 0, sizeof(cipMessage_t));
        lByteCount = recvfrom(gCIP.canSocket, (void *)&lMsg, sizeof(cipMessage_t), MSG_PEEK | MSG_DONTWAIT, 
            (struct sockaddr *)&lSrcAddr, &lSrcAddrLen);
        
        /* If the message is a loopback msg, read it for real to flush it out */
        if(gCIP.randID == lMsg.randID) {
            /* TODO : Find a better implementation than this */
            lByteCount = recvfrom(gCIP.canSocket, (void *)&lMsg, sizeof(cipMessage_t), MSG_PEEK | MSG_DONTWAIT, 
                (struct sockaddr *)&lSrcAddr, &lSrcAddrLen);
            *pMsgAvail = false;
        }
    }

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_recv(const cipID_t pID, 
    uint32_t * const pCOBID,
    uint8_t * const pLen,
    uint8_t * const pData,
    uint32_t * const pFlags,
    ssize_t * const pReadBytes)
{
    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_recv> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check if the module is already initialized */
    if(!gCIP.isInitialized) {
        printf("[ERROR] <CIP_rxThread> CAN-IP module %u is not initialized.\n", gCIP.cipInstanceID);
        return CAN_IP_ERROR_NOT_INIT;
    }

    /* Check arguments */
    if(NULL == pCOBID) {
        printf("[ERROR] <CIP_rxThread> pCOBID output pointer is NULL\n");
        return CAN_IP_ERROR_ARG;
    }
    if(NULL == pLen) {
        printf("[ERROR] <CIP_rxThread> pLen output pointer is NULL\n");
        return CAN_IP_ERROR_ARG;
    }
    if(NULL == pData) {
        printf("[ERROR] <CIP_rxThread> pData output pointer is NULL\n");
        return CAN_IP_ERROR_ARG;
    }
    if(NULL == pFlags) {
        printf("[ERROR] <CIP_rxThread> pFlags output pointer is NULL\n");
        return CAN_IP_ERROR_ARG;
    }
    if(NULL == pReadBytes) {
        printf("[ERROR] <CIP_rxThread> pReadBytes output pointer is NULL\n");
        return CAN_IP_ERROR_ARG;
    }

    cipMessage_t lMsg;
    memset(&lMsg, 0, sizeof(cipMessage_t));
    cipErrorCode_t lErrorCode = CIP_recvMsgStruct(pID, &lMsg, pReadBytes);
    if(CAN_IP_ERROR_NONE != lErrorCode) {
        printf("[ERROR] <CIP_send> CIP_recvMsgStruct failed !\n");
    } else {
        *pCOBID = lMsg.id;
        *pLen   = lMsg.size;
        *pFlags = lMsg.flags;
        for(uint8_t i = 0U; (i < *pLen) && (i < CAN_MESSAGE_MAX_SIZE); ++i) {
            pData[i] = lMsg.data[i];
        }
    }

    return lErrorCode;
}

cipErrorCode_t CIP_recvMsgStruct(const cipID_t pID, cipMessage_t * const pMsg, ssize_t * const pReadBytes) {
    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_recv> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check if the module is already initialized */
    if(!gCIP.isInitialized) {
        printf("[ERROR] <CIP_rxThread> CAN-IP module %u is not initialized.\n", gCIP.cipInstanceID);
        return CAN_IP_ERROR_NOT_INIT;
    }

    if(NULL == pMsg) {
        printf("[ERROR] <CIP_rxThread> Message is NULL\n");
        return CAN_IP_ERROR_ARG;
    }

    if(NULL == pReadBytes) {
        printf("[ERROR] <CIP_rxThread> pReadBytes output pointer is NULL\n");
        return CAN_IP_ERROR_ARG;
    }

    *pReadBytes = 0;
    struct sockaddr_in lSrcAddr;
    socklen_t lSrcAddrLen = sizeof(lSrcAddr);
    //char lSrcIPAddr[INET_ADDRSTRLEN] = "";

    pthread_mutex_lock(&gCIP.mutex);
    
    /* Receive the CAN frame */
    *pReadBytes = recvfrom(gCIP.canSocket, (void *)pMsg, sizeof(cipMessage_t), 0, 
        (struct sockaddr *)&lSrcAddr, &lSrcAddrLen);
    //*pReadBytes = recv(gCIP.canSocket, (void *)pMsg, sizeof(cipMessage_t), 0);
    if(0 > *pReadBytes) {
        if(EAGAIN != errno && EWOULDBLOCK != errno) {
            printf("[ERROR] <CIP_send> recvfrom failed !\n");
            if(0 != errno) {
                printf("        errno = %d (%s)\n", errno, strerror(errno));
            }
            return CAN_IP_ERROR_NET;
        } else {
            /* Nothing to read on the socket */
        }
    } else if (0 == *pReadBytes) {
        /* Nothing was read from the socket */
    } else {
        /* We got our message */
        
        // inet_ntop(PF_INET, &lSrcAddr.sin_addr, lSrcIPAddr, INET_ADDRSTRLEN);
        // printf("[DEBUG] <CIP_send> Received %ld bytes from %s\n", *pReadBytes, lSrcIPAddr, lSrcAddrLen);
    }

    pthread_mutex_unlock(&gCIP.mutex);

    return CAN_IP_ERROR_NONE;
}
