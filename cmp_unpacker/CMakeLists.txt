find_package(fcppt REQUIRED)

add_executable(cmp_unpacker src/main.cpp)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(cmp_unpacker)

target_compile_options(cmp_unpacker PRIVATE ${FCPPT_UTILS_MAIN_COMPILE_OPTIONS})

target_link_libraries(
  cmp_unpacker PRIVATE ${libstryker_TARGET} ${fcppt_core_TARGET}
                       ${FCPPT_UTILS_MAIN_LINK_LIBS})
