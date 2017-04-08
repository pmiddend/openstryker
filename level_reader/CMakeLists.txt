find_package(fcppt REQUIRED)
find_package(
  Boost REQUIRED
  COMPONENTS filesystem)

add_executable(
  level_reader
  src/main.cpp
)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(level_reader)

target_link_libraries(
  level_reader
  PRIVATE
  ${libstryker_TARGET}
  ${fcppt_core_TARGET}
  ${Boost_FILESYSTEM_LIBRARIES}
)
