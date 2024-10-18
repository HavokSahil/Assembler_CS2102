cmake_minimum_required(VERSION 3.10)
project(Assembler C)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Enable debug mode
set(CMAKE_BUILD_TYPE Debug)


# Add compiler flags for strict C89 compliance
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -std=c89
        -w
        #-pedantic
        #-W
        #-Wall
        #-Wpointer-arith
        #-Wwrite-strings
        -g
        -O0
    )
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.c
)

# Add libraries
add_library(ass_utils src/utils.c)
add_library(arg_utils src/apsr.c)

# Create the main executable
add_executable(Assembler ${SOURCES})

# Link libraries
target_link_libraries(Assembler PRIVATE
    ass_utils
    arg_utils
)

# Enable testing functionality
enable_testing()

set(TEST_SOURCES
    test/c_tests.c
)
set(HASHMAP_TEST_SOURCES
    test/hashmap_tests.c
)

set(SYSTEMIZER_TEST_SOURCES
    test/systemizer_tests.c
)

# Add test executable
add_executable(AssemblerTests ${TEST_SOURCES})
add_executable(HashMapTests ${HASHMAP_TEST_SOURCES})
add_executable(SystemizerTests ${SYSTEMIZER_TEST_SOURCES})

# Link the main project target to tests
target_link_libraries(AssemblerTests PRIVATE
    ass_utils
)

target_link_libraries(HashMapTests PRIVATE
    ass_utils
)

target_link_libraries(SystemizerTests PRIVATE
    ass_utils
)

add_test(NAME AssemblerTests COMMAND AssemblerTests)
add_test(NAME HashMapTests COMMAND HashMapTests)
add_test(NAME SystemizerTests COMMAND SystemizerTests)
