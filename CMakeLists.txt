cmake_minimum_required(VERSION 3.10)
project(serverweb C)

# Ejecutables TCP
add_executable(server src/server.c src/tcp.c)

include_directories(include)






