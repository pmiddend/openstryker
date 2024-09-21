#ifndef LIBSTRYKER_EGA_RGB_PIXEL_HPP_INCLUDED
#define LIBSTRYKER_EGA_RGB_PIXEL_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>

namespace libstryker::ega
{

template<typename T>
struct rgb_pixel
{
  using value_type = T;

  rgb_pixel(
    value_type _r,
    value_type _g,
    value_type _b)
  :
    r_{std::move(_r)},
    g_{std::move(_g)},
    b_{std::move(_b)}
  {}

  [[nodiscard]] value_type r() const { return r_; }

  [[nodiscard]] value_type g() const { return g_; }

  [[nodiscard]] value_type b() const { return b_; }
private:
  value_type r_;
  value_type g_;
  value_type b_;
};

}

#endif
