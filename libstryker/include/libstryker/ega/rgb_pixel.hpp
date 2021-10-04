#ifndef LIBSTRYKER_EGA_RGB_PIXEL_HPP_INCLUDED
#define LIBSTRYKER_EGA_RGB_PIXEL_HPP_INCLUDED


namespace libstryker::ega
{

template<typename T>
struct rgb_pixel
{
  using value_type = T;

  rgb_pixel(
    value_type const _r,
    value_type const _g,
    value_type const _b)
  :
    r_{_r},
    g_{_g},
    b_{_b}
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
