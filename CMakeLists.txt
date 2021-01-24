cmake_minimum_required(VERSION 3.17)
project(SO_LinkedList)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

include_directories(.)

#file(GLOB_RECURSE SOURCE_FILES "SO_LinkedList/*.cpp" "SO_LinkedList/*.h")

add_executable(SO_LinkedList
        main.cpp
        SONode.cpp
        SONode.h)

#add_executable(SO_LinkedList ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(SO_LinkedList  ${PQXX_LIB} ${PQ_LIB})