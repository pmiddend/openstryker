cmake_minimum_required(VERSION 3.3)

include(FcpptCMakeUtils)

fcppt_utils_static_shared()

add_subdirectory(
	libstryker
)

add_subdirectory(
	cmp_unpacker
)

add_subdirectory(
	ega_reader
)

add_subdirectory(
	level_reader
)
