/**
 * @brief CAN over IP basic sender example
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
#include <unistd.h>

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
    const cipMessage_t lMsg = {
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
    
    /* Send the CAN message over IP */
    while(CAN_IP_ERROR_NONE == lErrorCode) {
        if(1U != (lErrorCode = CIP_send(0U, lMsg.id, lMsg.size, lMsg.data, lMsg.flags))) {
            printf("[ERROR] CIP_send failed w/ error code %u.\n", lErrorCode);
            exit(EXIT_FAILURE);
        }

        CIP_printMessageShort(&lMsg);

        sleep(1U);
    }

    return EXIT_SUCCESS;
}
