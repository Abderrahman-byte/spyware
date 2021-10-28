cmake_minimum_required(VERSION 3.1)

project (spyware VERSION 0.9)

set(VERSION "0.9")

# Check variables
if (NOT DEFINED RMQ_HOST) 
    message (FATAL_ERROR "RMQ_HOST Variable is missing")
endif()

if (NOT DEFINED RMQ_USER) 
    message (FATAL_ERROR "RMQ_USER Variable is missing")
endif()

if (NOT DEFINED RMQ_VHOST) 
    message (FATAL_ERROR "RMQ_VHOST Variable is missing")
endif()

if (NOT DEFINED RMQ_PASSWORD) 
    message (FATAL_ERROR "RMQ_PASSWORD Variable is missing")
endif()

if (NOT DEFINED AUTH_USER) 
    message (FATAL_ERROR "AUTH_USER Variable is missing")
endif()

if (NOT DEFINED AUTH_PASSWORD) 
    message (FATAL_ERROR "AUTH_PASSWORD Variable is missing")
endif()

if (NOT DEFINED DEBUG) 
    set (DEBUG 0)
endif()

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in config.h)

add_subdirectory (json)

add_subdirectory (rabbitmq-c)

add_subdirectory (src)

target_include_directories (${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})