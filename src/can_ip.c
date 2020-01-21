/**
 * @brief CAN over IP API main functions
 * 
 * @file can_ip.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"
#include "can_ip.h"

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
    if(gCIPInternalVars.isÌnitialized) {
        /* Module is already initialized,
         * so we do nothing */
        /* TODO : Maybe reset ? */
        return CAN_IP_ERROR_ALREADY_INIT;
    }

    /* Initialize the module */
    gCIPInternalVars.cipMode       = pCIPMode;
    gCIPInternalVars.cipInstanceID = pID;
    gCIPInternalVars.isStopped     = false;
    gCIPInternalVars.isÌnitialized = true;

    return CAN_IP_ERROR_NONE;
}

int CIP_isInitialized(const cipID_t pID, bool * const pIsInitialized) {
    if(NULL != pIsInitialized
        && gCIPInternalVars.cipInstanceID == pID) {
        *pIsInitialized = gCIPInternalVars.isÌnitialized;
    } else {
        return CAN_IP_ERROR_ARG;
    }

    return CAN_IP_ERROR_NONE;
}

int CIP_reset(const cipID_t pID, const cipMode_t pCIPMode) {
    if(!gCIPInternalVars.isÌnitialized) {
        /* You shouldn't "reset" a non-initialized module */
        return CAN_IP_ERROR_NOT_INIT;
    }

    gCIPInternalVars.isStopped = true;
    gCIPInternalVars.isÌnitialized = false;
    CIP_init(pID, pCIPMode);
}

int CIP_stop(const cipID_t pID) {
    if(!gCIPInternalVars.isÌnitialized) {
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIPInternalVars.isStopped = true;

    return CAN_IP_ERROR_NONE;
}

int CIP_restart(const cipID_t pID) {
    if(!gCIPInternalVars.isÌnitialized) {
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIPInternalVars.isStopped = false;

    return CAN_IP_ERROR_NONE;
}
