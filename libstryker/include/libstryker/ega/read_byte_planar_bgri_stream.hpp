#ifndef LIBSTRYKER_EGA_READ_BYTE_PLANAR_BGRI_STREAM_HPP_INCLUDED
#define LIBSTRYKER_EGA_READ_BYTE_PLANAR_BGRI_STREAM_HPP_INCLUDED

#include <libstryker/detail/symbol.hpp>
#include <libstryker/ega/rgb_pixel_grid.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


namespace libstryker::ega
{

LIBSTRYKER_DETAIL_SYMBOL
libstryker::ega::rgb_pixel_grid
read_byte_planar_bgri_stream(
  std::istream &);

}

#endif
