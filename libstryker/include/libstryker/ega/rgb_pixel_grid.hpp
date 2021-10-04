#ifndef LIBSTRYKER_EGA_RGB_PIXEL_GRID_HPP_INCLUDED
#define LIBSTRYKER_EGA_RGB_PIXEL_GRID_HPP_INCLUDED

#include <libstryker/ega/rgb_pixel.hpp>
#include <fcppt/container/grid/object.hpp>


namespace libstryker::ega
{

using rgb_pixel_grid =
fcppt::container::grid::object<libstryker::ega::rgb_pixel<unsigned char>,2>;

}

#endif
