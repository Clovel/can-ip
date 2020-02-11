/**
 * @brief CAN over IP UDP server class definition
 * 
 * @file CANIPUDPServer.hpp
 */

#ifndef CANIP_CANIPUDPSERVER_HPP
#define CANIP_CANIPUDPSERVER_HPP

/* Includes -------------------------------------------- */
#include "CANIPUDPBase.hpp"

#include <string>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip {
    /* CANIPServer class ------------------------------- */
    class CANIPUDPServer : public CANIPUDPBase {
        public:
            /* Constructor */
            CANIPUDPServer(const std::string &pAddr, const int &pPort, const int &pFamily = AF_UNSPEC, const std::string *pMultiCastAddr = nullptr);

            /* Destructor */
            ~CANIPUDPServer();

            /* Send raw frame */
            int recv(char *pMsg, const size_t &pMaxSize);
            int timedRecv(char *pMsg, const size_t &pMaxSize, const int &pMaxWaitMs);
            std::string timedRecv(const int pBufferSize, const int pMaxWaitMs);
    };
}

#endif /* CANIP_CANIPUDPSERVER_HPP */
