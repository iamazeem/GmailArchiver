cmake_minimum_required(VERSION 3.10)

project(GmailArchiver VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(GmailArchiver ${SOURCES})

set(CMAKE_PREFIX_PATH /usr/lib/x86_64-linux-gnu/cmake/)
find_package(Boost REQUIRED system)
find_package(cpprestsdk REQUIRED)

target_link_libraries(GmailArchiver PRIVATE Boost::system)
target_link_libraries(GmailArchiver PRIVATE cpprestsdk::cpprest)
