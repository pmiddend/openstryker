cmake_minimum_required(VERSION 3.3)
project(cmp_unpacker)

find_package(
        fcppt
        REQUIRED
)

add_executable(
        cmp_unpacker
        src/main.cpp
)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(cmp_unpacker)

target_link_libraries(
        cmp_unpacker
        ${fcppt_core_TARGET}
        ${fcppt_filesystem_TARGET}
        ${fcppt_log_TARGET}
)
