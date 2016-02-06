#ifndef LIBSTRYKER_CMP_READ_CMP_ENTRY_HPP_INCLUDED
#define LIBSTRYKER_CMP_READ_CMP_ENTRY_HPP_INCLUDED

#include <libstryker/detail/symbol.hpp>
#include <fcppt/container/raw_vector.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace cmp
{

struct file_table_entry;

LIBSTRYKER_DETAIL_SYMBOL
fcppt::container::raw_vector<char>
read_cmp_entry(
  libstryker::cmp::file_table_entry const &,
  std::istream &);

}
}

#endif
