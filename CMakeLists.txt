cmake_minimum_required(VERSION 4.3)
project(uring_stdexec_server CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Locate the dependencies fetched by conan
find_package(liburing REQUIRED)
find_package(P2300 REQUIRED)

add_executable(uring_stdexec_server src/main.cpp)

# Link both targets safely
target_link_libraries(uring_stdexec_server PRIVATE 
    liburing::liburing
    P2300::P2300
)
