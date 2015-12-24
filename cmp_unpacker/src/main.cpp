#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <exception>
#include <ostream>
#include <vector>
#include <string>
#include <cstdint>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fcppt/config/external_end.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/endianness/convert.hpp>
#include <fcppt/endianness/format.hpp>
#include <fcppt/optional_bind_construct.hpp>
#include <fcppt/container/raw_vector.hpp>

namespace cmp
{
typedef std::uint32_t size_type;

struct file_table_entry {
  std::string const name_;
  cmp::size_type const offset_;
  cmp::size_type const size_;

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
  return fcppt::optional<std::string>{std::string(chars.data(),static_cast<std::string::size_type>(n))};
}

fcppt::optional<file_table_entry>
read_single_file_table_entry(
  boost::filesystem::ifstream &s)
{
  return
    fcppt::optional_bind_construct(
      read_string_from_istream(s,12),
      [&s](std::string const &file_name) { return file_table_entry(file_name,read_uint32le_from_istream(s),read_uint32le_from_istream(s)); });
}

/*
file_table const
read_file_table(
  boost::filesystem::path const &p)
{
  boost::filesystem::ifstream file_stream{p};

}
*/
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
  fcppt::optional<cmp::file_table_entry> const entry{cmp::read_single_file_table_entry(file_stream)};
  std::cout << entry.get_unsafe().name() << "\n";
}
