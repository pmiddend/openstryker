#ifndef LIBSTRYKER_EGA_RGB_PIXEL_MAP_HPP_INCLUDED
#define LIBSTRYKER_EGA_RGB_PIXEL_MAP_HPP_INCLUDED

#include <libstryker/ega/rgb_pixel.hpp>


namespace libstryker::ega
{

template<typename T,typename Function>
auto
rgb_pixel_map(
  libstryker::ega::rgb_pixel<T> const &p,
  Function const &function)
-> libstryker::ega::rgb_pixel<decltype(function(p.r()))>
{
  return
    libstryker::ega::rgb_pixel<decltype(function(p.r()))>{
      function(p.r()),
      function(p.g()),
      function(p.b())};
}

}

#endif
