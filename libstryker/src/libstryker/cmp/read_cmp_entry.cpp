#include <libstryker/cmp/file_table_entry.hpp>
#include <libstryker/cmp/read_cmp_entry.hpp>
#include <fcppt/config/external_begin.hpp>
#include <istream>
#include <stdexcept>
#include <fcppt/config/external_end.hpp>


fcppt::container::raw_vector<char>
libstryker::cmp::read_cmp_entry(
  libstryker::cmp::file_table_entry const &entry,
  std::istream &file_stream)
{
  fcppt::container::raw_vector<char> result(entry.size());
  file_stream.seekg(entry.offset(), std::ios_base::beg);
  if(!file_stream.read(result.data(),entry.size()))
    throw std::runtime_error("Failed to read a file table entry");
  return result;
}
