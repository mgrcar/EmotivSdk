cmake_minimum_required(VERSION 2.6.0)
project (mtcmd)
#For the shared library:
set ( PROJECT_LINK_LIBS libedk.so )

set ( EXAMPLES_DIR ../../../ )
link_directories( ${EXAMPLES_DIR}/../bin/linux64 libcurl.so )

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(mtcmd Socket.cpp MentalCommandControl.cpp MentalCommandDemo.cpp )
target_link_libraries(mtcmd ${PROJECT_LINK_LIBS} )



