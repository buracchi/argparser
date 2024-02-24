IF (NOT _VCPKG_CLANGCL_TOOLCHAIN)
    set(_VCPKG_CLANGCL_TOOLCHAIN 1)

    IF (DEFINED ENV{LLVMInstallDir})
        cmake_path(SET _vcpkg_llvmbin "$ENV{LLVMInstallDir}/bin" NORMALIZE)
        string(REPLACE "\"" "" _vcpkg_llvmbin "${_vcpkg_llvmbin}")
        find_program(CMAKE_C_COMPILER "clang-cl.exe" PATHS "${_vcpkg_llvmbin}" REQUIRED NO_DEFAULT_PATH)
        find_program(CMAKE_CXX_COMPILER "clang-cl.exe" PATHS "${_vcpkg_llvmbin}" REQUIRED NO_DEFAULT_PATH)
        find_program(CMAKE_AR "llvm-lib.exe" PATHS "${_vcpkg_llvmbin}" REQUIRED NO_DEFAULT_PATH)
    ELSE ()
        set(CMAKE_C_COMPILER "clang-cl.exe")
        set(CMAKE_CXX_COMPILER "clang-cl.exe")
    ENDIF ()

    IF (VCPKG_TARGET_ARCHITECTURE STREQUAL "x64")
        set(_vcpkg_clangcl_arch "-m64")
    ENDIF ()

    list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES ${_vcpkg_clangcl_arch})

    set(VCPKG_MSVC_CXX_WINRT_EXTENSIONS OFF)

    get_property(_CMAKE_IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE)

    IF (NOT _CMAKE_IN_TRY_COMPILE)
        string(APPEND VCPKG_C_FLAGS " ${_vcpkg_clangcl_arch}")
        string(APPEND VCPKG_CXX_FLAGS " ${_vcpkg_clangcl_arch}")
    ENDIF ()

    if(NOT DEFINED VCPKG_INSTALLED_DIR)
        set(VCPKG_INSTALLED_DIR "$ENV{VCPKG_ROOT}")
    endif()
    include("${VCPKG_INSTALLED_DIR}/scripts/toolchains/windows.cmake")

ENDIF ()
