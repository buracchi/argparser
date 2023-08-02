add_library(main-mock OBJECT "main-mock.c")
target_link_libraries(main-mock PRIVATE argparser)
target_link_libraries(main-mock INTERFACE coverage_config)

add_executable(test_argparser "argparser.cpp")
target_link_libraries(test_argparser PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(test_argparser PRIVATE main-mock)
gtest_discover_tests(test_argparser)
