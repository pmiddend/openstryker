#include <libstryker/cmp/file_table.hpp>
#include <libstryker/cmp/file_table_entry.hpp>
#include <libstryker/cmp/read_file_table.hpp>
#include <fcppt/algorithm/cat_optionals.hpp>
#include <fcppt/algorithm/generate_n.hpp>
#include <fcppt/container/raw_vector.hpp>
#include <fcppt/endianness/format.hpp>
#include <fcppt/io/read_exn.hpp>
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

std::uint32_t
read_uint32le_from_istream(std::istream &s)
{
  return fcppt::io::read_exn<std::uint32_t>(s,fcppt::endianness::format::little);
}


fcppt::optional::object<std::string>
read_string_from_istream(std::istream &s,std::streamsize const n)
{
  typedef fcppt::container::raw_vector<char> char_vector;
  char_vector chars(static_cast<char_vector::size_type>(n));
  if(!(s.read(chars.data(),n)))
    return fcppt::optional::object<std::string>{};
  return fcppt::optional::object<std::string>{std::string(chars.begin(),std::find(chars.begin(),chars.end(),0))};
}

fcppt::optional::object<libstryker::cmp::file_table_entry>
read_single_file_table_entry(
  std::istream &s)
{
  return
    fcppt::optional::map(
      fcppt::optional::filter(
	cmp::read_string_from_istream(s,12),
	[](std::string const &file_name) { return !file_name.empty(); }),
      [&s](std::string const &file_name)
      {
	return libstryker::cmp::file_table_entry{
		file_name,
		cmp::read_uint32le_from_istream(s),
		cmp::read_uint32le_from_istream(s)};
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
    fcppt::algorithm::cat_optionals<libstryker::cmp::file_table>(
      fcppt::algorithm::generate_n<optional_file_table>(
	200u,
	[&file_stream]() {
	  return ::cmp::read_single_file_table_entry(file_stream);
	}));
}
