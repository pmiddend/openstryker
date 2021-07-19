find_package(fcppt REQUIRED)

add_executable(
  level_reader
  src/main.cpp
)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(level_reader)

target_compile_options(
  level_reader
  PRIVATE
  ${FCPPT_UTILS_MAIN_COMPILE_OPTIONS}
)

target_link_libraries(
  level_reader
  PRIVATE
  ${libstryker_TARGET}
  ${fcppt_core_TARGET}
  ${FCPPT_UTILS_MAIN_LINK_LIBS}
)
