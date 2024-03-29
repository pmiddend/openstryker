#include <libstryker/ega/read_byte_planar_bgri_stream.hpp>
#include <libstryker/ega/rgb_pixel.hpp>
#include <libstryker/ega/rgb_pixel_grid.hpp>
#include <libstryker/ega/rgb_pixel_map.hpp>
#include <fcppt/args.hpp>
#include <fcppt/args_char.hpp>
#include <fcppt/args_vector.hpp>
#include <fcppt/main.hpp>
#include <fcppt/make_int_range.hpp>
#include <fcppt/reference.hpp>
#include <fcppt/string.hpp>
#include <fcppt/algorithm/fold.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/cast/promote_int.hpp>
#include <fcppt/container/at_optional.hpp>
#include <fcppt/container/grid/make_pos_ref_crange_start_end.hpp>
#include <fcppt/container/grid/min.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/container/grid/pos_ref_range.hpp>
#include <fcppt/container/grid/pos_reference.hpp>
#include <fcppt/container/grid/sup.hpp>
#include <fcppt/optional/maybe.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace
{
template<typename T>
std::vector<std::vector<T>>
grid_row_vectors(fcppt::container::grid::object<T,2> const &g)
{
  namespace grid = fcppt::container::grid;
  using fcppt::algorithm::map;

  using input_grid = grid::object<T,2>;

  using input_pos = typename input_grid::pos;

  using input_size_type = typename input_grid::size_type;

  using pos_ref_range = grid::pos_ref_range<input_grid const>;

  using row_vector = std::vector<pos_ref_range>;

  auto const row_ranges =
    map<row_vector>(
      fcppt::make_int_range<input_size_type>(0,g.size().h()),
      [&g](input_size_type const y)
      {
        return
          grid::make_pos_ref_crange_start_end<input_grid>(
              g,
              grid::min<input_size_type,2>(input_pos{0U,y}),
              grid::sup<input_size_type,2>(input_pos(g.size().w(),y+1U)));
      });

  using result_vector = std::vector<std::vector<T>>;

  return
    map<result_vector>(
      row_ranges,
      [](pos_ref_range const &row)
      {
        return
          map<typename result_vector::value_type>(
            row,
            [](grid::pos_reference<input_grid const> const &posref) { return posref.value(); });
      });
}

template<typename RowResult,typename ColResult,typename T,typename FunctionRow,typename FunctionCol>
ColResult
grid_row_col_app(
  fcppt::container::grid::object<T,2> const &g,
  FunctionRow const &frow,
  FunctionCol const &fcol)
{
  return
    fcol(
      fcppt::algorithm::map<std::vector<RowResult>>(
        grid_row_vectors(g),
        frow));
}
}

namespace
{
namespace ega
{
namespace
{

std::string
write_ppm(
  libstryker::ega::rgb_pixel_grid const &g)
{
  std::cout << "P3\n";
  std::cout << g.size().w() << " " << g.size().h() << "\n";
  std::cout << "254\n";
  return
    grid_row_col_app<std::string,std::string>(
      g,
      [](std::vector<libstryker::ega::rgb_pixel<unsigned char>> const &row)
      {
        return
          fcppt::algorithm::fold(
            row,
            std::string{},
            [](libstryker::ega::rgb_pixel<unsigned char> const &p,std::string const &s)
            {
              auto const mapped(
                libstryker::ega::rgb_pixel_map(
                  p,
                  [](unsigned char const e)
                  {
                    return std::to_string(fcppt::cast::promote_int(e));
                  }));

              return s + " " + mapped.r() + " " + mapped.g() + " " + mapped.b();
            });
      },
      [](std::vector<std::string> const &rows)
      {
        return
          fcppt::algorithm::fold(
            rows,
            std::string{},
            [](std::string const &new_row,std::string const &result)
            {
              return result + new_row + "\n";
            });
      });
}
}
}
}

namespace
{
void print_usage()
{
  std::cerr << "usage: ega_reader <ega file name>\n";
  std::cerr << "\noutputs ppm file to stdout\n";
}
}

int FCPPT_MAIN(int argc, fcppt::args_char ** argv)
try
{
  fcppt::args_vector const args(fcppt::args(argc,argv));
  return
    fcppt::optional::maybe(
      fcppt::container::at_optional(args,1U),
      []{
        print_usage();
        return EXIT_FAILURE;
      },
      [](fcppt::reference<fcppt::string const> const file_name)
      {
        std::filesystem::path const ega_file_name{file_name.get()}; // NOLINT(fuchsia-default-arguments-calls)

        std::ifstream fs{ega_file_name}; // NOLINT(fuchsia-default-arguments-calls)

        auto image = libstryker::ega::read_byte_planar_bgri_stream(fs);

        std::cout << ega::write_ppm(image);

        return EXIT_SUCCESS;
      });
}
catch(std::exception const &error)
{
  std::cerr << error.what() << '\n';
  return EXIT_FAILURE;
}
