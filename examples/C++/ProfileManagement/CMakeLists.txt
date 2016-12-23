cmake_minimum_required(VERSION 2.8.0)

project (ProfileManagement)

set ( EXAMPLES_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../ )

if (WIN32)
        set ( PROJECT_LINK_LIBS edk.lib)
        if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "(amd64)|(AMD64)")
           link_directories( ${EXAMPLES_DIR}/../bin/win64 )
        elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "(x86)|(X86)")
           link_directories( ${EXAMPLES_DIR}/../bin/win32 )
        else()
           link_directories( ${EXAMPLES_DIR}/../bin/winarm )
        endif()
elseif(APPLE)
		set ( PROJECT_LINK_LIBS "-framework edk" )
elseif(UNIX)
        set ( PROJECT_LINK_LIBS libedk.so )
        if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
           link_directories( ${EXAMPLES_DIR}/../bin/linux64 )
        elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86")
           link_directories( ${EXAMPLES_DIR}/../bin/linux32 )
        else()
           link_directories( ${EXAMPLES_DIR}/../bin/armhf )
        endif()
endif()

include_directories(${EXAMPLES_DIR}/include )
include_directories(${CMAKE_CURRENT_LIST_DIR} )
 
add_executable(ProfileManagement ProfileManagement.cpp
  ProfileControl.cpp
  ExternalProfileManager)
  
target_link_libraries(ProfileManagement ${PROJECT_LINK_LIBS} )
if (APPLE)
	set_target_properties(ProfileManagement PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)



