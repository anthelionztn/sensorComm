cmake_minimum_required(VERSION 3.13)
project(sensorComm C)

set(CMAKE_C_STANDARD 99)

add_executable(sensorComm main.c sensorComm.h sensorComm.c)