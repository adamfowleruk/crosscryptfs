cmake_minimum_required(VERSION 3.12)

find_package(Catch2 3 REQUIRED)

add_executable(crosscryptfs-tests
  # Test templates
	# test-templates.h
	# test-templates.cpp
	# test-util.h 

	# Catch all to ensure all files in Herald are included in code coverage reports
	# codecoverage-tests.cpp

	# Base functionality
	filesystemvolume-tests.cpp

	# Encryption
	ecies-tests.cpp

	# High level utilities
	importer-tests.cpp
)

include_directories(${crosscryptfs_SOURCE_DIR})

target_link_libraries(crosscryptfs-tests PRIVATE Catch2::Catch2WithMain crosscryptfs)

target_compile_features(crosscryptfs-tests PRIVATE cxx_std_17)
