cmake_minimum_required(VERSION 3.0.0)
project (motion_data_logger)

set(MOTION_DATA_LOGGER MotionDataLogger)

add_executable(${MOTION_DATA_LOGGER} main.cpp)
target_link_libraries(${MOTION_DATA_LOGGER} ${PROJECT_LINK_LIBS})
target_include_directories(${MOTION_DATA_LOGGER}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${MOTION_DATA_LOGGER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${MOTION_DATA_LOGGER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${MOTION_DATA_LOGGER} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${MOTION_DATA_LOGGER}>)
endif()

if (APPLE)
    set_target_properties(${MOTION_DATA_LOGGER} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)



