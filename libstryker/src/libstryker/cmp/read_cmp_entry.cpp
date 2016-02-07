#include <libstryker/cmp/file_table_entry.hpp>
#include <libstryker/cmp/read_cmp_entry.hpp>
#include <fcppt/io/buffer.hpp>
#include <fcppt/io/read_chars.hpp>
#include <fcppt/optional/to_exception.hpp>
#include <fcppt/config/external_begin.hpp>
#include <istream>
#include <stdexcept>
#include <fcppt/config/external_end.hpp>


fcppt::io::buffer
libstryker::cmp::read_cmp_entry(
  libstryker::cmp::file_table_entry const &entry,
  std::istream &file_stream)
{
  file_stream.seekg(entry.offset(), std::ios_base::beg);
  return
    fcppt::optional::to_exception(
      fcppt::io::read_chars(file_stream,entry.size()),
      []{
        return std::runtime_error("Failed to read a file table entry");
      });
}
