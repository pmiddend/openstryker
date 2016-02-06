#ifndef LIBSTRYKER_CMP_READ_FILE_TABLE_HPP_INCLUDED
#define LIBSTRYKER_CMP_READ_FILE_TABLE_HPP_INCLUDED

#include <libstryker/cmp/file_table.hpp>
#include <libstryker/detail/symbol.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace cmp
{

LIBSTRYKER_DETAIL_SYMBOL
libstryker::cmp::file_table
read_file_table(
  std::istream &);

}
}

#endif
