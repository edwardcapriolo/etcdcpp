# Make sure the compiler can find include files from our Hello library.
include_directories (${PROJECT_SOURCE_DIR}/etcdcpp)

# Make sure the linker can find the Hello library once it is built.
link_directories (${PROJECT_SOURCE_DIR}/etcdcpp)

# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable (etcdcppdemo etcdcppdemo.cpp)

# Link the executable to the Hello library.
target_link_libraries (etcdcppdemo etcdcpp yajl curl) 
