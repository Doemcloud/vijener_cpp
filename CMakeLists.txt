cmake_minimum_required(VERSION 3.22)
project(MagicSquar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8")

add_executable(MagicSquar
        vernam.cpp
        spek.cpp
        DES.cpp
        hellman/hellman.cpp
        hellman/Point.h
        hellman/EllipticCurve.cpp
        hellman/ECDHParticipant.cpp
        hellman/ECDHParticipant.h
        hellman/main.cpp)