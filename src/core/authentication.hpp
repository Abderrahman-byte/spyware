#include "../amqp/AmqpClient.hpp"

#ifndef _SPYWARE_AUTHENTICATION_HPP_
#define _SPYWARE_AUTHENTICATION_HPP_ 1

extern std::string authenticate(AmqpClient &, std::string, std::string);

#endif