cmake_minimum_required(VERSION 3.0.0)
project (performance_metrics)

set(PERFORMANCE_METRICS_EXE PerformanceMetrics)

add_executable(${PERFORMANCE_METRICS_EXE} PerformanceMetricsDemo.cpp)
target_link_libraries(${PERFORMANCE_METRICS_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${PERFORMANCE_METRICS_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${PERFORMANCE_METRICS_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${PERFORMANCE_METRICS_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${PERFORMANCE_METRICS_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${PERFORMANCE_METRICS_EXE}>)
endif()

if (APPLE)
    set_target_properties(${PERFORMANCE_METRICS_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)




