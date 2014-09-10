cmake_minimum_required(VERSION 2.8)

# Make sure the compiler can find include files from our library.
include_directories (${PROJECT_SOURCE_DIR}/etcdcpp)

# Make sure the linker can find the Hello library once it is built.
link_directories (${PROJECT_SOURCE_DIR}/etcdcpp/build)

# Add executable that is built from the source files
add_executable (etcdcppdemo etcdcppdemo.cpp)

# Link the executable to the library.
target_link_libraries (etcdcppdemo etcdcpp curl) 

