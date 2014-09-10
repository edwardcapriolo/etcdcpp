
#set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

add_library (etcdcpp etcdcpp.cpp etcdcpp.h)

install (FILES etcdcpp.h DESTINATION include)

install (
  TARGETS etcdcpp 
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
