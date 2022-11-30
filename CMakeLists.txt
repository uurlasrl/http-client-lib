message(STATUS "***** init http_client *****")

cmake_minimum_required(VERSION 3.18)
project(http-client-lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(http-client-lib src/http-client/http_errors.cpp)


if(IDE_CLION)
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@3)
    set(OPENSSL_LIBRARIES /usr/local/opt/openssl@3/lib)
endif()
find_package(OpenSSL REQUIRED)
find_package(gflags 2.2.2 REQUIRED)
find_package (glog 0.7.0 REQUIRED)
FIND_PACKAGE(Boost 1.79 COMPONENTS REQUIRED date_time system filesystem thread graph)

target_link_libraries(http-client-lib PRIVATE "${Boost_LIBRARIES}" glog::glog OpenSSL::SSL)

target_include_directories(http-client-lib PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(http-client-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")



message(STATUS "BUILD_TESTING=${BUILD_TESTING}")
message(STATUS "ENABLE_TESTS=${ENABLE_TESTS}")
message(STATUS "CMAKE_TESTING_ENABLED=${CMAKE_TESTING_ENABLED}")
if(CMAKE_TESTING_ENABLED AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/tests/CMakeLists.txt )
    message(STATUS "** testing for http_client is including")
    add_subdirectory(tests)
endif()
message(STATUS "***** end http_client *****")