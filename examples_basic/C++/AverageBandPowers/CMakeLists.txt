cmake_minimum_required(VERSION 3.0.0)
project (averagebandpowers)

set(AVERAGE_BAND_POWERS_EXE AverageBandPowers)

add_executable(${AVERAGE_BAND_POWERS_EXE} main.cpp)
target_link_libraries(${AVERAGE_BAND_POWERS_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${AVERAGE_BAND_POWERS_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${AVERAGE_BAND_POWERS_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${AVERAGE_BAND_POWERS_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${AVERAGE_BAND_POWERS_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${AVERAGE_BAND_POWERS_EXE}>)
endif()

if (APPLE)
    set_target_properties(${AVERAGE_BAND_POWERS_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)
