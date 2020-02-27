/**
 * @brief CAN over IP sender receiver example
 * 
 * @file main.c
 */

/* Includes -------------------------------------------- */
#include "can_ip.h"
#include "can_ip_error_codes.h"

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
    if(1U != (lErrorCode = CIP_init(0U, CAN_IP_MODE_NORMAL, 15024))) {
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
        0x00000000U
    };

    CIP_printMessage(&lMsg);

    printf("[DEBUG] sizeof(cipMessage_t) = %lu\n", sizeof(cipMessage_t));
    /* Send the CAN message over IP */
    if(1U != (lErrorCode = CIP_send(0U, &lMsg))) {
        printf("[ERROR] CIP_send failed w/ error code %u.\n", lErrorCode);
        exit(EXIT_FAILURE);
    }

    /* reset the message structure before receiving */
    memset(&lMsg, 0, sizeof(cipMessage_t));

    ssize_t lReadBytes = 0;

    /* Receive the CAN message over IP */
    while(lErrorCode == CAN_IP_ERROR_NONE && 0 >= lReadBytes) {
        lErrorCode = CIP_recv(0U, &lMsg, &lReadBytes);
    }

    if(CAN_IP_ERROR_NONE != lErrorCode) {
        printf("[ERROR] CIP_recv failed w/ error code %u.\n", lErrorCode);
        exit(EXIT_FAILURE);
    }

    CIP_printMessage(&lMsg);

    return EXIT_SUCCESS;
}
