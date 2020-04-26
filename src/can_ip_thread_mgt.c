/**
 * @brief CAN over IP API send functions
 * 
 * @file can_ip_send.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"
#include "can_ip.h"

/* C system */
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */
static pthread_t sThread = 0;

/* Extern variables ------------------------------------ */
extern cipInternalStruct_t gCIP;

/* Thread management functions ------------------------- */
cipErrorCode_t CIP_setPutMessageFunction(const cipID_t pID,
    const cipPutMessageFct_t pFct)
{
    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_setPutMessageFunction> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    if(NULL == pFct) {
        printf("[ERROR] <CIP_setPutMessageFunction> Function ptr arg is NULL !\n");
        return CAN_IP_ERROR_ARG;
    }

    gCIP.putMessageFct = pFct;

    return CAN_IP_ERROR_NONE;
}

static void CIP_rxThreadCleanup(void *pPtr) {
    (void)pPtr;
    gCIP.rxThreadOn = false;
}

static void CIP_rxThread(const cipID_t * const pID) {
    /* Check if the parameter is NULL */
    if(NULL == pID) {
        printf("[ERROR] <CIP_rxThread> ID ptr is NULL !\n");
        return;
    }

    /* Save the parameter before it is destroyed by the tread creator */
    const cipID_t lID = *pID;

    /* Check the ID */
    if(lID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_rxThread> No CAN-IP module has the ID %u\n", lID);
        return;
    }

    /* Check if the module is already initialized */
    if(!gCIP.isInitialized) {
        printf("[ERROR] <CIP_rxThread> CAN-IP module %u is not initialized.\n", gCIP.cipInstanceID);
        return;
    }

    if(NULL == gCIP.putMessageFct) {
        printf("[ERROR] <CIP_rxThread> Message buffer getter function is NULL.\n");
        return;
    }

    cipErrorCode_t  lErrorCode      = CAN_IP_ERROR_NONE;
    int             lGetBufferError = 0;
    ssize_t         lReadBytes      = 0;
    bool            lMsgAvail       = false;

    /* Starting thread routine */
    pthread_cleanup_push((void (*)(void *))CIP_rxThreadCleanup, NULL);

    gCIP.rxThreadOn = true;

    /* Infinite Rx loop */
    printf("[DEBUG] <CIP_rxThread> Starting RX thread.\n");
    while (CAN_IP_ERROR_NONE == lErrorCode) {
        /* Initialize a CIP message */
        cipMessage_t lMsg;
        memset(&lMsg, 0, sizeof(cipMessage_t));

        /* Check if a message is available */
        if(CAN_IP_ERROR_NONE != CIP_msgAvail(lID, &lMsgAvail)) {
            printf("[ERROR] <CIP_rxThread> CIP_msgAvail failed w/ error code %u\n", lErrorCode);
            break;
        }

        /* Check if message is available (not necessary here) */
        // if(!lMsgAvail) {
        //     /* Message not available, don't even bother trying to read the socket */
        //     continue;
        // }

        /* Reading a CAN message */
        lErrorCode = CIP_recvMsgStruct(lID, &lMsg, &lReadBytes);
        if(CAN_IP_ERROR_NONE != lErrorCode) {
            printf("[ERROR] <CIP_rxThread> CIP_recv failed w/ error code %u\n", lErrorCode);
            break;
        }

        if(CAN_IP_ERROR_NONE == lErrorCode && 0 > lReadBytes) {
            /* Nothing read, the socket is non-blocking */
            continue;
        }
        
        if(sizeof(cipMessage_t) != lReadBytes) {
            printf("[ERROR] <CIP_rxThread> CIP_recv received inconsistent data of size %d\n", lReadBytes);
            break;
        }

        /* Check if the message is a loopback message from this instance of CIP */
        if(gCIP.randID == lMsg.randID) {
            /* We sent this ! Ignoring... */
            continue;
        }

        /* Get buffer to store this data */
        lGetBufferError = gCIP.putMessageFct(lMsg.id, lMsg.size, lMsg.data, lMsg.flags);
        if(0 != lGetBufferError) {
            printf("[ERROR] <CIP_rxThread> putMessageFct callback failed w/ error code %u\n", lErrorCode);
            break;
        }

        usleep(10000U);
    }

    printf("[ERROR] <CIP_rxThread> RX thread shut down. (error code = %d)\n", lErrorCode);

    gCIP.rxThreadOn = false;

    /* Mandatory pop */
    pthread_cleanup_pop(1);
}

cipErrorCode_t CIP_startRxThread(const cipID_t pID) {
    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_startRxThread> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    if(NULL == gCIP.putMessageFct) {
        printf("[ERROR] <CIP_startRxThread> Message buffer getter function is NULL.\n");
        return CAN_IP_ERROR_CONFIG;
    }

    int lSysResult = 0;
    lSysResult = pthread_create(&sThread, NULL, (void * (*)(void *))CIP_rxThread, (void *)&pID);
    if (0 < lSysResult) {
        printf("[ERROR] <CIP_startRxThread> Thread creation failed\n");
        return CAN_IP_ERROR_SYS;
    } else {
        printf("[INFO ] <CIP_startRxThread> Thread creation successful\n");
    }

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_isRxThreadOn(const cipID_t pID, bool * const pOn) {
    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_isRxThreadOn> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check argument ptr */
    if(NULL == pOn) {
        printf("[ERROR] <CIP_isRxThreadOn> Parameter ptr is NULL !\n");
        return CAN_IP_ERROR_ARG;
    }

    *pOn = gCIP.rxThreadOn;

    return CAN_IP_ERROR_NONE;
}
