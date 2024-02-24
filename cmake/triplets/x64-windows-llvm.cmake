set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CRT_LINKAGE dynamic)

# I suffered to much, I deserve to put here this hardcoded carp and call it a day.
set(ENV{LLVMInstallDir} "C:/Program Files/LLVM")
set(ENV{VCPKG_ROOT} "${CMAKE_CURRENT_LIST_DIR}/../../vcpkg")

set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../toolchains/x64-windows-llvm.toolchain.cmake")
set(VCPKG_LOAD_VCVARS_ENV ON)

if(DEFINED VCPKG_PLATFORM_TOOLSET)
    set(VCPKG_PLATFORM_TOOLSET ClangCL)
endif()
set(VCPKG_ENV_PASSTHROUGH_UNTRACKED "LLVMInstallDir;LLVMToolsVersion")

set(VCPKG_POLICY_SKIP_ARCHITECTURE_CHECK enabled)
set(VCPKG_POLICY_SKIP_DUMPBIN_CHECKS enabled)
