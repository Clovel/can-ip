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
#include <stdio.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */
cipInternalStruct_t gCIPInternalVars;

/* CAN over IP main functions -------------------------- */
cipErrorCode_t CIP_createModule(const cipID_t pID) {
    if(CAN_IP_MAX_NB_MODULES <= pID) {
        return CAN_IP_ERROR_ARG;
    }

    /* check if the module already exists */
    if(gCIPInternalVars.cipInstanceID != pID) {
        return CAN_IP_ERROR_ARG;
    }

    /* TODO */

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_init(const cipID_t pID, const cipMode_t pCIPMode, const cipPort_t pPort) {
    /* Check the ID */
    if(pID != gCIPInternalVars.cipInstanceID) {
        printf("[ERROR] <CIP_init> No CAN-IP module has the ID %u\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    /* Check if the module is already initialized */
    if(gCIPInternalVars.isInitialized) {
        /* Module is already initialized,
         * so we do nothing */
        printf("[ERROR] <CIP_init> CAN-IP module %u is already initialized.\n", pID);
        /* TODO : Maybe reset ? */
        return CAN_IP_ERROR_ALREADY_INIT;
    }

    /* Initialize the module */
    gCIPInternalVars.cipMode       = pCIPMode;
    gCIPInternalVars.cipInstanceID = pID;
    gCIPInternalVars.isStopped     = false;

    gCIPInternalVars.isInitialized = true;

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_isInitialized(const cipID_t pID, bool * const pIsInitialized) {
    if(NULL != pIsInitialized
        && gCIPInternalVars.cipInstanceID == pID)
    {
        *pIsInitialized = gCIPInternalVars.isInitialized;
    } else {
        printf("[ERROR] <CIP_isInitialized> No CAN-IP module has the ID %u.\n", pID);
        return CAN_IP_ERROR_ARG;
    }

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_reset(const cipID_t pID, const cipMode_t pCIPMode) {
    if(!gCIPInternalVars.isInitialized) {
        /* You shouldn't "reset" a non-initialized module */
        printf("[ERROR] <CIP_reset> CAN-IP module %u is not initialized, cannot reset.\n", pID);
        return CAN_IP_ERROR_NOT_INIT;
    }

    gCIPInternalVars.isStopped = true;
    gCIPInternalVars.isInitialized = false;

    /* Close the socket */
    if(CAN_IP_ERROR_NONE != CIP_closeSocket(pID)) {
        return CAN_IP_ERROR_NET;
    }

    return CIP_init(pID, pCIPMode, gCIPInternalVars.canPort);
}

cipErrorCode_t CIP_stop(const cipID_t pID) {
    (void)pID; /* TODO : Multiline CAN */

    if(!gCIPInternalVars.isInitialized) {
        printf("[ERROR] <CIP_stop> CAN-IP module %u is not initialized, cannot stop it.\n", pID);
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIPInternalVars.isStopped = true;

    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_restart(const cipID_t pID) {
    (void)pID; /* TODO : Multiline CAN */

    if(!gCIPInternalVars.isInitialized) {
        printf("[ERROR] <CIP_restart> CAN-IP module %u is not initialized, cannot restart it.\n", pID);
        return CAN_IP_ERROR_NOT_INIT;
    }
    
    gCIPInternalVars.isStopped = false;

    return CAN_IP_ERROR_NONE;
}
