/**
 * @brief CAN over IP basic sender example
 * 
 * @file main.c
 */

/* Includes -------------------------------------------- */
/* can-ip */
#include "can_ip.h"
#include "can_ip_error_codes.h"

/* C System */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Defines --------------------------------------------- */

/* Notes ----------------------------------------------- */

/* Variable declaration -------------------------------- */

/* Type definitions ------------------------------------ */

/* Support functions ----------------------------------- */

/* ----------------------------------------------------- */
/* Main ------------------------------------------------ */
/* ----------------------------------------------------- */
int main(const int argc, const char * const * const argv) {
    (void)argc;
    (void)argv;

    unsigned int lErrorCode = 0U;

    /* Initialize the CAN over IP module */
    if(1U != (lErrorCode = CIP_init(0U, CAN_IP_MODE_NORMAL, 15024, false))) {
        printf("[ERROR] CIP_init failed w/ error code %u.\n", lErrorCode);
        exit(EXIT_FAILURE);
    }

    /* Set up CAN message */
    cipMessage_t lMsg = {
        0x701U,
        8U,
        {
            0x01U,
            0x23U,
            0x45U,
            0x67U,
            0x89U,
            0xABU,
            0xCDU,
            0xEFU
        },
        0x00000000U,
        0x00000000U
    };

    ssize_t lReadBytes = 0;

    /* Receive the CAN message over IP */
    while(lErrorCode == CAN_IP_ERROR_NONE) {
        lErrorCode = CIP_recvMsgStruct(0U, &lMsg, &lReadBytes);
        if(0 < lReadBytes && sizeof(cipMessage_t) == lReadBytes) {
            CIP_printMessageShort(&lMsg);
        }
    }

    if(CAN_IP_ERROR_NONE != lErrorCode) {
        printf("[ERROR] CIP_recvMsgStruct failed w/ error code %u.\n", lErrorCode);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
