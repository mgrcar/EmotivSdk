cmake_minimum_required(VERSION 3.0.0)
project (emostatelogger)

set(EMOSTATE_LOGGER_EXE EmostateLogger)

add_executable(${EMOSTATE_LOGGER_EXE} main.cpp)
target_link_libraries(${EMOSTATE_LOGGER_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${EMOSTATE_LOGGER_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${EMOSTATE_LOGGER_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${EMOSTATE_LOGGER_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${EMOSTATE_LOGGER_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${EMOSTATE_LOGGER_EXE}>)
endif()

if (APPLE)
    set_target_properties(${EMOSTATE_LOGGER_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)



