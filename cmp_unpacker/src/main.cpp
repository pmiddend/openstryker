#include <libstryker/cmp/file_table.hpp>
#include <libstryker/cmp/read_cmp_entry.hpp>
#include <libstryker/cmp/read_file_table.hpp>
#include <fcppt/args.hpp>
#include <fcppt/args_vector.hpp>
#include <fcppt/reference.hpp>
#include <fcppt/string.hpp>
#include <fcppt/algorithm/loop.hpp>
#include <fcppt/io/buffer.hpp>
#include <fcppt/container/at_optional.hpp>
#include <fcppt/optional/maybe.hpp>
#include <fcppt/config/external_begin.hpp>
#include <exception>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <filesystem>
#include <fcppt/config/external_end.hpp>

namespace
{
namespace cmp
{
namespace
{

void
write_vector_to_filesystem(
  fcppt::io::buffer const &v,
  std::filesystem::path const &base_path)
{
  std::cout << "Writing " << base_path << "\n";
  std::ofstream stream{base_path};
  stream.write(v.data(), static_cast<std::streamsize>(v.size()));
  if(!stream)
    std::cerr << "Failed writing " << base_path << '\n';
}
}
}
}

namespace
{
void print_usage()
{
  std::cerr << "usage: cmd_unpacker <cmp-file>\n";
}
}

int
main(int argc, char ** argv)
try
{
  fcppt::args_vector const args(fcppt::args(argc,argv));
  return
    fcppt::optional::maybe(
      fcppt::container::at_optional(args,1u),
      []{
        print_usage();
        return EXIT_FAILURE;
      },
      [](fcppt::reference<fcppt::string const> file_name)
      {
        std::ifstream file_stream{std::filesystem::path{file_name.get()}};
        libstryker::cmp::file_table const files{libstryker::cmp::read_file_table(file_stream)};
        std::cout << "found " << files.size() << " file(s)\n";
        std::filesystem::path base_path{"data/"};
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
      });
}
catch(std::exception const &error)
{
  std::cerr << error.what() << '\n';
  return EXIT_FAILURE;
}
