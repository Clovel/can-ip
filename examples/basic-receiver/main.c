/**
 * @brief CAN over IP basic sender example
 * 
 * @file main.c
 */

/* Includes -------------------------------------------- */
#include "can_ip.h"

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
    if(1U != (lErrorCode = CIP_init(0U, CAN_IP_MODE_NORMAL))) {
        printf("[ERROR] CIP_init failed w/ error code %u.\n", lErrorCode);
        exit(EXIT_FAILURE);
    }

    /* TODO */

    return EXIT_SUCCESS;
}