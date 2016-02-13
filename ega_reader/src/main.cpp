#include <libstryker/ega/read_byte_planar_bgri_stream.hpp>
#include <libstryker/ega/rgb_pixel.hpp>
#include <libstryker/ega/rgb_pixel_grid.hpp>
#include <libstryker/ega/rgb_pixel_map.hpp>
#include <fcppt/make_int_range.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/cast/promote.hpp>
#include <fcppt/container/grid/make_pos_ref_crange_start_end.hpp>
#include <fcppt/container/grid/min.hpp>
#include <fcppt/container/grid/pos_reference.hpp>
#include <fcppt/container/grid/sup.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <cstdlib>
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
  using namespace fcppt::container::grid;
  using fcppt::algorithm::map;

  typedef
  object<T,2>
  input_grid;

  typedef typename
  input_grid::pos
  input_pos;

  typedef typename
  input_grid::size_type
  input_size_type;

  typedef
  pos_ref_range<input_grid const>
  pos_ref_range;

  typedef
  std::vector<pos_ref_range>
  row_vector;

  row_vector const row_ranges =
    map<row_vector>(
      fcppt::make_int_range<input_size_type>(0,g.size().h()),
      [&g](input_size_type const y)
      {
        return
	  make_pos_ref_crange_start_end<input_grid>(
	      g,
	      min<input_size_type,2>(input_pos{0u,y}),
	      sup<input_size_type,2>(input_pos(g.size().w(),y+1u)));
      });

  typedef
  std::vector<std::vector<T>>
  result_vector;

  return
    map<result_vector>(
      row_ranges,
      [](pos_ref_range const &row)
      {
	return
	  map<typename result_vector::value_type>(
	    row,
	    [](pos_reference<input_grid const> const &posref) { return posref.value(); });
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
                    return std::to_string(fcppt::cast::promote(e));
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

namespace
{
void print_usage(std::string const &program_name)
{
  std::cerr << "usage: " << program_name << "<ega file name>\n";
  std::cerr << "\noutputs ppm file to stdout\n";
}
}

int main(
  int argc,
  char ** argv)
{
  std::string const program_name{argv[0]};
  if(argc != 2)
  {
    print_usage(program_name);
    return EXIT_FAILURE;
  }

  boost::filesystem::path const ega_file_name{std::string{argv[1]}};

  boost::filesystem::ifstream fs{ega_file_name};

  auto image = libstryker::ega::read_byte_planar_bgri_stream(fs);

  std::cout << ega::write_ppm(image);

  return EXIT_SUCCESS;
}
