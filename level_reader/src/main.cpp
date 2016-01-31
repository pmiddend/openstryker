#include <fcppt/io/read.hpp>
#include <fcppt/either/apply.hpp>
#include <fcppt/algorithm/generate_n.hpp>
#include <fcppt/container/raw_vector.hpp>
#include <fcppt/optional/apply.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/optional/bind.hpp>
#include <fcppt/either/object.hpp>
#include <fcppt/either/map.hpp>
#include <fcppt/either/cat.hpp>
#include <fcppt/either/bind.hpp>
#include <fcppt/either/from_optional.hpp>
#include <fcppt/either/apply.hpp>
#include <fcppt/make_strong_typedef.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <istream>
#include <vector>
#include <array>
#include <fcppt/config/external_end.hpp>

namespace
{
FCPPT_MAKE_STRONG_TYPEDEF(
  std::string,
  error_string);

template<std::streamsize N>
fcppt::either::object<error_string,std::string>
read_string(std::istream &s,std::string const &_error)
{
  char chars[N];
  return
    s.read(chars,N)
    ?
      fcppt::either::object<error_string,std::string>{std::string(chars,std::find(chars,chars+N,0))}
    :
      fcppt::either::object<error_string,std::string>{error_string{_error}};
}

template<typename T>
fcppt::either::object<error_string,fcppt::container::raw_vector<T>>
read_array(
  std::istream &s,
  std::size_t const n,
  fcppt::endianness::format const _format,
  error_string const &_error)
{
  fcppt::container::raw_vector<char> chars(n * sizeof(T));

  typedef
  fcppt::container::raw_vector<T>
  result_container;

  if(!s.read(chars.data(),static_cast<std::streamsize>(chars.size())))
    return fcppt::either::object<error_string,result_container>{_error};

  result_container result(n);
  for(std::size_t i = 0; i < chars.size();i += sizeof(T)) {
    std::memcpy(&result[i/sizeof(T)],&chars[i],sizeof(T));
    result[i/sizeof(T)] = fcppt::endianness::convert(result[i/sizeof(T)],_format);
  }
  return fcppt::either::object<error_string,result_container>{result};
}

}

namespace msl
{
struct level_actor
{
public:
  typedef std::uint16_t id_type;
  typedef std::uint16_t pos_type;
private:
  id_type id_;
  pos_type pos_x_;
  pos_type pos_y_;
public:
  level_actor(
    id_type const _id,
    pos_type const _pos_x,
    pos_type const _pos_y)
  :
    id_{_id},
    pos_x_{_pos_x},
    pos_y_{_pos_y}
  {}
};

struct level
{
public:
  typedef
  std::uint16_t
  actor_container_length_type;

  typedef
  std::vector<level_actor>
  actor_container;

  typedef
  std::array<std::uint8_t,480u>
  unknown_container;

  typedef
  fcppt::container::raw_vector<std::uint16_t>
  tile_container;

  typedef
  std::uint16_t
  light_flag_type;

  typedef
  std::uint16_t
  width_type;
private:
  std::string mask_tiles_;
  std::string solid_tiles_;
  std::string backdrop_;
  std::string music_;
  light_flag_type light_flags_;
  actor_container actors_;
  unknown_container unknown_;
  tile_container tile_codes_;
public:
  level(
    std::string const &_mask_tiles,
    std::string const &_solid_tiles,
    std::string const &_backdrop,
    std::string const &,
    std::string const &,
    std::string const &_music,
    std::string const &,
    light_flag_type const _light_flags,
    width_type const _width,
    actor_container const &_actors,
    unknown_container const &_unknown,
    tile_container const &_tile_codes)
  :
    mask_tiles_{_mask_tiles},
    solid_tiles_{_solid_tiles},
    backdrop_{_backdrop},
    music_{_music},
    light_flags_{_light_flags},
    actors_{_actors},
    unknown_{_unknown},
    tile_codes_{_tile_codes}
  {}

  std::string const &mask_tiles() const { return mask_tiles_; }
  std::string const &solid_tiles() const { return solid_tiles_; }
  std::string const &backdrop() const { return backdrop_; }
  std::string const &music() const { return music_; }
  light_flag_type light_flags() const { return light_flags_; }
  actor_container const &actors() const { return actors_; }
  unknown_container const &unknown() const { return unknown_; }
  tile_container const &tile_codes() const { return tile_codes_; }
};

template<typename Type,typename Error>
fcppt::either::object<Error,Type>
read_to_either(
	std::istream &_stream,
	fcppt::endianness::format const _format,
	Error const &_error) {
  return
    fcppt::either::from_optional(
      fcppt::io::read<Type>(_stream,_format),
      [&_error]() { return _error; });
}

fcppt::either::object<error_string,level::unknown_container>
load_unknown(std::istream &s)
{
  level::unknown_container chars;
  return
    s.read(reinterpret_cast<char *>(chars.data()),chars.size())
    ?
      fcppt::either::object<error_string,level::unknown_container>{chars}
    :
      fcppt::either::object<error_string,level::unknown_container>{error_string{"unknown"}};

}

fcppt::either::object<error_string,level_actor>
load_actor(std::istream &s)
{
  auto id =
    read_to_either<level_actor::id_type>(
      s,
      fcppt::endianness::format::little,
      error_string{"actor id"});
  auto pos_x =
    read_to_either<level_actor::pos_type>(
      s,
      fcppt::endianness::format::little,
      error_string{"actor_pos_x"});
  auto pos_y =
    read_to_either<level_actor::pos_type>(
      s,
      fcppt::endianness::format::little,
      error_string{"actor_pos_y"});
  return
    fcppt::either::apply(
      [](
	level_actor::id_type const _id,
	level_actor::pos_type const _pos_x,
	level_actor::pos_type const _pos_y)
      {
	return level_actor{_id,_pos_x,_pos_y};
      },
      id,
      pos_x,
      pos_y);
}

fcppt::either::object<error_string,level::actor_container>
load_actors(std::istream &s)
{
  return
      fcppt::either::bind(
	read_to_either<level::actor_container_length_type>(
	  s,
	  fcppt::endianness::format::little,
	  error_string{"actor_count"}),
	[&s](level::actor_container_length_type const actor_count)
	{
	  return
	    fcppt::either::cat<level::actor_container>(
	      fcppt::algorithm::generate_n<std::vector<fcppt::either::object<error_string,level_actor>>>(
		static_cast<std::size_t>(actor_count),
		[&s]() { return load_actor(s); }));
	});
}

fcppt::either::object<error_string,level>
load_level(std::istream &s)
{
  auto mask_tiles = read_string<12>(s,"mask_tiles");
  auto solid_tiles = read_string<12>(s,"solid_tiles");
  auto backdrop = read_string<12>(s,"backdrop");
  auto unused1 = read_string<12>(s,"unused 1");
  auto unused2 = read_string<12>(s,"unused 2");
  auto music = read_string<12>(s,"music");
  auto unused3 = read_string<12>(s,"unused 3");
  auto light_flags = read_to_either<level::light_flag_type>(s,fcppt::endianness::format::little,error_string{"light_flags"});
  auto width = read_to_either<level::width_type>(s,fcppt::endianness::format::little,error_string{"width_flags"});
  auto actors = load_actors(s);
  auto unknown = load_unknown(s);
  auto tiles = read_array<std::uint16_t>(s,16767u,fcppt::endianness::format::little,error_string{"tiles"});
  return
    fcppt::either::apply(
    [](
      std::string const &mask_tiles,
      std::string const &solid_tiles,
      std::string const &backdrop,
      std::string const &unused1,
      std::string const &unused2,
      std::string const &music,
      std::string const &unused3,
      level::light_flag_type const light_flags,
      level::width_type const width,
      level::actor_container const &actors,
      level::unknown_container const &unknown,
      level::tile_container const &tiles) {
	return level{mask_tiles,solid_tiles,backdrop,unused1,unused2,music,unused3,light_flags,width,actors,unknown,tiles};
      },
      mask_tiles,
      solid_tiles,
      backdrop,
      unused1,
      unused2,
      music,
      unused3,
      light_flags,
      width,
      actors,
      unknown,
      tiles);
}
}

int main(
  int const argc,
  char const * const *argv)
{

}
