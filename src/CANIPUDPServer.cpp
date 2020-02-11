/**
 * @brief CAN over IP UDP server class implementation
 * 
 * @file CANIPUDPServer.cpp
 */

/* Includes -------------------------------------------- */
#include "CANIPUDPServer.hpp"
#include "CANIPExceptions.hpp"

#include "can_ip_error_codes.h"

/* C++ system */
#include <string>
#include <iostream>

/* C system */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/* errno */
#include <errno.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip {
    /* CANIPUDPServer class implementation --------------- */

    /* Contructor */
    CANIPUDPServer::CANIPUDPServer(const std::string &pAddr, const int &pPort, const int &pFamily, const std::string *pMultiCastAddr) :
        CANIPUDPBase(pAddr, pPort, pFamily)
    {
        /* Bind to the very first address */
        errno = 0;
        int lResult = bind(mSocket, mAddrInfo->ai_addr, mAddrInfo->ai_addrlen);
        if(0 != lResult) {
            /* 
             * We need to reverse the address form mAddrInfo 
             * to know exactly which one was picked
             */
            char lAddrBuffer[256U];
            switch(mAddrInfo->ai_family) {
                case AF_INET:
                    inet_ntop(AF_INET, 
                        &reinterpret_cast<struct sockaddr_in *>(mAddrInfo->ai_addr)->sin_addr, 
                        lAddrBuffer, 
                        sizeof(lAddrBuffer));
                    break;
                case AF_INET6:
                    inet_ntop(AF_INET6, 
                        &reinterpret_cast<struct sockaddr_in6 *>(mAddrInfo->ai_addr)->sin6_addr, 
                        lAddrBuffer, 
                        sizeof(lAddrBuffer));
                    break;
                default:
                    strncpy(lAddrBuffer, "Unknown Adress Family", sizeof(lAddrBuffer));
                    break;
            }

            std::cerr << "[ERROR] <CANIPUDPServer::CANIPUDPServer> bind failed !" << std::endl;
            std::cerr << "        errno = " << errno << " (" << strerror(errno) << ")" << std::endl;
            
            throw CANIPRuntimeException("[ERROR] Could not bind UDP socket with \"" + mAddr + ":" + std::to_string(mPort) + "\"");
        }

        /* Are we creating a server to listen to multicast packets ? */
        if(nullptr != pMultiCastAddr) {
            struct ip_mreqn lMreq;

            /* For the getaddrinfo() function, convert the port to a C string */
            const char *lPortCStr = std::to_string(mPort).c_str();

            /* Definition fo the getaddrinfo() hints parameters */
            struct addrinfo lHints;
            lHints.ai_family   = pFamily;
            lHints.ai_socktype = SOCK_DGRAM;
            lHints.ai_protocol = IPPROTO_UDP;

            /* We are using a multicast address, 
             * but the same port as for the other address
             */

            /* Retrieve the list of addresses defined by getaddrinfo() */
            struct addrinfo *lInfo = nullptr;
            errno = 0;
            lResult = getaddrinfo(pMultiCastAddr->c_str(), lPortCStr, &lHints, &lInfo);
            if(0 != lResult || nullptr == lInfo) {
                if(0 != errno) {
                    throw CANIPParameterException(("[ERROR] <CANIPUDPServer::CANIPUDPServer> getaddrinfo : Invalid address (" + *pMultiCastAddr + ") or port (" + lPortCStr + ") !").c_str(), 
                        ("errno = " + std::to_string(errno) + " (" + strerror(errno) + ")").c_str());
                } else {
                    throw CANIPParameterException(("[ERROR] <CANIPUDPServer::CANIPUDPServer> getaddrinfo : Invalid address (" + *pMultiCastAddr + ") or port (" + lPortCStr + ") !").c_str());
                }
            }

            /* Check if both addresses have the same size */
            if(lInfo->ai_addrlen != sizeof(lMreq.imr_multiaddr)
                || mAddrInfo->ai_addrlen != sizeof(lMreq.imr_multiaddr))
            {
                throw CANIPRuntimeException("[ERROR] <CANIPUDPServer::CANIPUDPServer> Invalid address or multicast address type for UDP mutlicast",
                    mAddr + ":" + lPortCStr + "\" or \"" + *pMultiCastAddr + ":" + lPortCStr + "\"");
            }

            /* Copy multicast data */
            memcpy(&lMreq.imr_multiaddr, lInfo->ai_addr->sa_data,     sizeof(lMreq.imr_multiaddr));
            memcpy(&lMreq.imr_address,   mAddrInfo->ai_addr->sa_data, sizeof(lMreq.imr_address));
            lMreq.imr_ifindex = 0; /* No specific interface */

            /* Set the socket options : IP_ADD_MEMBERSHIP */
            errno = 0;
            lResult = setsockopt(mSocket, IPPROTO_UDP, IP_ADD_MEMBERSHIP, &lMreq, sizeof(lMreq));
            if(0 != lResult) {
                if(0 != errno) {
                    throw CANIPParameterException("[ERROR] <CANIPUDPServer::CANIPUDPServer> setsockopt IP_ADD_MEMBERSHIP failed !", 
                        ("errno = " + std::to_string(errno) + " (" + strerror(errno) + ")").c_str());
                } else {
                    throw CANIPParameterException("[ERROR] <CANIPUDPServer::CANIPUDPServer> setsockopt IP_ADD_MEMBERSHIP failed !");
                }
            }

#ifdef IP_MULTICAST_ALL
            /* Set the socket options : IP_ADD_MEMBERSHIP */
            errno = 0;
            lResult = setsockopt(mSocket, IPPROTO_UDP, IP_MULTICAST_ALL, &lMreq, sizeof(lMreq));
            if(0 != lResult) {
                if(0 != errno) {
                    throw CANIPParameterException("[ERROR] <CANIPUDPServer::CANIPUDPServer> setsockopt IP_MULTICAST_ALL failed !", 
                        ("errno = " + std::to_string(errno) + " (" + strerror(errno) + ")").c_str());
                } else {
                    throw CANIPParameterException("[ERROR] <CANIPUDPServer::CANIPUDPServer> setsockopt IP_MULTICAST_ALL failed !");
                }
            }
#endif /* IP_MULTICAST_ALL */
        }
    }

    /* Destructor */
    CANIPUDPServer::~CANIPUDPServer() {
        /* Empty */
    }

    /* Receive */
}
