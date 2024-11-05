cmake_minimum_required(VERSION 3.11)

project("Triangles")

include_directories(include/)

add_executable (triag src/main.cpp src/config.cpp)

# find_package(GTest REQUIRED)
# enable_testing()
# add_executable(google_test src/google_test.cpp)

# target_link_libraries(
#     google_test
#   GTest::gtest_main
# )
