cmake_minimum_required(VERSION 2.6.0)
project (eeglogger)
#For the shared library:
set ( PROJECT_LINK_LIBS libedk.so )

set ( EXAMPLES_DIR ../../../ )
link_directories( ${EXAMPLES_DIR}/../bin/linux64 )

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(eeglogger main.cpp)
target_link_libraries(eeglogger ${PROJECT_LINK_LIBS} )



