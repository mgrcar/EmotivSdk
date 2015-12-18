cmake_minimum_required(VERSION 2.6.0)
project (gyro)
#For the shared library:
set ( PROJECT_LINK_LIBS libedk.so libglut.so libGLU.so libGL.so)

set ( EXAMPLES_DIR ../../../ )
link_directories( ${EXAMPLES_DIR}/../bin/linux64 )

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(gyro main.cpp)
target_link_libraries(gyro ${PROJECT_LINK_LIBS} )



