cmake_minimum_required(VERSION 3.0.0)
project (saving_loading_cloud_profile)

set(SAVING_LOADING_CLOUD_PROFILE_EXE SavingLoadingCloudProfile)

add_executable(${SAVING_LOADING_CLOUD_PROFILE_EXE} main.cpp)
target_link_libraries(${SAVING_LOADING_CLOUD_PROFILE_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${SAVING_LOADING_CLOUD_PROFILE_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${SAVING_LOADING_CLOUD_PROFILE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${SAVING_LOADING_CLOUD_PROFILE_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${SAVING_LOADING_CLOUD_PROFILE_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${SAVING_LOADING_CLOUD_PROFILE_EXE}>)
endif()

if (APPLE)
    set_target_properties(${SAVING_LOADING_CLOUD_PROFILE_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)



