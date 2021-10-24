project (spyware)

set ( SOURCES 
    main.cpp 
    amqp/AmqpConnection.cpp 
    amqp/AmqpChannel.cpp
    core/utils.cpp
    core/authentication.cpp
)

add_executable (${PROJECT_NAME} ${SOURCES})

target_link_libraries (${PROJECT_NAME} 
    nlohmann_json::nlohmann_json
    rabbitmq::rabbitmq-static
)