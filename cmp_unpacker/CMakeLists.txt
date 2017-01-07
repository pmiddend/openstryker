find_package(fcppt REQUIRED)
find_package(
  Boost REQUIRED
  COMPONENTS filesystem)

add_executable(
  cmp_unpacker
  src/main.cpp
)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(cmp_unpacker)

target_link_libraries(
  cmp_unpacker
  ${libstryker_TARGET}
  ${fcppt_core_TARGET}
  ${Boost_FILESYSTEM_LIBRARIES}
)
