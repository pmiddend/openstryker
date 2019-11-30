cmake_minimum_required(VERSION 3.3)

project(openstryker)

find_package(fcppt REQUIRED)
find_package(Metal REQUIRED)
include(FcpptCMakeUtils)

fcppt_utils_static_shared()

add_subdirectory(
	libstryker
)

fcppt_utils_link_target(libstryker)

add_subdirectory(
	cmp_unpacker
)

add_subdirectory(
	ega_reader
)

add_subdirectory(
	level_reader
)

add_subdirectory(
	level_viewer
)
