#ifndef LIBSTRYKER_LEVEL_READ_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_READ_HPP_INCLUDED

#include <libstryker/detail/symbol.hpp>
#include <libstryker/level/record.hpp>
#include <alda/raw/stream/error.hpp>
#include <fcppt/either/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


namespace libstryker::level
{

LIBSTRYKER_DETAIL_SYMBOL
fcppt::either::object<
  alda::raw::stream::error,
  libstryker::level::record>
read(
  std::istream &);

}

#endif
