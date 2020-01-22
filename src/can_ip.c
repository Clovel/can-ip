/**
 * @brief CAN over IP API main functions
 * 
 * @file can_ip.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"
#include "can_ip.h"

#include <stddef.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */
cipInternalStruct_t gCIPInternalVars;

/* CAN over IP main functions -------------------------- */
int CIP_init(const cipID_t pID, const cipMode_t pCIPMode) {
    /* Check the ID */
    if(pID != gCIPInternalVars.cipInstanceID) {
        return CAN_IP_ERROR_ARG;
    }

    /* Check if the module is already initialized */
    if(gCIPInternalVars.isInitialized) {
        /* Module is already initialized,
         * so we do nothing */
        /* TODO : Maybe reset ? */
        return CAN_IP_ERROR_ALREADY_INIT;
    }

    /* Initialize the module */
    gCIPInternalVars.cipMode       = pCIPMode;
    gCIPInternalVars.cipInstanceID = pID;
    gCIPInternalVars.isStopped     = false;

    /* TODO : Remove this w/ correct config mechanism */
    gCIPInternalVars.canIP   = "192.168.1.255";
    gCIPInternalVars.canPort = 560;

    /* Initialize the socket */
    if(CAN_IP_ERROR_NONE != CIP_initCanSocket(pID)) {
        return CAN_IP_ERROR_NET;
    }

    gCIPInternalVars.isInitialized = true;

    return CAN_IP_ERROR_NONE;
}

int CIP_isInitialized(const cipID_t pID, bool * const pIsInitialized) {
    if(NULL != pIsInitialized
        && gCIPInternalVars.cipInstanceID == pID) {
        *pIsInitialized = gCIPInternalVars.isInitialized;
    } else {
        return CAN_IP_ERROR_ARG;
    }

    return CAN_IP_ERROR_NONE;
}

int CIP_reset(const cipID_t pID, const cipMode_t pCIPMode) {
    if(!gCIPInternalVars.isInitialized) {
        /* You shouldn't "reset" a non-initialized module */
        return CAN_IP_ERROR_NOT_INIT;
    }

    gCIPInternalVars.isStopped = true;
    gCIPInternalVars.isInitialized = false;

    /* Close the socket */
    if(CAN_IP_ERROR_NONE != CIP_closeSocket(pID)) {
        return CAN_IP_ERROR_NET;
    }

    return CIP_init(pID, pCIPMode);
}

int CIP_stop(const cipID_t pID) {
    (void)pID; /* TODO : Multiline CAN */

    if(!gCIPInternalVars.isInitialized) {
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIPInternalVars.isStopped = true;

    return CAN_IP_ERROR_NONE;
}

int CIP_restart(const cipID_t pID) {
    (void)pID; /* TODO : Multiline CAN */

    if(!gCIPInternalVars.isInitialized) {
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIPInternalVars.isStopped = false;

    return CAN_IP_ERROR_NONE;
}
