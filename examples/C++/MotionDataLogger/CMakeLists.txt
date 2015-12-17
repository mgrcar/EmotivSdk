cmake_minimum_required(VERSION 2.6.0)
project (motion)
#For the shared library:
set ( PROJECT_LINK_LIBS libedk.so libglut.so libGLU.so libGL.so)

set ( EXAMPLES_DIR ../../../ )
link_directories( ${EXAMPLES_DIR}/../bin/linux64 )

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(motion main.cpp)
target_link_libraries(motion ${PROJECT_LINK_LIBS} )



