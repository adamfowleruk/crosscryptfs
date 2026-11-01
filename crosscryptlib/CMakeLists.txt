cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
#set(CMAKE_C_EXTENSIONS ON)

include(GNUInstallDirs)

include_directories (
        include
)

set(HEADERS
#        include/context.h
        include/crosscryptfs.h
)

add_library(crosscryptlib
        ${HEADERS}

#        src/context.c
        src/crosscryptfs.c
)

set_target_properties(crosscryptlib PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_include_directories(crosscryptlib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(crosscryptlib PRIVATE c_std_23)

add_compile_options(-std=c23 -Wl -Wno-unused-function)

add_link_options(-std=c23)

# Use the DEV location (not system location) to pick up OpenSSL v3
# see .vscode/settings.json for CMake path overrides
find_package(OpenSSL 4.0.1 REQUIRED)
target_link_libraries(crosscryptlib OpenSSL::Crypto)

install(TARGETS crosscryptlib
#        EXPORT crosscryptlib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/crosscryptlib
)

#install(EXPORT crosscryptlib
#        NAMESPACE crosscryptlib::
#        DESTINATION ${CMAKE_INSTALL_LIBDIR}/crosscryptlib
#)
