#ifndef LIBSTRYKER_LEVEL_READ_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_READ_HPP_INCLUDED

#include <libstryker/detail/symbol.hpp>
#include <libstryker/level/record.hpp>
#include <fcppt/optional/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace level
{

LIBSTRYKER_DETAIL_SYMBOL
fcppt::optional::object<libstryker::level::record>
read(
  std::istream &);

}
}

#endif
