cmake_minimum_required(VERSION 3.10)
project(serverweb C)

# Ejecutables TCP
add_executable(server src/server.c src/tcp.c src/hash.c)
add_executable(client src/client.c src/tcp.c src/hash.c)

include_directories(include)

# Agregar biblioteca OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(server OpenSSL::SSL)
target_link_libraries(client OpenSSL::SSL)




