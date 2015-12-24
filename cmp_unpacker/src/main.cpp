#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <algorithm>
#include <exception>
#include <ostream>
#include <vector>
#include <string>
#include <cstdint>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fcppt/config/external_end.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/maybe.hpp>
#include <fcppt/algorithm/map_concat.hpp>
#include <fcppt/endianness/convert.hpp>
#include <fcppt/endianness/format.hpp>
#include <fcppt/optional_bind.hpp>
#include <fcppt/container/raw_vector.hpp>

namespace cmp
{
typedef std::uint32_t size_type;

struct file_table_entry {
  std::string name_;
  cmp::size_type offset_;
  cmp::size_type size_;

  file_table_entry(
    std::string const _name,
    cmp::size_type const _offset,
    cmp::size_type const _size)
  :
    name_{_name},
    offset_{_offset},
    size_{_size}
  {}

  std::string const name() const  { return name_; }
  cmp::size_type offset() const { return offset_; }
  cmp::size_type size() const { return size_; }
};

typedef
std::vector<file_table_entry>
file_table;

std::uint32_t
read_uint32le_from_istream(std::istream &s)
{
  std::size_t const num_bytes{4};
  char bytes[num_bytes];
  if(!(s.read(&bytes[0],num_bytes)))
    throw std::runtime_error("error reading uint32_t from istream");
  std::uint32_t result;
  std::memcpy(&result,bytes,num_bytes);
  return fcppt::endianness::convert(result,fcppt::endianness::format::little);
}

fcppt::optional<std::string> const
read_string_from_istream(std::istream &s,std::streamsize const n)
{
  typedef fcppt::container::raw_vector<char> char_vector;
  char_vector chars(static_cast<char_vector::size_type>(n));
  if(!(s.read(chars.data(),n)))
    return fcppt::optional<std::string>{};
  return fcppt::optional<std::string>{std::string(chars.begin(),std::find(chars.begin(),chars.end(),0))};
}

fcppt::optional<file_table_entry>
read_single_file_table_entry(
  boost::filesystem::ifstream &s)
{
  // TODO: optional_filter, when it exists
  return
    fcppt::optional_bind(
      read_string_from_istream(s,12),
      [&s](std::string const &file_name)
      {
	return
	  file_name.empty()
	  ?
	    fcppt::optional<file_table_entry>{}
	  :
	    fcppt::optional<file_table_entry>{
	      file_table_entry{
		file_name,
		read_uint32le_from_istream(s),
		read_uint32le_from_istream(s)}};
      });
}

file_table const
read_file_table(
  boost::filesystem::path const &p)
{
  typedef
  std::vector<fcppt::optional<file_table_entry>>
  optional_file_table;

  boost::filesystem::ifstream file_stream{p};

  // TODO: generate_n
  optional_file_table::size_type const max_cmp_fat_entries{200};
  optional_file_table result;
  result.reserve(max_cmp_fat_entries);
  std::generate_n(
    std::back_inserter(result),
    max_cmp_fat_entries-1,
    [&file_stream]() { auto entry = read_single_file_table_entry(file_stream); std::cerr << entry.get_unsafe().name() << "\n"; return entry; });
  // TODO: flat_map for optional containers
  return
    fcppt::algorithm::map_concat<file_table>(
      result,
      [](optional_file_table::value_type const &r)
      {
	return
	  fcppt::maybe(
	    r,
	    []() { return file_table{}; },
	    [](file_table_entry const &e) { return file_table{e}; });
      });
}
}

namespace
{
void print_usage(
  std::string const &program_name)
{
  std::cerr << "usage: " << program_name << " <cmp-file>";
}
}

int
main(
  int const argc,
  char const * const * argv)
{
  std::string const program_name(
    argv[0]);
  if(argc != 2) {
    print_usage(program_name);
    return -1;
  }
  std::string const file_name(argv[1]);
  cmp::file_table const files{cmp::read_file_table(boost::filesystem::path{file_name})};
  std::cout << "found " << files.size() << " file(s)\n";
  std::for_each(files.begin(),files.end(),[](cmp::file_table::value_type const &fte) {
    std::cout << fte.name() << "\n";
  });
}
