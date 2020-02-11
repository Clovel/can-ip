/**
 * @brief CAN over IP UDP base class definition
 * 
 * @file CANIPUDPBase.hpp
 */

#ifndef CANIP_CANIPUDPBASE_HPP
#define CANIP_CANIPUDPBASE_HPP

/* Includes -------------------------------------------- */
/* C++ system */
#include <string>

/* C System */
#include <netdb.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip {
    /* CANIPUDPBase class ------------------------------ */
    class CANIPUDPBase {
        public:
            /* Getters */
            int getSocket(void) const;
            int getPort(void) const;
            std::string getAddr(void) const;

        protected:
            /* Constructor */
            CANIPUDPBase(const std::string &pAddr, const int &pPort, const int &pFamily);
            
            /* Destructor */
            virtual ~CANIPUDPBase();

            int              mSocket;
            int              mPort;
            std::string      mAddr;
            struct addrinfo *mAddrInfo;
    };
}

#endif /* CANIP_CANIPUDPBASE_HPP */
