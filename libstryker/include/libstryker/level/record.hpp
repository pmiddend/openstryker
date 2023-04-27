#ifndef LIBSTRYKER_LEVEL_RECORD_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_RECORD_HPP_INCLUDED

#include <libstryker/level/string_decl.hpp>
#include <alda/bindings/array_decl.hpp>
#include <alda/bindings/dynamic_len_decl.hpp>
#include <alda/bindings/fundamental_decl.hpp>
#include <alda/bindings/record_variadic.hpp>
#include <alda/bindings/static_decl.hpp>
#include <alda/bindings/unsigned_decl.hpp>
#include <alda/raw/element_type.hpp>
#include <fcppt/array/object_impl.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/record/element.hpp>
#include <fcppt/record/make_label.hpp>
#include <fcppt/config/external_begin.hpp>
#include <bit>
#include <cstdint>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace libstryker::level
{

using tile_array =
fcppt::array::object<
  std::uint16_t,
  16767 // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
>;

using string_binding =
libstryker::level::string<
  12 // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
>;

using ui16le_binding =
alda::bindings::unsigned_<
  std::uint16_t,
  std::endian::little
>;

FCPPT_RECORD_MAKE_LABEL(
  actor_type_role
);

FCPPT_RECORD_MAKE_LABEL(
  actor_pos_role
);

using actor_pos =
fcppt::math::vector::static_<
  std::uint16_t,
  2
>;

using actor_pos_binding =
alda::bindings::static_<
  actor_pos,
  ui16le_binding
>;

using actor_record_binding =
alda::bindings::record_variadic<
  fcppt::record::element<
    actor_type_role,
    ui16le_binding
  >,
  fcppt::record::element<
    actor_pos_role,
    actor_pos_binding
  >
>;

using actor_record =
alda::raw::element_type<
  actor_record_binding
>;

using actor_vector =
std::vector<
  actor_record
>;

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
      >(_size / 3U);
  }
};

using actor_binding =
alda::bindings::dynamic_len<
  actor_vector,
  actor_record_binding,
  ui16le_binding,
  actor_size_policy
>;

using unknown_binding =
alda::bindings::array<
  fcppt::array::object<
    std::uint8_t,
    480 // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  >,
  alda::bindings::fundamental<
    std::uint8_t
  >
>;

using tile_array_binding =
alda::bindings::array<
  tile_array,
  ui16le_binding
>;

FCPPT_RECORD_MAKE_LABEL(
  mask_tiles_role
);

FCPPT_RECORD_MAKE_LABEL(
  solid_tiles_role
);

FCPPT_RECORD_MAKE_LABEL(
  backdrop_role
);

FCPPT_RECORD_MAKE_LABEL(
  unused1_role
);

FCPPT_RECORD_MAKE_LABEL(
  unused2_role
);

FCPPT_RECORD_MAKE_LABEL(
  music_role
);

FCPPT_RECORD_MAKE_LABEL(
  unused3_role
);

FCPPT_RECORD_MAKE_LABEL(
  flags_role
);

FCPPT_RECORD_MAKE_LABEL(
  width_role
);

FCPPT_RECORD_MAKE_LABEL(
  actor_role
);

FCPPT_RECORD_MAKE_LABEL(
  unknown_role
);

FCPPT_RECORD_MAKE_LABEL(
  tile_role
);

using record_binding =
alda::bindings::record_variadic<
  fcppt::record::element<
    mask_tiles_role,
    string_binding
  >,
  fcppt::record::element<
    solid_tiles_role,
    string_binding
  >,
  fcppt::record::element<
    backdrop_role,
    string_binding
  >,
  fcppt::record::element<
    unused1_role,
    string_binding
  >,
  fcppt::record::element<
    unused2_role,
    string_binding
  >,
  fcppt::record::element<
    music_role,
    string_binding
  >,
  fcppt::record::element<
    unused3_role,
    string_binding
  >,
  fcppt::record::element<
    flags_role,
    ui16le_binding
  >,
  fcppt::record::element<
    width_role,
    ui16le_binding
  >,
  fcppt::record::element<
    actor_role,
    actor_binding
  >,
  fcppt::record::element<
    unknown_role,
    unknown_binding
  >,
  fcppt::record::element<
    tile_role,
    tile_array_binding
  >
>;

using record =
alda::raw::element_type<
  record_binding
>;

}

#endif
