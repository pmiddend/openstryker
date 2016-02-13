#include <libstryker/cmp/file_table.hpp>
#include <libstryker/cmp/read_cmp_entry.hpp>
#include <libstryker/cmp/read_file_table.hpp>
#include <fcppt/algorithm/loop.hpp>
#include <fcppt/io/buffer.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fcppt/config/external_end.hpp>

namespace
{
namespace cmp
{

void
write_vector_to_filesystem(
  fcppt::io::buffer const &v,
  boost::filesystem::path const &base_path)
{
  std::cout << "Writing " << base_path << "\n";
  boost::filesystem::ofstream stream{base_path};
  stream.write(v.data(), static_cast<std::streamsize>(v.size()));
  if(!stream)
    std::cerr << "Failed writing " << base_path << '\n';
}
}
}

namespace
{
void print_usage(
  std::string const &program_name)
{
  std::cerr << "usage: " << program_name << " <cmp-file>\n";
}
}

int
main(
  int argc,
  char ** argv)
{
  std::string const program_name(
    argv[0]);
  if(argc != 2) {
    print_usage(program_name);
    return EXIT_FAILURE;
  }
  std::string const file_name(argv[1]);
  boost::filesystem::ifstream file_stream{boost::filesystem::path{file_name}};
  libstryker::cmp::file_table const files{libstryker::cmp::read_file_table(file_stream)};
  std::cout << "found " << files.size() << " file(s)\n";
  boost::filesystem::path base_path{"data/"};
  fcppt::algorithm::loop(
    files,
    [&base_path,&file_stream](libstryker::cmp::file_table::value_type const &fte)
    {
      std::cout << "Writing " << fte.name() << " to data/\n";
      cmp::write_vector_to_filesystem(
	libstryker::cmp::read_cmp_entry(fte,file_stream),
	base_path / fte.name());
  });
  return EXIT_SUCCESS;
}
