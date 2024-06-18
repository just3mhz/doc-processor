cmake_minimum_required(VERSION 3.11)

project(document_processor CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.83.0 REQUIRED)
find_package(libpqxx)

add_library(
    document_processor_lib
    processor.h
    processor.cpp
    document.h
    storage.h
    storage.cpp
)

target_include_directories(document_processor_lib PRIVATE boost::boost)
target_link_libraries(document_processor_lib PRIVATE boost::boost libpqxx::pqxx)

add_executable(
    document_processor
    main.cpp
)

target_link_libraries(document_processor PRIVATE document_processor_lib libpqxx::pqxx)
