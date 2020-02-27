/**
 * @brief CAN over IP socket management functions
 * 
 * @file can_ip_socket_mgt.c
 */

/* Includes -------------------------------------------- */
#include "can_ip_private.h"
#include "can_ip_error_codes.h"

/* Networking headers */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

/* C system */
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

/* Socket management functions ------------------------- */
cipErrorCode_t CIP_initCanSocket(const cipID_t pID) {
    (void) pID;

    const int lBroadcastPermission = 1;

    /* Construct local address structure */
    gCIPInternalVars.socketInAddress.sin_family         = PF_INET;
    gCIPInternalVars.socketInAddress.sin_port           = htons(gCIPInternalVars.canPort);
    // gCIPInternalVars.socketInAddress.sin_addr.s_addr    = inet_addr(gCIPInternalVars.canIP);
    gCIPInternalVars.socketInAddress.sin_addr.s_addr    = INADDR_ANY;

    // /* Get socket information */
    // struct addrinfo lHints = {
    //     .ai_family = AF_UNSPEC,
    //     .ai_socktype = SOCK_DGRAM,
    //     .ai_protocol = IPPROTO_UDP
    // };

    // int lFctReturn = getaddrinfo(gCIPInternalVars.canIP, itoa(gCIPInternalVars.canPort), &lHints, &gCIPInternalVars.addrinfo);
    // if(0 != lFctReturn || NULL == gCIPInternalVars.addrinfo) {
    //     printf("[ERROR] <CIP_initcanSocket> getaddrinfo failed !\n");
    //     printf("        Invalid address (%s) or port (%d)\n", gCIPInternalVars.canIP, gCIPInternalVars.canPort);
    //     return CAN_IP_ERROR_NET;
    // }

    printf("[DEBUG] <CIP_initcanSocket> IPAddr = %s\n", gCIPInternalVars.canIP);
    printf("[DEBUG] <CIP_initcanSocket> Port   = %d\n", gCIPInternalVars.canPort);
    
    /* Create the UDP socket (DGRAM for UDP */
    errno = 0;
    if(0 > (gCIPInternalVars.canSocket = socket(gCIPInternalVars.socketInAddress.sin_family, SOCK_DGRAM, IPPROTO_UDP))) {
        printf("[ERROR] <CIP_initcanSocket> socket failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }

        freeaddrinfo(gCIPInternalVars.addrinfo);

        return CAN_IP_ERROR_NET;
    }

    /* set socket options */
    errno = 0;
    if(0 > setsockopt(gCIPInternalVars.canSocket, SOL_SOCKET, SO_BROADCAST, (const void *)&lBroadcastPermission, sizeof(int))) {
        printf("[ERROR] <CIP_initcanSocket> setsockopt SO_BROADCAST failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }
    if(0 > setsockopt(gCIPInternalVars.canSocket, SOL_SOCKET, SO_REUSEADDR, (const void *)&lBroadcastPermission, sizeof(int))) {
        printf("[ERROR] <CIP_initcanSocket> setsockopt SO_REUSEADDR failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }
    if(0 > setsockopt(gCIPInternalVars.canSocket, SOL_SOCKET, SO_REUSEPORT, (const void *)&lBroadcastPermission, sizeof(int))) {
        printf("[ERROR] <CIP_initcanSocket> setsockopt SO_REUSEPORT failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }

    /* Set the socket as non-blocking */
    int lFlags = 0;
    if(0 > (lFlags = fcntl(gCIPInternalVars.canSocket, F_GETFL))) {
        printf("[ERROR] <CIP_initcanSocket> fcntl F_GETFL failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }

    lFlags |= O_NONBLOCK;
    if(0 > fcntl(gCIPInternalVars.canSocket, F_SETFL, lFlags)) {
        printf("[ERROR] <CIP_initcanSocket> fcntl F_SETFL failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }

    /* Bind socket */
    errno = 0;
    if(0 > bind(gCIPInternalVars.canSocket, (struct sockaddr *)&gCIPInternalVars.socketInAddress, sAddrLen)) {
        printf("[ERROR] <CIP_initcanSocket> bind failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }

    /* Socket initialized */
    return CAN_IP_ERROR_NONE;
}

cipErrorCode_t CIP_closeSocket(const cipID_t pID) {
    (void)pID;

    /* Close the socket */
    errno = 0;
    if(0 > close(gCIPInternalVars.canSocket)) {
        printf("[ERROR] <CIP_initcanSocket> close failed !\n");
        if(0 != errno) {
            printf("        errno = %d (%s)\n", errno, strerror(errno));
        }
        return CAN_IP_ERROR_NET;
    }

    return CAN_IP_ERROR_NONE;
}
