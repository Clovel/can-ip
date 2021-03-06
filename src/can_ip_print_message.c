/**
 * @brief CAN over IP API print message functions
 * 
 * @file can_ip_print_message.c
 */

/* Includes -------------------------------------------- */
#include "can_ip.h"

#include <stdio.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Prnt message function ------------------------------- */
void CIP_printMessage(const cipMessage_t * const pMsg) {
    printf("CAN-IP Message :\n");
    printf("\tID    : 0x%03X\n", pMsg->id);
    printf("\tSize  : %u\n", pMsg->size);
    printf("\tFlags : 0x%08X\n", pMsg->flags);
    if(0 < pMsg->size) {
        printf("\tData  : ");
        for(uint8_t i = 0U; (i < pMsg->size) && (i < CAN_MESSAGE_MAX_SIZE); ++i) {
            printf("%02X ", pMsg->data[i]);
        }
        printf("\n");
    }
}


void CIP_printMessageShort(const cipMessage_t * const pMsg) {
    printf("0x%03X [%u] ", pMsg->id, pMsg->size);
    for(uint8_t i = 0U; (i < pMsg->size) && (i < 8U); i++) {
        printf("%02X ", pMsg->data[i]);
    }
    printf("0x%08X\n", pMsg->flags);
}
