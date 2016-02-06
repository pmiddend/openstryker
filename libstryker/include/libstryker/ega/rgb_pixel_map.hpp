#ifndef LIBSTRYKER_EGA_RGB_PIXEL_MAP_HPP_INCLUDED
#define LIBSTRYKER_EGA_RGB_PIXEL_MAP_HPP_INCLUDED

#include <libstryker/ega/rgb_pixel.hpp>


namespace libstryker
{
namespace ega
{

template<typename U,typename T,typename Function>
libstryker::ega::rgb_pixel<U>
rgb_pixel_map(
  libstryker::ega::rgb_pixel<T> const &p,
  Function const &function)
{
  return
    libstryker::ega::rgb_pixel<U>{
      function(p.r()),
      function(p.g()),
      function(p.b())};
}

}
}

#endif
