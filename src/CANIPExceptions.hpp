/**
 * @brief CAN over IP exceptions
 * 
 * @file CANIPExceptions.hpp
 */

#ifndef CANIP_CANIPEXCEPTIONS_HPP
#define CANIP_CANIPEXCEPTIONS_HPP

/* Includes -------------------------------------------- */

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* Extern variables ------------------------------------ */

namespace canip
{
    class CANIPRuntimeException : public std::runtime_error
    {
        public:
            CANIPRuntimeException(const std::string &pErrMsg) : std::runtime_error(pErrMsg) {
                /* Empty */
            }

            CANIPRuntimeException(const std::string &pErrMsg1, const std::string &pErrMsg2) : 
                std::runtime_error(pErrMsg1 + "\n        " + pErrMsg2) {
                /* Empty */
            }
    };

    class CANIPParameterException : public std::logic_error
    {
        public:
            CANIPParameterException(const std::string & errmsg) : std::logic_error(errmsg) {
                /* Empty */
            }

            CANIPParameterException(const std::string &pErrMsg1, const std::string &pErrMsg2) : 
                std::logic_error(pErrMsg1 + "\n        " + pErrMsg2) {
                /* Empty */
            }
    };
}

#endif /* CANIP_CANIPEXCEPTIONS_HPP */
