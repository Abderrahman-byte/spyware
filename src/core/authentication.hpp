#include "../amqp/AmqpChannel.hpp"

#ifndef _SPYWARE_AUTHENTICATION_HPP_
#define _SPYWARE_AUTHENTICATION_HPP_ 1

extern std::string authenticate(AmqpChannel &, std::string, std::string);

#endif