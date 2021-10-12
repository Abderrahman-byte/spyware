project (spware)

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries (${PROJECT_NAME} 
    nlohmann_json::nlohmann_json
)