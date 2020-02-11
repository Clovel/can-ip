/**
 * @brief CAN over IP UDP base class implementation
 * 
 * @file CANIPUDPBase.cpp
 */

/* Includes -------------------------------------------- */
#include "CANIPUDPBase.hpp"
#include "CANIPExceptions.hpp"

/* C++ system */
#include <string>
#include <iostream>

/* C system */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

/* errno */
#include <errno.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip {
    /* CANIPUDPBase class implementation --------------- */

    /* Contructor */
    CANIPUDPBase::CANIPUDPBase(const std::string &pAddr, const int &pPort, const int &pFamily) :
        mPort(pPort),
        mAddr(pAddr)
    {
        /* Check if the address is an empty string */
        if(mAddr.empty()) {
            throw CANIPParameterException("[ERROR] <CANIPUDPBase::CANIPUDPBase> Empty IP address given !");
        }

        /* Check if the port is between 0 and 65535 */
        if(0 > mPort || 65535 < mPort) {
            throw CANIPParameterException("[ERROR] <CANIPUDPBase::CANIPUDPBase> Port value is out of bounds !");
        }

        /* For the getaddrinfo() function, convert the port to a C string */
        const char *lPortCStr = std::to_string(mPort).c_str();

        /* Definition fo the getaddrinfo() hints parameters */
        struct addrinfo lHints;
        lHints.ai_family   = pFamily;
        lHints.ai_socktype = SOCK_DGRAM;
        lHints.ai_protocol = IPPROTO_UDP;

        /* Retrieve the list of addresses defined by getaddrinfo() */
        struct addrinfo *lInfo = nullptr;

        errno = 0;
        const int lGetAddrInfoResult = getaddrinfo(mAddr.c_str(), lPortCStr, &lHints, &lInfo);
        if(0 != lGetAddrInfoResult || nullptr == lInfo) {
            if(0 != errno) {
                throw CANIPParameterException(("[ERROR] <CANIPUDPBase::CANIPUDPBase> getaddrinfo : Invalid address (" + mAddr + ") or port (" + lPortCStr + ") !").c_str(), 
                    ("errno = " + std::to_string(errno) + " (" + strerror(errno) + ")").c_str());
            } else {
                throw CANIPParameterException(("[ERROR] <CANIPUDPBase::CANIPUDPBase> getaddrinfo : Invalid address (" + mAddr + ") or port (" + lPortCStr + ") !").c_str());
            }
        }

        mAddrInfo = lInfo;

        /* Create the UDP socket */
        errno = 0;
        if(0 > (mSocket = socket(mAddrInfo->ai_family, SOCK_DGRAM, IPPROTO_UDP))) {
            freeaddrinfo(mAddrInfo);
            if(0 != errno) {
                throw CANIPParameterException("[ERROR] <CANIPUDPBase::CANIPUDPBase> socket failed !", 
                    ("errno = " + std::to_string(errno) + " (" + strerror(errno) + ")").c_str());
            } else {
                throw CANIPParameterException("[ERROR] <CANIPUDPBase::CANIPUDPBase> socket failed !\n");
            }
        }
    }

    CANIPUDPBase::~CANIPUDPBase() {
        freeaddrinfo(mAddrInfo);
        close(mSocket);
    }

    int CANIPUDPBase::getSocket(void) const {
        return mSocket;
    }

    int CANIPUDPBase::getPort(void) const {
        return mPort;
    }

    std::string CANIPUDPBase::getAddr(void) const {
        return mAddr;
    }

    /* TODO : Get MTU size & get MSS size */
}