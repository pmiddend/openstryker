#include <libstryker/level/read.hpp>
#include <libstryker/level/record.hpp>
#include <sge/renderer/display_mode/optional_object.hpp>
#include <sge/renderer/display_mode/parameters.hpp>
#include <sge/renderer/display_mode/vsync.hpp>
#include <sge/renderer/pixel_format/color.hpp>
#include <sge/renderer/pixel_format/depth_stencil.hpp>
#include <sge/renderer/pixel_format/object.hpp>
#include <sge/renderer/pixel_format/optional_multi_samples.hpp>
#include <sge/renderer/pixel_format/srgb.hpp>
#include <sge/systems/cursor_option_field.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/make_list.hpp>
#include <sge/systems/original_window.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/renderer_caps.hpp>
#include <sge/systems/window.hpp>
#include <sge/systems/window_source.hpp>
#include <sge/systems/with_input.hpp>
#include <sge/systems/with_renderer.hpp>
#include <sge/systems/with_window.hpp>
#include <sge/viewport/fill_on_resize.hpp>
#include <sge/viewport/optional_resize_callback.hpp>
#include <sge/window/loop.hpp>
#include <sge/window/loop_function.hpp>
#include <sge/window/system.hpp>
#include <sge/window/title.hpp>
#include <alda/raw/stream/error.hpp>
#include <awl/show_error.hpp>
#include <awl/show_error_narrow.hpp>
#include <awl/event/base_fwd.hpp>
#include <awl/main/exit_code.hpp>
#include <awl/main/exit_failure.hpp>
#include <awl/main/exit_success.hpp>
#include <awl/main/function_context.hpp>
#include <fcppt/args.hpp>
#include <fcppt/args_vector.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/container/at_optional.hpp>
#include <fcppt/either/to_exception.hpp>
#include <fcppt/optional/to_exception.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <exception>
#include <stdexcept>
#include <fcppt/config/external_end.hpp>


awl::main::exit_code
level_viewer_main(awl::main::function_context const &);

awl::main::exit_code
level_viewer_main(awl::main::function_context const &_args)
try
{
  fcppt::args_vector const args(fcppt::args(_args.argc(),_args.argv()));
  fcppt::string const file_name(
    fcppt::optional::to_exception(
      fcppt::container::at_optional(args,1u),
      []{
        return std::runtime_error("Usage: level_viewer <level-file>");
      }).get());

  boost::filesystem::ifstream stream(boost::filesystem::path{file_name});

  libstryker::level::record const level{
    fcppt::either::to_exception(
      libstryker::level::read(stream),
      [](alda::raw::stream::error const &_error){
        return std::runtime_error{"Failed to read level! " + fcppt::to_std_string(_error.get())};
      })};

  sge::systems::instance<
    boost::mpl::vector3<
      sge::systems::with_window,
      sge::systems::with_renderer<
        sge::systems::renderer_caps::core
      >,
      sge::systems::with_input
    >
  >
  const sys(
    sge::systems::make_list
    (
      sge::systems::window{sge::systems::window_source{sge::systems::original_window{
       sge::window::title{FCPPT_TEXT("openstryker level viewer")}
      }}}
    )
    (
      sge::systems::renderer{
        sge::renderer::pixel_format::object{
          sge::renderer::pixel_format::color::depth32,
          sge::renderer::pixel_format::depth_stencil::off,
          sge::renderer::pixel_format::optional_multi_samples(),
          sge::renderer::pixel_format::srgb::no
        },
        sge::renderer::display_mode::parameters{
          sge::renderer::display_mode::vsync::on,
          sge::renderer::display_mode::optional_object()},
        sge::viewport::optional_resize_callback{sge::viewport::fill_on_resize()}}
    )
    (
      sge::systems::input{sge::systems::cursor_option_field::null()}
    )
  );

  // TODO
  sys.window_system().quit(awl::main::exit_success());

  return sge::window::loop(
    sys.window_system(),
    sge::window::loop_function{
      [](awl::event::base const &) {}
    });
}
catch(fcppt::exception const &_error)
{
  awl::show_error(_error.string());
  return awl::main::exit_failure();
}
catch(std::exception const &_error)
{
  awl::show_error_narrow(_error.what());
  return awl::main::exit_failure();
}
