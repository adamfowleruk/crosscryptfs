cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(GNUInstallDirs)

set(HEADERS 
	include/context.h
	include/crosscryptfs.h
	include/exporter.h
	include/importer.h
	include/io.h
	include/printer.h
	include/encryption.h
	include/noencryption.h
	include/ecies.h
	include/volume.h
	include/filesystemvolume.h
)

add_library(crosscryptfs 
	${HEADERS}

    src/context.cpp
    src/crosscryptfs.cpp
    src/exporter.cpp
    src/importer.cpp
	src/io.cpp
    src/printer.cpp
	src/encryption.cpp
	src/noencryption.cpp
	src/ecies.cpp
	src/volume.cpp
	src/filesystemvolume.cpp
)

set_target_properties(crosscryptfs PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_include_directories(crosscryptfs 
	PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_features(crosscryptfs PRIVATE cxx_std_17)

add_compile_options(-Wl,-Wno-unused-function)

# Use the DEV location (not system location) to pick up OpenSSL v3
# see .vscode/settings.json for CMake path overrides
find_package(OpenSSL 3.0.2 REQUIRED)
target_link_libraries(crosscryptfs OpenSSL::Crypto)

install(TARGETS crosscryptfs 
    EXPORT crosscryptfs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}  
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/crosscryptfs
)

install(EXPORT crosscryptfs 
	NAMESPACE crosscryptfs::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/crosscryptfs
)
