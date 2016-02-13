fcppt_utils_link_target(
	libstryker
)

find_package(
	Boost REQUIRED
	COMPONENTS filesystem
)

find_package(
        fcppt REQUIRED
)

find_package(
	majutsu REQUIRED
)

find_package(
	alda REQUIRED
)

set(
	LIBSTRYKER_FILES
	libstryker/include/libstryker/cmp/file_table.hpp
	libstryker/include/libstryker/cmp/file_table_entry.hpp
	libstryker/include/libstryker/cmp/read_cmp_entry.hpp
	libstryker/include/libstryker/cmp/read_file_table.hpp
	libstryker/include/libstryker/ega/read_byte_planar_bgri_stream.hpp
	libstryker/include/libstryker/ega/rgb_pixel.hpp
	libstryker/include/libstryker/ega/rgb_pixel_grid.hpp
	libstryker/include/libstryker/ega/rgb_pixel_map.hpp
	libstryker/include/libstryker/level/read.hpp
	libstryker/include/libstryker/level/record.hpp
	libstryker/include/libstryker/level/string.hpp
	libstryker/include/libstryker/level/string_decl.hpp
	libstryker/src/libstryker/cmp/read_cmp_entry.cpp
	libstryker/src/libstryker/cmp/read_file_table.cpp
	libstryker/src/libstryker/ega/read_byte_planar_bgri_stream.cpp
	libstryker/src/libstryker/level/read.cpp
)

set(
	LIBSTRYKER_STATIC_LINK_FLAG
	LIBSTRYKER_STATIC_LINK
)

include(
	FcpptSymbol
)

fcppt_generate_symbol_header(
	STATIC_LINK_FLAG
		"${LIBSTRYKER_STATIC_LINK_FLAG}"
	EXPORTS_NAME
		"libstryker"
	SYMBOL_NAME
		"LIBSTRYKER_DETAIL"
)

function(
	add_libstryker_library
	VARIANT
)
	if(
		${VARIANT} STREQUAL "STATIC"
	)
		fcppt_utils_static_link_name(
			libstryker
			LIBSTRYKER_LIB_NAME
		)
	else()
		set(
			LIBSTRYKER_LIB_NAME
			libstryker
		)
	endif()

	fcppt_utils_append_source_dir_and_make_groups(
		"${LIBSTRYKER_FILES}"
		LIBSTRYKER_FILES_ABS
	)

	add_library(
		${LIBSTRYKER_LIB_NAME}
		${VARIANT}
		${LIBSTRYKER_FILES_ABS}
	)

	fcppt_utils_set_target_compiler_flags(
		${LIBSTRYKER_LIB_NAME}
	)

	target_include_directories(
		${LIBSTRYKER_LIB_NAME}
		PRIVATE
		${FCPPT_UTILS_PROJECT_BINARY_DIR}/include
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${Boost_INCLUDE_DIRS}
		${fcppt_INCLUDE_DIRS}
		${alda_INCLUDE_DIRS}
		${majutsu_INCLUDE_DIRS}
	)

	fcppt_utils_add_target_include_dir(
		${LIBSTRYKER_LIB_NAME}
		TRUE
	)

	fcppt_utils_interface_static_link(
		${LIBSTRYKER_LIB_NAME}
		${VARIANT}
		"${LIBSTRYKER_STATIC_LINK_FLAG}"
	)

	target_link_libraries(
		${LIBSTRYKER_LIB_NAME}
		PRIVATE
		${fcppt_core_TARGET}
		${Boost_FILESYSTEM_LIBRARIES}
	)

	fcppt_utils_export_install_target(
		${LIBSTRYKER_LIB_NAME}
	)
endfunction()

if(
	ENABLE_SHARED
)
	add_libstryker_library(
		"SHARED"
	)
endif()

if(
	ENABLE_STATIC
)
	add_libstryker_library(
		"STATIC"
	)
endif()
