find_package(fcppt REQUIRED)
find_package(awl REQUIRED)
find_package(mizuiro REQUIRED)
find_package(
  sge REQUIRED
  COMPONENTS renderer systems viewport window)

include(AwlMainGenerator)

awl_utils_add_portable_executable(
  level_viewer
  "level_viewer_main"
  src/main.cpp)

include(FcpptCMakeUtils)
fcppt_utils_set_target_compiler_flags(level_viewer)

target_link_libraries(
  level_viewer
  PRIVATE
  ${sgerenderer_TARGET}
  ${sgesystems_TARGET}
  ${sgeviewport_TARGET}
  ${sgewindow_TARGET}
  ${libstryker_TARGET}
  ${awl_TARGET}
  ${fcppt_core_TARGET}
  ${FCPPT_UTILS_FILESYSTEM_LIBRARIES}
)
