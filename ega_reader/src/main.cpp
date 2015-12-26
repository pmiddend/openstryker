#include <fcppt/container/grid/object.hpp>
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

namespace ega
{
typedef
fcppt::container::grid::object<bool,2>
pixel_plane;

pixel_plane
read_pixel_plane(
  std::istream &s,
  pixel_plane::dim const &dims)
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
    *current_pixel++ = c & 1;
    *current_pixel++ = c & 2;
    *current_pixel++ = c & 4;
    *current_pixel++ = c & 8;
    *current_pixel++ = c & 16;
    *current_pixel++ = c & 32;
    *current_pixel++ = c & 64;
    *current_pixel++ = c & 128;    
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

template<typename U,typename T>
rgb_pixel<U>
rgb_pixel_static_cast(rgb_pixel<T> const &p)
{
  return rgb_pixel_map<U>(p,[](T const t) { return static_cast<U>(t); });
}

// Shoutout to http://www.shikadi.net/moddingwiki/EGA_Palette
rgb_pixel<unsigned char>
rgbi_indices_to_pixel(
  bool const r,
  bool const g,
  bool const b,
  bool const i)
{
  return
    rgb_pixel_static_cast<unsigned char>(
      (i ? 2 : 0) *
      rgb_pixel_map<int>(
	rgb_pixel<bool>{r,g,b},
	[](bool const p) { return p ? 128 : 0; }));
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

template<typename U,typename T,typename Function>
fcppt::container::grid::object<U,2>
grid_apply4(
  fcppt::container::grid::object<T,2> const &a,
  fcppt::container::grid::object<T,2> const &b,
  fcppt::container::grid::object<T,2> const &c,
  fcppt::container::grid::object<T,2> const &d,
  Function const &f)
{
  if(!all_equal(a.size(),b.size(),c.size(),d.size()))
    throw std::runtime_error("invalid grid dimensions, not all equal");

  fcppt::container::grid::object<U,2> result{a.size(),fcppt::no_init()};

  typename fcppt::container::grid::object<U,2>::iterator dest_it = result.begin();
    
  typedef typename
  fcppt::container::grid::object<T,2>::const_iterator
  source_iterator;

  for(
    source_iterator ait = a.cbegin(),bit = b.cbegin(),cit = c.cbegin(),dit = d.cbegin();
    ait != c.cend();
    ++ait,++bit,++cit,++dit) {
    *dest_it++ = f(*ait,*bit,*cit,*dit);
  }

  return result;
}

typedef
fcppt::container::grid::object<rgb_pixel<unsigned char>,2>
rgb_pixel_grid;

rgb_pixel_grid
read_planar_bgri_stream(std::istream &s)
{
  auto d = rgb_pixel_grid::dim{320u,200u};
  return
    grid_apply4<rgb_pixel<unsigned char>>(
      read_pixel_plane(s,d),
      read_pixel_plane(s,d),
      read_pixel_plane(s,d),
      read_pixel_plane(s,d),
      rgbi_indices_to_pixel);
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

  
}
