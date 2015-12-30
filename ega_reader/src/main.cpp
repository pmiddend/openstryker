#include <fcppt/container/grid/apply.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/make_int_range.hpp>
#include <fcppt/container/grid/make_pos_ref_crange_start_end.hpp>
#include <fcppt/container/grid/min.hpp>
#include <fcppt/container/grid/pos_reference.hpp>
#include <fcppt/container/grid/sup.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cstddef>
#include <string>
#include <exception>
#include <iostream>
#include <istream>
#include <boost/filesystem.hpp>
#include <ostream>
#include <iterator>
#include <fcppt/config/external_end.hpp>
#include <fcppt/no_init.hpp>

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
    int const c = s.get();
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

template<typename T>
struct rgb_pixel
{
public:
  typedef T value_type;
private:
  value_type r_;
  value_type g_;
  value_type b_;
public:
  rgb_pixel()
  :
    r_{},
    g_{},
    b_{}
  {}

  rgb_pixel(
    value_type const _r,
    value_type const _g,
    value_type const _b)
  :
    r_{_r},
    g_{_g},
    b_{_b}
  {}

  value_type r() const { return r_; }
  
  value_type g() const { return g_; }
  
  value_type b() const { return b_; }
};

template<typename U,typename T,typename Function>
rgb_pixel<U>
rgb_pixel_map(
  rgb_pixel<T> const &p,
  Function const &function)
{
  return
    rgb_pixel<U>{
      function(p.r()),
      function(p.g()),
      function(p.b())};
}

template<typename T>
rgb_pixel<T>
operator*(
  T const m,
  rgb_pixel<T> const &p)
{
  return
    rgb_pixel_map<T>(p,[m](T const t) { return static_cast<T>(m * t); });
}

template<typename T>
rgb_pixel<T>
operator+(
  rgb_pixel<T> const &l,
  rgb_pixel<T> const &r)
{
  return
    rgb_pixel<T>{l.r()+r.r(),l.g()+r.g(),l.b()+r.b()};
}

template<typename U,typename T>
rgb_pixel<U>
rgb_pixel_static_cast(rgb_pixel<T> const &p)
{
  return rgb_pixel_map<U>(p,[](T const t) { return static_cast<U>(t); });
}

// Shoutout to http://www.shikadi.net/moddingwiki/EGA_Palette
rgb_pixel<unsigned char>
bgri_indices_to_pixel(
  bool const b,
  bool const g,
  bool const r,
  bool const i)
{
  return
    r && g && !i && !b
    ?
      rgb_pixel<unsigned char>{0xa8,0x54,0}
    :
      rgb_pixel_static_cast<unsigned char>(
	((i ? 1 : 0) * rgb_pixel<int>{0x54,0x54,0x54}) +
	rgb_pixel_map<int>(
	  rgb_pixel<bool>{r,g,b},
	  [](bool const p) { return p ? 0xa8 : 0; }));
}


template<typename T>
bool
all_equal(T const &)
{
  return true;
}

template<typename T,typename... Args>
bool
all_equal(T const &t,T const &u,Args...args)
{
  return t == u && all_equal(t,args...);
}

typedef
fcppt::container::grid::object<rgb_pixel<unsigned char>,2>
rgb_pixel_grid;

rgb_pixel_grid
read_planar_bgri_stream(std::istream &s)
{
  auto d = rgb_pixel_grid::dim{320u,200u};
  std::streamsize const stride{0};
  return
    fcppt::container::grid::apply(
      bgri_indices_to_pixel,
      read_pixel_plane(s,d,stride),
      read_pixel_plane(s,d,stride),
      read_pixel_plane(s,d,stride),
      read_pixel_plane(s,d,stride));
}

rgb_pixel_grid
read_byte_planar_bgri_stream(std::istream &s)
{
  auto d = rgb_pixel_grid::dim{320u,192u};
//  auto d = rgb_pixel_grid::dim{16u,3480u};
//  auto d = rgb_pixel_grid::dim{16u,3840u};
  std::streamoff const stream_start{s.tellg()};
  std::streamsize const stride{3};
  auto b_plane = read_pixel_plane(s,d,stride);
  s.seekg(stream_start+1,std::ios_base::beg);
  auto g_plane = read_pixel_plane(s,d,stride);
  s.seekg(stream_start+2,std::ios_base::beg);
  auto r_plane = read_pixel_plane(s,d,stride);
  s.seekg(stream_start+3,std::ios_base::beg);
  auto i_plane = read_pixel_plane(s,d,stride);
  return
    fcppt::container::grid::apply(
      bgri_indices_to_pixel,
      b_plane,
      g_plane,
      r_plane,
      i_plane);
}

std::string
write_ppm(
  rgb_pixel_grid const &g)
{
  std::cout << "P3\n";
  std::cout << g.size().w() << " " << g.size().h() << "\n";
  std::cout << "254\n";
  return
    grid_row_col_app<std::string,std::string>(
      g,
      [](std::vector<rgb_pixel<unsigned char>> const &row)
      {
	return
	  fcppt::algorithm::fold(
	    row,
	    std::string{},
	    [](rgb_pixel<unsigned char> const &p,std::string const &s)
	    {
	      return s + " " + std::to_string(static_cast<int>(p.r())) + " " + std::to_string(static_cast<int>(p.g())) + " " + std::to_string(static_cast<int>(p.b()));
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

namespace
{
void print_usage(std::string const &program_name)
{
  std::cerr << "usage: " << program_name << "<ega file name>\n";
  std::cerr << "\noutputs ppm file to stdout\n";
}
}

int main(
  int const argc,
  char * const * const argv)
{
  std::string const program_name{argv[0]};
  if(argc != 2)
  {
    print_usage(program_name);
    return EXIT_FAILURE;
  }

  boost::filesystem::path const ega_file_name{std::string{argv[1]}};

  boost::filesystem::ifstream fs{ega_file_name};

  auto image = ega::read_byte_planar_bgri_stream(fs);

  std::cout << ega::write_ppm(image);
}
