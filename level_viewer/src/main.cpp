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
#include <sge/systems/keyboard_collector.hpp>
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
#include <sge/window/title.hpp>
#include <awl/show_error.hpp>
#include <awl/show_error_narrow.hpp>
#include <awl/main/exit_code.hpp>
#include <awl/main/exit_failure.hpp>
#include <awl/main/exit_success.hpp>
#include <awl/main/function_context.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/optional/to_exception.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <exception>
#include <stdexcept>
#include <string>
#include <fcppt/config/external_end.hpp>


awl::main::exit_code const
level_viewer_main(awl::main::function_context const &);

awl::main::exit_code const
level_viewer_main(awl::main::function_context const &_args)
try
{
  if(_args.argc() != 2)
  {
    awl::show_error_narrow(std::string("Usage: ") + _args.argv()[0] + " <level-file>");
    return awl::main::exit_failure();
  }

   boost::filesystem::ifstream stream(boost::filesystem::path{_args.argv()[1]});

  libstryker::level::record const level{
    fcppt::optional::to_exception(
      libstryker::level::read(stream),
      []{
        return std::runtime_error{"Failed to read level!"};
      })};

  sge::systems::instance<
    boost::mpl::vector3<
      sge::systems::with_window,
      sge::systems::with_renderer<
        sge::systems::renderer_caps::core
      >,
      sge::systems::with_input<
        boost::mpl::vector1<
          sge::systems::keyboard_collector
        >
      >
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

  return awl::main::exit_success();
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
