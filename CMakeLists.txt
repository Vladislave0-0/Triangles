cmake_minimum_required(VERSION 3.11)
project("Triangles")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option for sanitizer
option(USE_SANITIZER "Enable address sanitizer" OFF)
if(USE_SANITIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

# Required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GTest REQUIRED)

include_directories(include/)
include_directories(imgui/)

# ImGui files
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

set(TRIANGLES_SOURCES
    src/main.cpp
    src/config.cpp
)

set(VISUALIZER_SOURCES
    src/visualizer/camera.cpp
    src/visualizer/flag.cpp
    src/visualizer/grid.cpp
    src/visualizer/shader.cpp
    src/visualizer/ui.cpp
    src/visualizer/visualizer.cpp
)

set(ALL_SOURCES
    ${TRIANGLES_SOURCES}
    ${VISUALIZER_SOURCES}
    ${IMGUI_SOURCES}
)

add_executable(triag ${ALL_SOURCES})

# Linking libraries
target_link_libraries(triag PRIVATE 
    OpenGL::GL 
    GLEW::GLEW 
    glfw
    pthread
    X11
    Xrandr
    Xi
)

# Include directories for the target
target_include_directories(triag PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

# Testing
enable_testing()
add_executable(google_test src/google_test.cpp)
target_link_libraries(google_test PRIVATE GTest::gtest_main)

gtest_discover_tests(google_test TEST_PREFIX gtest_)

add_test(
    NAME end2end_tests
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/end2end/run_e2e.sh 
        -b $<TARGET_FILE:triag>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/end2end
)

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS google_test
)
