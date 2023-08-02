cmake_minimum_required(VERSION 3.24)

option(ARGPARSER_BUILD_TESTS "Build tests" ON)
option(ARGPARSER_ENABLE_TEST_COVERAGE "Enable coverage reporting" OFF)
option(ARGPARSER_INSTALL "Generate installation target" ON)

if (ARGPARSER_BUILD_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

project(argparser
        DESCRIPTION "Parser for command-line options, arguments and sub-commands."
        LANGUAGES C CXX
        VERSION 0.1)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (ARGPARSER_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
endif ()

if (ARGPARSER_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    find_package(GTest CONFIG REQUIRED)
endif ()

add_library(coverage_config INTERFACE)

if (ARGPARSER_ENABLE_TEST_COVERAGE)
    target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
    target_link_options(coverage_config INTERFACE --coverage)
endif ()

if (ARGPARSER_INSTALL)
    set(ARGPARSER_EXPORT_NAME "buracchi-${PROJECT_NAME}")
    set(ARGPARSER_NAMESPACE "buracchi::${PROJECT_NAME}")
    set(ARGPARSER_LIBRARY_PREFIX "libburacchi-${PROJECT_NAME}-")

    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/config.cmake.in"
            "${PROJECT_BINARY_DIR}/${ARGPARSER_EXPORT_NAME}-config.cmake"
            INSTALL_DESTINATION
            "${CMAKE_INSTALL_DATAROOTDIR}/${ARGPARSER_EXPORT_NAME}")

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${ARGPARSER_EXPORT_NAME}-config.cmake"
            DESTINATION
            "${CMAKE_INSTALL_DATAROOTDIR}/${ARGPARSER_EXPORT_NAME}")
endif ()

find_package(buracchi-common CONFIG REQUIRED)

add_library(argparser STATIC
            "src/argparser.c"
            "src/parser.c"
            "src/usage.c"
            "src/utils.c")
target_include_directories(argparser PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(argparser PRIVATE buracchi::common::utilities)
target_link_libraries(argparser INTERFACE $<BUILD_INTERFACE:coverage_config>)
set_target_properties(argparser PROPERTIES PREFIX ${ARGPARSER_LIBRARY_PREFIX})

if (ARGPARSER_BUILD_TESTS)
    add_subdirectory("test")
endif ()

if (ARGPARSER_INSTALL)
    install(TARGETS argparser
            EXPORT argparser-targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
            FILES_MATCHING PATTERN "*.h*")

    install(EXPORT argparser-targets
            FILE ${ARGPARSER_EXPORT_NAME}-targets.cmake
            NAMESPACE ${ARGPARSER_NAMESPACE}::
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${ARGPARSER_EXPORT_NAME}")
endif ()
