cmake_minimum_required(VERSION 3.0.0)
project (gyro)

set(GYRO_EXE Gyro)
set(FrameworkMac)
if(APPLE)
set(FrameworkMac "-framework OpenGL"
                 "-framework GLUT")
endif()
add_executable(${GYRO_EXE} main.cpp)
target_link_libraries(${GYRO_EXE} PRIVATE
                      ${PROJECT_LINK_LIBS}
                      ${GLUT_LIBS} ${FrameworkMac})
                      
target_include_directories(${GYRO_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${GYRO_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${GYRO_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    
    add_custom_command(TARGET ${GYRO_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} ${THIRD_PARTY_DLLS} $<TARGET_FILE_DIR:${GYRO_EXE}>)
              
elseif (APPLE)
    set_target_properties(${GYRO_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif()
