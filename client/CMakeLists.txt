cmake_minimum_required(VERSION "4.2.3")

set(CMAKE_CXX_STANDART 11)

add_executable(client
    main.cpp
    Client.cpp
    Client.h
)

target_link_libraries(client ws2_32)