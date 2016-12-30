cmake_minimum_required(VERSION 3.0.0)

project (activatelicense)

set(ACTIVATE_LICENSE_EXE ActivateLicense)
 
add_executable(${ACTIVATE_LICENSE_EXE} ActivateLicense.cpp)
target_link_libraries(${ACTIVATE_LICENSE_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${ACTIVATE_LICENSE_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${ACTIVATE_LICENSE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${ACTIVATE_LICENSE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${ACTIVATE_LICENSE_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${ACTIVATE_LICENSE_EXE}>)
endif()

if (APPLE)
    set_target_properties(${ACTIVATE_LICENSE_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)
