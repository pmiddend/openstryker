#ifndef LIBSTRYKER_CMP_FILE_TABLE_HPP_INCLUDED
#define LIBSTRYKER_CMP_FILE_TABLE_HPP_INCLUDED

#include <libstryker/cmp/file_table_entry.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace cmp
{

using file_table =
std::vector<libstryker::cmp::file_table_entry>;

}
}

#endif
