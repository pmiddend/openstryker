#ifndef LIBSTRYKER_LEVEL_RECORD_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_RECORD_HPP_INCLUDED

#include <libstryker/level/string_decl.hpp>
#include <alda/bindings/array_decl.hpp>
#include <alda/bindings/dynamic_len_decl.hpp>
#include <alda/bindings/fundamental_decl.hpp>
#include <alda/bindings/unsigned_decl.hpp>
#include <alda/bindings/static_decl.hpp>
#include <alda/raw/record_impl.hpp>
#include <alda/raw/record_variadic_fwd.hpp>
#include <majutsu/make_role_tag.hpp>
#include <majutsu/role.hpp>
#include <fcppt/endianness/format.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/config/external_begin.hpp>
#include <array>
#include <cstddef>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace level
{

typedef
std::array<
  std::uint16_t,
  16767
>
tile_array;

typedef
libstryker::level::string<
  12
>
string_binding;

typedef
alda::bindings::unsigned_<
  std::uint16_t,
  fcppt::endianness::format::little
>
ui16le_binding;

MAJUTSU_MAKE_ROLE_TAG(
  actor_type_role
);

MAJUTSU_MAKE_ROLE_TAG(
  actor_pos_role
);

typedef
fcppt::math::vector::static_<
  std::uint16_t,
  2
>
actor_pos;

typedef
alda::bindings::static_<
  actor_pos,
  ui16le_binding
>
actor_pos_binding;

typedef
alda::raw::record_variadic<
  majutsu::role<
    ui16le_binding,
    actor_type_role
  >,
  majutsu::role<
    actor_pos_binding,
    actor_pos_role
  >
>
actor_record;

typedef
std::vector<
  actor_record
>
actor_vector;

struct actor_size_policy
{
  static
  std::uint16_t
  make(
    std::uint16_t const _size
  )
  {
    return
      static_cast<
        std::uint16_t
      >(
        _size
        /
        3u
      );
  }
};

typedef
alda::bindings::dynamic_len<
  actor_vector,
  actor_record,
  ui16le_binding,
  actor_size_policy
>
actor_binding;

typedef
alda::bindings::array<
  std::array<
    std::uint8_t,
    480
  >,
  alda::bindings::fundamental<
    std::uint8_t
  >
>
unknown_binding;

typedef
alda::bindings::array<
  tile_array,
  ui16le_binding
>
tile_array_binding;

MAJUTSU_MAKE_ROLE_TAG(
  mask_tiles_role
);

MAJUTSU_MAKE_ROLE_TAG(
  solid_tiles_role
);

MAJUTSU_MAKE_ROLE_TAG(
  backdrop_role
);

MAJUTSU_MAKE_ROLE_TAG(
  unused1_role
);

MAJUTSU_MAKE_ROLE_TAG(
  unused2_role
);

MAJUTSU_MAKE_ROLE_TAG(
  music_role
);

MAJUTSU_MAKE_ROLE_TAG(
  unused3_role
);

MAJUTSU_MAKE_ROLE_TAG(
  flags_role
);

MAJUTSU_MAKE_ROLE_TAG(
  width_role
);

MAJUTSU_MAKE_ROLE_TAG(
  actor_role
);

MAJUTSU_MAKE_ROLE_TAG(
  unknown_role
);

MAJUTSU_MAKE_ROLE_TAG(
  tile_role
);

typedef
alda::raw::record_variadic<
  majutsu::role<
    string_binding,
    mask_tiles_role
  >,
  majutsu::role<
    string_binding,
    solid_tiles_role
  >,
  majutsu::role<
    string_binding,
    backdrop_role
  >,
  majutsu::role<
    string_binding,
    unused1_role
  >,
  majutsu::role<
    string_binding,
    unused2_role
  >,
  majutsu::role<
    string_binding,
    music_role
  >,
  majutsu::role<
    string_binding,
    unused3_role
  >,
  majutsu::role<
    ui16le_binding,
    flags_role
  >,
  majutsu::role<
    ui16le_binding,
    width_role
  >,
  majutsu::role<
    actor_binding,
    actor_role
  >,
  majutsu::role<
    unknown_binding,
    unknown_role
  >,
  majutsu::role<
    tile_array_binding,
    tile_role
  >
>
record;

}
}

#endif
