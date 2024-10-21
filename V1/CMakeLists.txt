cmake_minimum_required(VERSION 3.10)
project(AssemblerProject)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Compile in Debug Mode
set(CMAKE_BUILD_TYPE Debug)

# Add compiler flags for strict C89 compliance
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -std=c89
        -w
        -pg
        #-pedantic
        #-W
        -Wall
        -Wpointer-arith
        -Wwrite-strings
        -g
        -O0
    )
endif()

# Set source directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include ${SRC_DIR}/parser ${SRC_DIR}/tokenizer ${SRC_DIR}/logger ${SRC_DIR}/decoder)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# Include directories for header files
include_directories(${INCLUDE_DIR})

# Source files for the main application
set(SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/overlord.c
    ${SRC_DIR}/decoder/decoder.c
    ${SRC_DIR}/logger/logger.c
    ${SRC_DIR}/parser/parser.c
    ${SRC_DIR}/tokenizer/tokenizer.c
    ${SRC_DIR}/common_ds.c
)

# Create an executable for the main application
add_executable(Assembler ${SOURCES})

# Enable testing
enable_testing()

# Test executables for each module
add_executable(test_decoder ${TEST_DIR}/test_decoder.c ${SRC_DIR}/decoder/decoder.c ${SRC_DIR}/common_ds.c)
add_executable(test_logger ${TEST_DIR}/test_logger.c ${SRC_DIR}/logger/logger.c ${SRC_DIR}/common_ds.c)
add_executable(test_parser ${TEST_DIR}/test_parser.c ${SRC_DIR}/parser/parser.c ${SRC_DIR}/tokenizer/tokenizer.c ${SRC_DIR}/common_ds.c)
add_executable(test_overlord ${TEST_DIR}/test_overlord.c ${SRC_DIR}/overlord.c ${SRC_DIR}/decoder/decoder.c ${SRC_DIR}/parser/parser.c ${SRC_DIR}/logger/logger.c ${SRC_DIR}/common_ds.c)
add_executable(test_ds ${TEST_DIR}/test_ds.c ${SRC_DIR}/common_ds.c)
add_executable(test_tokenizer ${TEST_DIR}/test_tokenizer.c ${SRC_DIR}/tokenizer/tokenizer.c)

# Add tests
add_test(NAME DecoderTest COMMAND test_decoder)
add_test(NAME LoggerTest COMMAND test_logger)
add_test(NAME ParserTest COMMAND test_parser)
add_test(NAME TokenizerTest COMMAND test_tokenizer)
add_test(NAME OverlordTest COMMAND test_overlord)
add_test(NAME DataStructureTest COMMAND test_ds)

# Link necessary libraries (if needed)
# For example: target_link_libraries(Assembler <libraries>)

# Optionally, set compilation flags for warnings
target_compile_options(Assembler PRIVATE -Wall -Wextra -Werror)

