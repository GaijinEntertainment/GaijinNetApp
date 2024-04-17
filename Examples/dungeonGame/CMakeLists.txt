cmake_minimum_required(VERSION 3.4.0)
project(dungeonGame VERSION 0.1.0)

set(SOURCES
    ./main.cpp
    ./gameClient.cpp
    ./server.cpp
    ./gapApi/requestHelpers.cpp
    ./gapApi/gapApi.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
add_executable(dungeonGame ${SOURCES})

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
