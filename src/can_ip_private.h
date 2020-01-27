/**
 * @brief CAN over IP private header
 * 
 * @file can_ip_private.h
 */

#ifndef CAN_IP_PRIVATE_H
#define CAN_IP_PRIVATE_H

/* Includes -------------------------------------------- */
#include "can_ip.h"

#include <netinet/in.h>

#include <stdint.h>  /* TODO : Delete this and use custom types */
#include <stdbool.h> /* TODO : Delete this and use custom types */

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */
typedef int cipSocket_t;
typedef int cipPort_t;

typedef struct _cipInternalVariables {
    uint8_t   cipInstanceID; /* TODO : Multiline CAN */
    cipMode_t cipMode;
    bool      isInitialized;
    bool      isStopped;

    cipSocket_t         canSocket; /* The socket used to communicate CAN frames */
    struct sockaddr_in  socketInAddress;
    char               *canIP;      /* IP Address */
    cipPort_t           canPort;    /* Server port number */
    struct hostent     *hostPtr; /* Server information */
} cipInternalStruct_t;

#endif /* CAN_IP_PRIVATE_H */
