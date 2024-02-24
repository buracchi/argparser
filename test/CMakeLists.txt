find_package(buracchi-cutest CONFIG REQUIRED)

add_executable(test_argparser "argparser.c")
target_link_libraries(test_argparser PRIVATE buracchi::cutest::cutest buracchi::cutest::cutest_main)
target_link_libraries(test_argparser PRIVATE argparser)
target_link_libraries(test_argparser INTERFACE coverage_config)
cutest_discover_tests(test_argparser)

