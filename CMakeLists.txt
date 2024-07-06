cmake_minimum_required(VERSION 3.27.0)

project(openstryker)

find_package(fcppt REQUIRED)

list(APPEND FCPPT_UTILS_CLANG_TIDY_ADDITIONAL_CHECKS
     "-cppcoreguidelines-avoid-const-or-ref-data-members"
     "-misc-misplaced-const")

set(FCPPT_UTILS_CLANG_TIDY_WHITELIST "fcppt" "awl" "mizuiro" "sge" "alda"
                                     "libstryker")

include(FcpptCMakeUtils)

fcppt_utils_static_shared()

add_subdirectory(libstryker)

fcppt_utils_link_target(libstryker)

add_subdirectory(cmp_unpacker)

add_subdirectory(ega_reader)

add_subdirectory(level_reader)

add_subdirectory(level_viewer)
