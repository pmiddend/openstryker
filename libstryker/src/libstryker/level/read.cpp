#include <libstryker/level/read.hpp>
#include <libstryker/level/record.hpp>
#include <libstryker/level/string.hpp> // NOLINT(misc-include-cleaner)
#include <alda/bindings/array.hpp> // NOLINT(misc-include-cleaner)
#include <alda/bindings/dynamic_len.hpp> // NOLINT(misc-include-cleaner)
#include <alda/bindings/fundamental.hpp> // NOLINT(misc-include-cleaner)
#include <alda/bindings/static.hpp> // NOLINT(misc-include-cleaner)
#include <alda/bindings/unsigned.hpp> // NOLINT(misc-include-cleaner)
#include <alda/raw/make_generic.hpp>
#include <alda/raw/stream/error.hpp>
#include <alda/raw/stream/istream.hpp>
#include <fcppt/either/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


fcppt::either::object<
  alda::raw::stream::error,
  libstryker::level::record>
libstryker::level::read(
  std::istream &stream)
{
  return
    alda::raw::make_generic<
      alda::raw::stream::istream,
      libstryker::level::record_binding
    >(
      stream);
}
