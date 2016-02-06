#include <libstryker/ega/read_byte_planar_bgri_stream.hpp>
#include <libstryker/ega/rgb_pixel.hpp>
#include <libstryker/ega/rgb_pixel_grid.hpp>
#include <libstryker/ega/rgb_pixel_map.hpp>
#include <fcppt/no_init.hpp>
#include <fcppt/container/grid/apply.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <istream>
#include <iterator>
#include <string>
#include <stdexcept>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace ega
{

template<typename T>
rgb_pixel<T>
operator+(
  rgb_pixel<T> const &l,
  rgb_pixel<T> const &r)
{
  return
    rgb_pixel<T>{l.r()+r.r(),l.g()+r.g(),l.b()+r.b()};
}

template<typename T>
rgb_pixel<T>
operator*(
  T const m,
  rgb_pixel<T> const &p)
{
  return
    libstryker::ega::rgb_pixel_map(p,[m](T const t) { return static_cast<T>(m * t); });
}

}
}

namespace
{
namespace ega
{
typedef
fcppt::container::grid::object<bool,2>
pixel_plane;

pixel_plane
read_pixel_plane(
  std::istream &s,
  pixel_plane::dim const &dims,
  std::streamsize const stride)
{
  if(dims.w() % 8u != 0)
    throw std::runtime_error(std::to_string(dims.w())+" is not a multiple of 8");
  pixel_plane result(dims,fcppt::no_init{});
  for(
    pixel_plane::iterator current_pixel = result.begin();
    current_pixel != result.end();)
  {
    int const c{s.get()};
    if(c == std::istream::traits_type::eof())
      throw std::runtime_error("premature end of file after "+std::to_string(std::distance(result.begin(),current_pixel))+"th pixel");
    *current_pixel++ = c & 128;
    *current_pixel++ = c & 64;
    *current_pixel++ = c & 32;
    *current_pixel++ = c & 16;
    *current_pixel++ = c & 8;
    *current_pixel++ = c & 4;
    *current_pixel++ = c & 2;
    *current_pixel++ = c & 1;
    s.ignore(stride);
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
      libstryker::ega::rgb_pixel<unsigned char>{0xa8,0x54,0}
    :
      rgb_pixel_static_cast<unsigned char>(
	((i ? 1 : 0) * libstryker::ega::rgb_pixel<int>{0x54,0x54,0x54}) +
	libstryker::ega::rgb_pixel_map(
	  libstryker::ega::rgb_pixel<bool>{r,g,b},
	  [](bool const p) { return p ? 0xa8 : 0; }));
}
}
}

libstryker::ega::rgb_pixel_grid
libstryker::ega::read_byte_planar_bgri_stream(
  std::istream &s)
{
  rgb_pixel_grid::dim const d{320u,192u};
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
