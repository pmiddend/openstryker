find_package(fcppt REQUIRED)

add_executable(
  ega_reader
  src/main.cpp
)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(ega_reader)

target_link_libraries(
  ega_reader
  PRIVATE
  ${libstryker_TARGET}
  ${fcppt_core_TARGET}
  ${FCPPT_UTILS_FILESYSTEM_LIBRARIES}
)
