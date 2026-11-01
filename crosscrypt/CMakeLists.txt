cmake_minimum_required(VERSION 3.12)

add_executable(crosscrypt
  src/main.c
)

add_link_options(-std=c23)

target_link_libraries(crosscrypt PRIVATE crosscryptlib)

target_compile_features(crosscrypt PRIVATE c_std_23)

add_compile_options(-std=c23 -Wl -Wno-unused-function)


include_directories(
  ${crosscryptlib_SOURCE_DIR} 
  include
)

install(TARGETS crosscrypt
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
)
