/**
 * @brief CAN over IP UDP client class definition
 * 
 * @file CANIPUDPClient.hpp
 */

#ifndef CANIP_CANIPUDPCLIENT_HPP
#define CANIP_CANIPUDPCLIENT_HPP

/* Includes -------------------------------------------- */
#include "CANIPUDPBase.hpp"

#include <string>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip {
    /* CANIPClient class ------------------------------- */
    class CANIPUDPClient : public CANIPUDPBase  {
        public:
            /* Constructor */
            CANIPUDPClient(const std::string &pAddr, const int &pPort, const int &pFamily = AF_UNSPEC);

            /* Destructor */
            ~CANIPUDPClient();

            /* Send raw frame */
            int sendRaw(const char * const pMsg, const size_t &pSize);
    };
}

#endif /* CANIP_CANIPUDPCLIENT_HPP */
