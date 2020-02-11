/**
 * @brief CAN over IP UDP base class implementation
 * 
 * @file CANIPUDPBase.cpp
 */

/* Includes -------------------------------------------- */
#include "CANIPUDPClient.hpp"
#include "CANIPExceptions.hpp"

#include "can_ip_error_codes.h"

/* C++ system */
#include <string>
#include <iostream>

/* C system */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* errno */
#include <errno.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip {
    /* CANIPUDPClient class implementation --------------- */

    /* Contructor */
    CANIPUDPClient::CANIPUDPClient(const std::string &pAddr, const int &pPort, const int &pFamily) :
        CANIPUDPBase(pAddr, pPort, pFamily)
    {
        /* Empty */
    }

    /* Destructor */
    CANIPUDPClient::~CANIPUDPClient() {
        /* Empty */
    }

    /* Send */
    int CANIPUDPClient::sendRaw(const char * const pMsg, const size_t &pSize) {
        ssize_t lSentBytes = 0;

        errno = 0;
        lSentBytes = sendto(mSocket, (const void *)pMsg, pSize, 0, mAddrInfo->ai_addr, mAddrInfo->ai_addrlen);
        if(sizeof(pMsg) != lSentBytes) {
            std::cerr << "[ERROR] <CANIPUDPClient::sendRaw> sendto failed !" << std::endl;
            if(0 != errno) {
                std::cerr << "        errno = " << errno << " (" << strerror(errno) << ")" << std::endl;
            }
            return CAN_IP_ERROR_NET;
        }

        return CAN_IP_ERROR_NONE;
    }
}
