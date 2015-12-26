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
#include <fcppt/optional_map.hpp>
#include <fcppt/algorithm/generate_n.hpp>
#include <fcppt/algorithm/cat_optionals.hpp>
#include <fcppt/maybe.hpp>
#include <fcppt/algorithm/map_concat.hpp>
#include <fcppt/endianness/convert.hpp>
#include <fcppt/endianness/format.hpp>
#include <fcppt/optional_filter.hpp>
#include <fcppt/container/raw_vector.hpp>

namespace cmp
{
typedef std::uint32_t size_type;

struct file_table_entry {
private:
  std::string name_;
  cmp::size_type offset_;
  cmp::size_type size_;
public:
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

fcppt::optional<std::string>
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
  std::istream &s)
{
  return
    fcppt::optional_map(
      fcppt::optional_filter(
	read_string_from_istream(s,12),
	[](std::string const &file_name) { return !file_name.empty(); }),
      [&s](std::string const &file_name)
      {
	return file_table_entry{
		file_name,
		read_uint32le_from_istream(s),
		read_uint32le_from_istream(s)};
      });
}

file_table
read_file_table(
  std::istream &file_stream)
{
  typedef
  std::vector<fcppt::optional<file_table_entry>>
  optional_file_table;

  // We don't know in what state the stream is in - better seek to the
  // beginning
  file_stream.seekg(0, std::ios_base::beg);

  return
    fcppt::algorithm::cat_optionals<file_table>(
      fcppt::algorithm::generate_n<optional_file_table>(
	200u,
	[&file_stream]() {
	  return read_single_file_table_entry(file_stream);
	}));
}

fcppt::container::raw_vector<char>
read_cmp_entry(
  file_table_entry const &entry,
  std::istream &file_stream)
{
  fcppt::container::raw_vector<char> result(entry.size());
  file_stream.seekg(entry.offset(), std::ios_base::beg);
  file_stream.read(result.data(),entry.size());
  return result;
}

void
write_vector_to_filesystem(
  fcppt::container::raw_vector<char> const &v,
  boost::filesystem::path const &base_path)
{
  std::cout << "Writing " << base_path << "\n";
  boost::filesystem::ofstream{base_path}.write(v.data(), static_cast<std::streamsize>(v.size()));
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
  boost::filesystem::ifstream file_stream{boost::filesystem::path{file_name}};
  cmp::file_table const files{cmp::read_file_table(file_stream)};
  std::cout << "found " << files.size() << " file(s)\n";
  boost::filesystem::path base_path{"data/"};
  std::for_each(
    files.begin(),
    files.end(),
    [&base_path,&file_stream](cmp::file_table::value_type const &fte)
    {
      std::cout << "Writing " << fte.name() << " to data/\n";
      cmp::write_vector_to_filesystem(
	read_cmp_entry(fte,file_stream),
	base_path / fte.name());
  });
}
