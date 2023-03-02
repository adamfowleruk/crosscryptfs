cmake_minimum_required(VERSION 3.12)

project(crosscryptfs VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
# Include the sanitizer module
# include(code-coverage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
add_definitions(-DCROSSCRYPTFS_LOG_LEVEL=4)
#ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

include(GNUInstallDirs)

add_subdirectory(crosscryptfs) 
add_subdirectory(crosscryptfscli)