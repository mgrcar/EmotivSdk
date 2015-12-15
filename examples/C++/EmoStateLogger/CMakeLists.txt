cmake_minimum_required(VERSION 2.6.0)
project (eslogger)
#For the shared library:
set ( PROJECT_LINK_LIBS libedk.so libcurl.so )

set ( EXAMPLES_DIR ../../../ )
link_directories( ${EXAMPLES_DIR}/../bin/linux64 )

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(eslogger main.cpp)
target_link_libraries(eslogger ${PROJECT_LINK_LIBS} )



