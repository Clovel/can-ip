/**
 * @brief CAN over IP API main functions
 * 
 * @file can_ip.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"
#include "can_ip.h"
#include "can_ip_socket_mgt.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h> /* For rand() */

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */
cipInternalStruct_t gCIP;

/* CAN over IP main functions -------------------------- */
cipErrorCode_t CIP_createModule(const cipID_t pID) {
    if(CAN_IP_MAX_NB_MODULES <= pID) {
        return CAN_IP_ERROR_ARG;
    }

    /* check if the module already exists */
    if(gCIP.cipInstanceID != pID) {
        return CAN_IP_ERROR_ARG;
    }

    /* TODO */

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_init(const cipID_t pID, const cipMode_t pCIPMode, const cipPort_t pPort, const bool pThreaded) {
    /* Check the ID */
    if(pID != gCIP.cipInstanceID) {
        printf("[ERROR] <CIP_init> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check if the module is already initialized */
    if(gCIP.isInitialized) {
        /* Module is already initialized,
         * so we do nothing */
        printf("[ERROR] <CIP_init> CAN-IP module %u is already initialized.\n", pID);
        /* TODO : Maybe reset ? */
        return CAN_IP_ERROR_ALREADY_INIT;
    }

    /* Initialize the module */
    gCIP.cipMode       = pCIPMode;
    gCIP.cipInstanceID = pID;
    gCIP.isStopped     = false;

    /* Set port */
    gCIP.canPort = pPort;

    /* Generate random ID */
    time_t lTime;
    srand((unsigned)time(&lTime));
    gCIP.randID  = (rand() & 0xFFU) << 0U;
    gCIP.randID |= (rand() & 0xFFU) << 8U;
    gCIP.randID |= (rand() & 0xFFU) << 16U;
    gCIP.randID |= (rand() & 0xFFU) << 24U;
    printf("[DEBUG] Generated random ID : %u\n", gCIP.randID);

    /* Initialize the socket */
    if(CAN_IP_ERROR_NONE != CIP_initCanSocket(pID)) {
        printf("[ERROR] <CIP_init> Failed to initialize socket w/ CIP_initCanSocket\n");
        return CAN_IP_ERROR_NET;
    }

    /* Initialize thread related variables */
    gCIP.rxThreadOn      = false;
    gCIP.rxThreadEnabled = pThreaded;
    gCIP.putMessageFct   = NULL;

    gCIP.isInitialized = true;

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_isInitialized(const cipID_t pID, bool * const pIsInitialized) {
    if(NULL != pIsInitialized
        && gCIP.cipInstanceID == pID)
    {
        *pIsInitialized = gCIP.isInitialized;
    } else {
        printf("[ERROR] <CIP_isInitialized> No CAN-IP module has the ID %u.\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_reset(const cipID_t pID, const cipMode_t pCIPMode, const bool pThreaded) {
    if(!gCIP.isInitialized) {
        /* You shouldn't "reset" a non-initialized module */
        printf("[ERROR] <CIP_reset> CAN-IP module %u is not initialized, cannot reset.\n", pID);
        return CAN_IP_ERROR_NOT_INIT;
    }

    gCIP.isStopped = true;
    gCIP.isInitialized = false;

    /* Close the socket */
    if(CAN_IP_ERROR_NONE != CIP_closeSocket(pID)) {
        return CAN_IP_ERROR_NET;
    }

    return CIP_init(pID, pCIPMode, gCIP.canPort, pThreaded);
}

cipErrorCode_t CIP_stop(const cipID_t pID) {
    (void)pID; /* TODO : Multiline CAN */

    if(!gCIP.isInitialized) {
        printf("[ERROR] <CIP_stop> CAN-IP module %u is not initialized, cannot stop it.\n", pID);
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIP.isStopped = true;

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_restart(const cipID_t pID) {
    (void)pID; /* TODO : Multiline CAN */

    if(!gCIP.isInitialized) {
        printf("[ERROR] <CIP_restart> CAN-IP module %u is not initialized, cannot restart it.\n", pID);
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIP.isStopped = false;

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_process(const cipID_t pID) {
    (void)pID;

    /* Check if the module is already initialized */
    if(!gCIP.isInitialized) {
        printf("[ERROR] <CIP_rxThread> CAN-IP module %u is not initialized.\n", gCIP.cipInstanceID);
        return CAN_IP_ERROR_NOT_INIT;
    }

    if(gCIP.rxThreadEnabled) {
        if(NULL == gCIP.putMessageFct) {
            printf("[ERROR] <CIP_rxThread> Message buffer getter function is NULL.\n");
            return CAN_IP_ERROR_CONFIG;
        }

        cipErrorCode_t lErrorCode = CAN_IP_ERROR_NONE;

        if(!gCIP.rxThreadOn) {
            /* Start reception thread */
            lErrorCode = CIP_startRxThread(pID);
            if(CAN_IP_ERROR_NONE != lErrorCode) {
                printf("[ERROR] <CIP_rxThread> CIP_startRxThread failed w/ error code %u\n", lErrorCode);
                return CAN_IP_ERROR_SYS;
            }
        }
    } else {
        /* Non threaded process */
        /* TODO : Non-threaded process.
         * - Maximum messages per process call
         *      - Error, discard or wait until next call ?
         * - Timout ?
         */
    }

    return CAN_IP_ERROR_NONE;
}
