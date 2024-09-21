#include <libstryker/ega/read_byte_planar_bgri_stream.hpp>
#include <libstryker/ega/rgb_pixel.hpp>
#include <libstryker/ega/rgb_pixel_grid.hpp>
#include <libstryker/ega/rgb_pixel_map.hpp>
#include <fcppt/container/grid/apply.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/io/get.hpp>
#include <fcppt/optional/to_exception.hpp>
#include <fcppt/config/external_begin.hpp>
#include <ios>
#include <istream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace libstryker::ega
{

template<typename T>
rgb_pixel<T>
operator+( // NOLINT(misc-use-internal-linkage)
  rgb_pixel<T> const &l,
  rgb_pixel<T> const &r)
{
  return
    rgb_pixel<T>{l.r()+r.r(),l.g()+r.g(),l.b()+r.b()};
}

template<typename T>
rgb_pixel<T>
operator*( // NOLINT(misc-use-internal-linkage)
  T const m,
  rgb_pixel<T> const &p)
{
  return
    libstryker::ega::rgb_pixel_map(p,[m](T const t) { return static_cast<T>(m * t); });
}

}

namespace
{
namespace ega
{
namespace
{
using pixel_plane =
fcppt::container::grid::object<bool,2>;

pixel_plane
read_pixel_plane(
  std::istream &s,
  pixel_plane::dim const &dims,
  std::streamsize const stride)
{
  if(dims.w() % 8U != 0)
  {
    throw std::runtime_error(std::to_string(dims.w())+" is not a multiple of 8");
  }
  // TODO(philipp): Express this using an init function
  pixel_plane result(dims,false);
  for(
    auto current_pixel = result.begin();
    current_pixel != result.end();)
  {
    char const c{
      fcppt::optional::to_exception(
        fcppt::io::get(s),
	[&result,current_pixel]{
	  return
            std::runtime_error{"premature end of file after "
              + std::to_string(std::distance(result.begin(),current_pixel))
              +"th pixel"};
    })};
    *current_pixel++ = ((c & 128) != 0);
    *current_pixel++ = ((c & 64) != 0);
    *current_pixel++ = ((c & 32) != 0);
    *current_pixel++ = ((c & 16) != 0);
    *current_pixel++ = ((c & 8) != 0);
    *current_pixel++ = ((c & 4) != 0);
    *current_pixel++ = ((c & 2) != 0);
    *current_pixel++ = ((c & 1) != 0);
    s.ignore(stride); // NOLINT(fuchsia-default-arguments-calls)
  }

  return result;
}

template<typename U,typename T>
libstryker::ega::rgb_pixel<U>
rgb_pixel_static_cast(libstryker::ega::rgb_pixel<T> const &p)
{
  return libstryker::ega::rgb_pixel_map(p,[](T const t) { return static_cast<U>(t); });
}

// Shoutout to http://www.shikadi.net/moddingwiki/EGA_Palette
libstryker::ega::rgb_pixel<unsigned char>
bgri_indices_to_pixel(
  bool const b,
  bool const g,
  bool const r,
  bool const i)
{
  return
    r && g && !i && !b
    ?
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      libstryker::ega::rgb_pixel<unsigned char>{0xa8,0x54,0}
    :
      rgb_pixel_static_cast<unsigned char>(
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	((i ? 1 : 0) * libstryker::ega::rgb_pixel<int>{0x54,0x54,0x54}) +
	libstryker::ega::rgb_pixel_map(
	  libstryker::ega::rgb_pixel<bool>{r,g,b},
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	  [](bool const p) { return p ? 0xa8 : 0; }));
}
}
}
}

libstryker::ega::rgb_pixel_grid
libstryker::ega::read_byte_planar_bgri_stream(
  std::istream &s)
{
  rgb_pixel_grid::dim const d{320U,192U}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
//  auto d = rgb_pixel_grid::dim{16u,3480u};
//  auto d = rgb_pixel_grid::dim{16u,3840u};
  std::streamoff const stream_start{s.tellg()};
  std::streamsize const stride{3};

  ::ega::pixel_plane const b_plane{::ega::read_pixel_plane(s,d,stride)};
  s.seekg(stream_start+1,std::ios_base::beg);

  ::ega::pixel_plane const g_plane{::ega::read_pixel_plane(s,d,stride)};
  s.seekg(stream_start+2,std::ios_base::beg);

  ::ega::pixel_plane const r_plane{::ega::read_pixel_plane(s,d,stride)};
  s.seekg(stream_start+3,std::ios_base::beg);

  ::ega::pixel_plane const i_plane{::ega::read_pixel_plane(s,d,stride)};
  return
    fcppt::container::grid::apply(
      ::ega::bgri_indices_to_pixel,
      b_plane,
      g_plane,
      r_plane,
      i_plane);
}
