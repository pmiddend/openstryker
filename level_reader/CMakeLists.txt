cmake_minimum_required(VERSION 3.3)
project(level_reader)

find_package(
        fcppt
        REQUIRED
)

add_executable(
        level_reader
        src/main.cpp
)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(level_reader)

target_link_libraries(
	level_reader
	${libstryker_TARGET}
	${fcppt_core_TARGET}
)
