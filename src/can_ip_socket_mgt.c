/**
 * @brief CAN over IP API socket management functions
 * 
 * @file can_ip_socket_mgt.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"

/* Networking headers */
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

/* C system */
#include <unistd.h>
#include <stddef.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */
static const socklen_t sAddrLen = sizeof(struct sockaddr);

/* Extern variables ------------------------------------ */
extern cipInternalStruct_t gCIPInternalVars;

/* Socket management functions ------------------------- */
cipErrorCode_t CIP_initCanSocket(const cipID_t pID) {
    (void) pID;

    const int lBroadcastPermission = 1;

    /* Construct local address structure */
    gCIPInternalVars.socketInAddress.sin_family         = AF_INET;
    gCIPInternalVars.socketInAddress.sin_port           = htons(gCIPInternalVars.canPort);
    gCIPInternalVars.socketInAddress.sin_addr.s_addr    = inet_addr(gCIPInternalVars.canIP);

    /* set socket options */
    if(0 > setsockopt(gCIPInternalVars.canSocket, SOL_SOCKET, SO_BROADCAST, (void *)&lBroadcastPermission, sizeof(int))) {
        return CAN_IP_ERROR_NET;
    }

    /* Create the UDP socket (DGRAM for UDP */
    if(0 > (gCIPInternalVars.canSocket = socket(gCIPInternalVars.socketInAddress.sin_family, SOCK_DGRAM, 0))) {
        return CAN_IP_ERROR_NET;
    }

    /* Bind socket */
    if(-1 == bind(gCIPInternalVars.canSocket, (struct sockaddr *)&gCIPInternalVars.socketInAddress, sAddrLen)) {
        return CAN_IP_ERROR_NET;
    }

    /* Socket initialized */
    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_closeSocket(const cipID_t pID) {
    (void)pID;

    if(0 > close(gCIPInternalVars.canSocket)) {
        return CAN_IP_ERROR_NET;
    }

    return CAN_IP_ERROR_NONE;
}
