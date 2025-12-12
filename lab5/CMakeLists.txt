cmake_minimum_required(VERSION 3.15)
project(QueueWithCustomAllocator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(main 
    src/main.cpp 
    src/DynamicHeapResource.cpp
)

add_executable(test_queue 
    tests/test_queue.cpp 
    src/DynamicHeapResource.cpp
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(test_queue PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS main DESTINATION bin)
install(TARGETS test_queue DESTINATION bin)