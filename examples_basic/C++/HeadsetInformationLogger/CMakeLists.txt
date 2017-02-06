cmake_minimum_required(VERSION 3.0.0)
project (headsetinfologger)

set(HEADSET_INFO_LOGGER_EXE HeadsetInfoLogger)

add_executable(${HEADSET_INFO_LOGGER_EXE} main.cpp)
target_link_libraries(${HEADSET_INFO_LOGGER_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${HEADSET_INFO_LOGGER_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${HEADSET_INFO_LOGGER_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${HEADSET_INFO_LOGGER_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${HEADSET_INFO_LOGGER_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${HEADSET_INFO_LOGGER_EXE}>)
endif()

if (APPLE)
    set_target_properties(${HEADSET_INFO_LOGGER_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)




