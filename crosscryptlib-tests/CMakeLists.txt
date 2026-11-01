
cmake_minimum_required(VERSION 3.16)

include(CTest)


# Tests
function(add_test_executable name)
    set(TEST_NAME "test-${name}")

    # Create the test executable
    add_executable("${TEST_NAME}" "src/${name}.c" "src/test.h")
    target_include_directories("${TEST_NAME}" PRIVATE "${PROJECT_SOURCE_DIR}/crosscryptlib/include")

    # Link the library to the test executable
    add_dependencies("${TEST_NAME}" "crosscryptlib")

    # Set the test executable name
    string(REPLACE "_" "/" TEST_EXECUTABLE_NAME ${name}) # Replace underscores with slashes in name
    add_test(NAME ${TEST_EXECUTABLE_NAME} COMMAND "${TEST_NAME}") # Add test command

    # Set the test executable properties if needed
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        # Add coverage flags for GCC/Clang
        target_compile_options("${TEST_NAME}" PRIVATE --coverage -lm -w) # Link with math library and suppress warnings
        target_link_options("${TEST_NAME}" PRIVATE --coverage)
    endif()

    # Set the output directory for the test executable
    set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests/
    )
endfunction()

# Find Test Files
file(GLOB TESTS "src/*.c")

foreach(TEST_FILE ${TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Call the preceding function to add the test executable
    add_test_executable(${TEST_NAME})
endforeach()

