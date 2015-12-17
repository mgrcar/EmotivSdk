cmake_minimum_required(VERSION 2.6.0)
project (faceexp)
#For the shared library:
set ( PROJECT_LINK_LIBS libedk.so )

set ( EXAMPLES_DIR ../../../ )
link_directories( ${EXAMPLES_DIR}/../bin/linux64 )

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(faceexp Socket.cpp FacialExpressionControl.cpp FacialExpressionDemo.cpp )
target_link_libraries(faceexp ${PROJECT_LINK_LIBS} )



