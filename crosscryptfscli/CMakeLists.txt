cmake_minimum_required(VERSION 3.12)

add_executable(crosscryptfscli
  src/main.cpp
)

target_link_libraries(crosscryptfscli PRIVATE crosscryptfs)

target_compile_features(crosscryptfscli PRIVATE cxx_std_17)

include_directories(
  ${crosscryptfs_SOURCE_DIR} 
  include
)

install(TARGETS crosscryptfscli
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
)
