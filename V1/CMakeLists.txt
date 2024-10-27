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
        -W
        -Wall
        -Wpointer-arith
        -Wwrite-strings
        -g
        -O0
    )
endif()

# Set source directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# Include directories for header files
include_directories(${INCLUDE_DIR})

# Source files for the main application

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
        #-w
        -pedantic
        -Wall
        -Wpointer-arith
        -Wwrite-strings
        -g
        -O0
    )
endif()

# Set source directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# Include directories for header files
include_directories(${INCLUDE_DIR})

# Create libraries for each module
add_library(tokenizer_lib ${SRC_DIR}/tokenizer/tokenizer.c)
target_include_directories(tokenizer_lib PUBLIC ${INCLUDE_DIR})
add_library(parser_lib ${SRC_DIR}/parser/parser.c ${SRC_DIR}/tokenizer/tokenizer.c ${SRC_DIR}/common_ds.c)
target_include_directories(parser_lib PUBLIC ${INCLUDE_DIR})
add_library(decoder_lib ${SRC_DIR}/decoder/decoder.c ${SRC_DIR}/common_ds.c)
target_include_directories(decoder_lib PUBLIC ${INCLUDE_DIR})
add_library(logger_lib ${SRC_DIR}/logger/logger.c ${SRC_DIR}/common_ds.c ${SRC_DIR}/parser/parser.c ${SRC_DIR}/tokenizer/tokenizer.c ${SRC_DIR}/decoder/decoder.c)
target_include_directories(logger_lib PUBLIC ${INCLUDE_DIR})

# Create the main executable
add_executable(Assembler ${SRC_DIR}/main.c ${SRC_DIR}/overlord.c ${SRC_DIR}/common_ds.c)

# Link libraries to the main executable
target_link_libraries(Assembler PRIVATE
    decoder_lib
    logger_lib
    parser_lib
    tokenizer_lib
)

target_include_directories(Assembler PUBLIC ${INCLUDE_DIR})

# Enable testing
enable_testing()

# Test executables for each module
add_executable(test_decoder ${TEST_DIR}/test_decoder.c)
add_executable(test_logger ${TEST_DIR}/test_logger.c)
add_executable(test_parser ${TEST_DIR}/test_parser.c)
add_executable(test_overlord ${TEST_DIR}/test_overlord.c)
add_executable(test_ds ${TEST_DIR}/test_ds.c ${SRC_DIR}/common_ds.c)
add_executable(test_tokenizer ${TEST_DIR}/test_tokenizer.c)

# Link libraries to the test executables
target_link_libraries(test_decoder PRIVATE decoder_lib parser_lib logger_lib)
target_link_libraries(test_logger PRIVATE logger_lib)
target_link_libraries(test_parser PRIVATE parser_lib)
target_link_libraries(test_overlord PRIVATE decoder_lib logger_lib parser_lib)
target_link_libraries(test_tokenizer PRIVATE tokenizer_lib)

target_include_directories(test_decoder PUBLIC ${INCLUDE_DIR})
target_include_directories(test_logger PUBLIC ${INCLUDE_DIR})
target_include_directories(test_parser PUBLIC ${INCLUDE_DIR})
target_include_directories(test_ds PUBLIC ${INCLUDE_DIR})
target_include_directories(test_tokenizer PUBLIC ${INCLUDE_DIR})

# Add tests
add_test(NAME DecoderTest COMMAND test_decoder)
add_test(NAME LoggerTest COMMAND test_logger)
add_test(NAME ParserTest COMMAND test_parser)
add_test(NAME TokenizerTest COMMAND test_tokenizer)
add_test(NAME OverlordTest COMMAND test_overlord)
add_test(NAME DataStructureTest COMMAND test_ds)

# Optionally, set compilation flags for warnings
target_compile_options(Assembler PRIVATE -Wall -Wextra -Werror)

