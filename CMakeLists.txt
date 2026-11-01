cmake_minimum_required(VERSION 3.21)

project(crosscryptfs VERSION 0.1.0 LANGUAGES C)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
# Include the sanitizer module
# include(code-coverage)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
add_definitions(-DCROSSCRYPTFS_LOG_LEVEL=4)
#ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

include(GNUInstallDirs)

add_subdirectory(crosscryptlib)
enable_testing()
add_subdirectory(crosscryptlib-tests)
add_subdirectory(crosscrypt)

#add_subdirectory(crosscryptfs)
#add_subdirectory(crosscryptfs-tests)
#add_subdirectory(crosscryptfscli)

#include(Doxygen)
#Doxygen(crosscryptfs/include doxygen)