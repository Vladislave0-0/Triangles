cmake_minimum_required(VERSION 3.11)

project("Triangles")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/)
add_executable(triag src/main.cpp src/config.cpp)

find_package(GTest REQUIRED)
enable_testing()
add_executable(google_test src/google_test.cpp)

target_link_libraries(google_test GTest::gtest_main)

gtest_discover_tests(
    google_test
    TEST_PREFIX gtest_
)

add_test(
    NAME end2end_tests
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/end2end/run_e2e.sh 
        -b $<TARGET_FILE:triag>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/end2end
)

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS google_test end2end
)
