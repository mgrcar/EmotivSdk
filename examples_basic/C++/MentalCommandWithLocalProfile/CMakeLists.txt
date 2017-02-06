cmake_minimum_required(VERSION 3.0.0)
project (mentalcommandwithlocalprofile)

set(META_COMMAND_WITH_LOCAL_PROFILE_EXE MentalCommandWithLocalProfile)

add_executable(${META_COMMAND_WITH_LOCAL_PROFILE_EXE} MentalCommandWithLocalProfile.cpp)
target_link_libraries(${META_COMMAND_WITH_LOCAL_PROFILE_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${META_COMMAND_WITH_LOCAL_PROFILE_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${META_COMMAND_WITH_LOCAL_PROFILE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${META_COMMAND_WITH_LOCAL_PROFILE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${META_COMMAND_WITH_LOCAL_PROFILE_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${META_COMMAND_WITH_LOCAL_PROFILE_EXE}>)
endif()

if (APPLE)
    set_target_properties(${META_COMMAND_WITH_LOCAL_PROFILE_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)


