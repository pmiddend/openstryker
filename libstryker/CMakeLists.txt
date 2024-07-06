fcppt_utils_link_target(libstryker)

find_package(fcppt REQUIRED)

find_package(alda REQUIRED)

fcppt_utils_load_source_files(libstryker/files.txt LIBSTRYKER_FILES)

set(LIBSTRYKER_STATIC_LINK_FLAG LIBSTRYKER_STATIC_LINK)

include(FcpptSymbol)

fcppt_generate_symbol_header(
  STATIC_LINK_FLAG "${LIBSTRYKER_STATIC_LINK_FLAG}" EXPORTS_NAME "libstryker"
  SYMBOL_NAME "LIBSTRYKER_DETAIL")

function(add_libstryker_library VARIANT)
  if(${VARIANT} STREQUAL "STATIC")
    fcppt_utils_static_link_name(libstryker LIBSTRYKER_LIB_NAME)
  else()
    set(LIBSTRYKER_LIB_NAME libstryker)
  endif()

  fcppt_utils_append_source_dir_and_make_groups("${LIBSTRYKER_FILES}"
                                                LIBSTRYKER_FILES_ABS)

  add_library(${LIBSTRYKER_LIB_NAME} ${VARIANT} ${LIBSTRYKER_FILES_ABS})

  fcppt_utils_set_target_compiler_flags(${LIBSTRYKER_LIB_NAME})

  target_include_directories(
    ${LIBSTRYKER_LIB_NAME} PUBLIC ${FCPPT_UTILS_PROJECT_BINARY_DIR}/include
                                  ${CMAKE_CURRENT_SOURCE_DIR}/include)

  fcppt_utils_interface_static_link(${LIBSTRYKER_LIB_NAME} ${VARIANT}
                                    "${LIBSTRYKER_STATIC_LINK_FLAG}")

  target_link_libraries(
    ${LIBSTRYKER_LIB_NAME}
    PUBLIC alda_interface fcppt_core_interface
    PRIVATE ${fcppt_core_TARGET} ${FCPPT_UTILS_FILESYSTEM_LIBRARIES})

  fcppt_utils_export_install_target(${LIBSTRYKER_LIB_NAME})
endfunction()

if(ENABLE_SHARED)
  add_libstryker_library("SHARED")
endif()

if(ENABLE_STATIC)
  add_libstryker_library("STATIC")
endif()
