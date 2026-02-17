cmake_minimum_required(VERSION "4.2.3")

set(CMAKE_CXX_STANDART 11)

add_executable(server
    main.cpp
    Server.cpp
    Server.h
    TCP.cpp
    TCP.h
    ITransport.h
)

target_link_libraries(server ws2_32)