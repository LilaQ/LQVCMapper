cmake_minimum_required(VERSION 3.15)
project(ViGEmService)

set(CMAKE_CXX_STANDARD 17)

add_executable(service service.cpp controller_manager.cpp keyboard_input.cpp named_pipe_ipc.cpp)
add_executable(cli_client cli_client.cpp named_pipe_ipc.cpp)

target_include_directories(service PRIVATE . ./nlohmann_json/include)
target_include_directories(cli_client PRIVATE . ./nlohmann_json/include)

target_link_libraries(service ViGEmClient setupapi)
target_link_libraries(cli_client setupapi)