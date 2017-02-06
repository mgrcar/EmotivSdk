cmake_minimum_required(VERSION 3.0.0)
project (facial_expression)

set(FACIAL_EXPRESSION_EXE FacialExpression)

add_executable(${FACIAL_EXPRESSION_EXE} Socket.cpp FacialExpressionControl.cpp FacialExpressionDemo.cpp)
target_link_libraries(${FACIAL_EXPRESSION_EXE} ${PROJECT_LINK_LIBS})
target_include_directories(${FACIAL_EXPRESSION_EXE}
    PRIVATE
    ${COMMUNITY_SDK_INCLUDE_DIR})
if (WIN32)
    if(MSVC)
        set_target_properties(${FACIAL_EXPRESSION_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>)
    else()
        set_target_properties(${FACIAL_EXPRESSION_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    endif()
    add_custom_command(TARGET ${FACIAL_EXPRESSION_EXE} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${SDK_DLLS} $<TARGET_FILE_DIR:${FACIAL_EXPRESSION_EXE}>)
endif()

if (APPLE)
    set_target_properties(${FACIAL_EXPRESSION_EXE} PROPERTIES LINK_FLAGS "-F/Library/Frameworks")
endif(APPLE)



