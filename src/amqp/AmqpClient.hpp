#include <string>
#include <mutex>

#include <rabbitmq-c/amqp.h>
#include <nlohmann/json.hpp>

#ifndef _SPYWARE_AMQPCLIENT_HPP_
#define _SPYWARE_AMQPCLIENT_HPP_ 1
class AmqpClient {
    private :
        amqp_connection_state_t connection = NULL;
        amqp_socket_t* socket = NULL;

        std::string host = "localhost";
        std::string vhost = "/";
        std::string user = "guest";
        std::string password = "guest";
        int port = 5672;
        bool alive = false;

        std::mutex mtx;

    public :
        AmqpClient (nlohmann::json);
        ~AmqpClient ();
        
        bool open ();
        void close ();
        std::string declareQueue (std::string, bool = false, bool = false, bool = false, bool = false, amqp_table_t = amqp_empty_table);
        bool bindQueue (std::string, std::string, std::string);
        bool basicPublish (std::string, std::string, std::string, amqp_basic_properties_t* = NULL);
        bool basicPublish (std::string, std::string, char*, int, amqp_basic_properties_t* = NULL);
        void basicConsume (std::string);
        amqp_rpc_reply_t consumeMessage (amqp_envelope_t *, int = 0);
        void basicAck(uint64_t);
        void deleteQueue (std::string);

    private :
        void checkIfAlive ();
};

#endif