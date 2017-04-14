#include <libstryker/cmp/file_table.hpp>
#include <libstryker/cmp/file_table_entry.hpp>
#include <libstryker/cmp/read_file_table.hpp>
#include <fcppt/algorithm/generate_n.hpp>
#include <fcppt/endianness/format.hpp>
#include <fcppt/io/buffer.hpp>
#include <fcppt/io/read.hpp>
#include <fcppt/io/read.hpp>
#include <fcppt/io/read_chars.hpp>
#include <fcppt/optional/apply.hpp>
#include <fcppt/optional/bind.hpp>
#include <fcppt/optional/cat.hpp>
#include <fcppt/optional/filter.hpp>
#include <fcppt/optional/map.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <string>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace
{
namespace cmp
{

fcppt::optional::object<std::uint32_t>
read_uint32le_from_istream(std::istream &s)
{
  return fcppt::io::read<std::uint32_t>(s,fcppt::endianness::format::little);
}


fcppt::optional::object<std::string>
read_string_from_istream(std::istream &s,std::size_t const n)
{
  // TODO: Use the alda binding here
  return
    fcppt::optional::map(
      fcppt::io::read_chars(s,n),
      [](fcppt::io::buffer const &chars)
      {
        return std::string{
            chars.begin(),
            std::find(chars.begin(),chars.end(),0)};
      });
}

fcppt::optional::object<libstryker::cmp::file_table_entry>
read_single_file_table_entry(
  std::istream &s)
{
  return
    fcppt::optional::bind(
      fcppt::optional::filter(
        cmp::read_string_from_istream(s,12),
        [](std::string const &file_name) { return !file_name.empty(); }),
      [&s](std::string const &file_name)
      {
        fcppt::optional::object<std::uint32_t> const offset{
          cmp::read_uint32le_from_istream(s)};
        fcppt::optional::object<std::uint32_t> const size{
          cmp::read_uint32le_from_istream(s)};
        return fcppt::optional::apply(
          [&file_name](std::uint32_t const _offset, std::uint32_t const _size){
            return
              libstryker::cmp::file_table_entry{
                file_name, _offset, _size};
          },
          offset, size);
      });
}

}
}

libstryker::cmp::file_table
libstryker::cmp::read_file_table(
  std::istream &file_stream)
{
  typedef
  std::vector<fcppt::optional::object<libstryker::cmp::file_table_entry>>
  optional_file_table;

  // We don't know in what state the stream is in - better seek to the
  // beginning
  file_stream.seekg(0, std::ios_base::beg);

  return
    fcppt::optional::cat<libstryker::cmp::file_table>(
      fcppt::algorithm::generate_n<optional_file_table>(
        200u,
       [&file_stream]() {
          return ::cmp::read_single_file_table_entry(file_stream);
       }));
}
