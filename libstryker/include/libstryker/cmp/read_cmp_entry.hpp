#ifndef LIBSTRYKER_CMP_READ_CMP_ENTRY_HPP_INCLUDED
#define LIBSTRYKER_CMP_READ_CMP_ENTRY_HPP_INCLUDED

#include <libstryker/detail/symbol.hpp>
#include <fcppt/io/buffer.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iosfwd>
#include <fcppt/config/external_end.hpp>


namespace libstryker::cmp
{

struct file_table_entry;

LIBSTRYKER_DETAIL_SYMBOL
fcppt::io::buffer
read_cmp_entry(
  libstryker::cmp::file_table_entry const &,
  std::istream &);

}

#endif
