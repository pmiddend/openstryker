#include <libstryker/level/read.hpp>
#include <libstryker/level/record.hpp>
#include <alda/raw/stream/error.hpp>
#include <fcppt/args.hpp>
#include <fcppt/args_vector.hpp>
#include <fcppt/reference.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/container/at_optional.hpp>
#include <fcppt/either/match.hpp>
#include <fcppt/optional/maybe.hpp>
#include <fcppt/record/output.hpp>
#include <fcppt/config/external_begin.hpp>
#include <filesystem>
#include <array>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <fcppt/config/external_end.hpp>

namespace std
{
template<typename T, std::size_t N>
std::ostream &operator<<(std::ostream &_stream,std::array<T,N> const &_array)
{
  _stream << '(';
  for(T const &val : _array)
    _stream << val << ',';
  return _stream << ')';
}
template<typename T>
std::ostream &operator<<(std::ostream &_stream,std::vector<T> const &_array)
{
  _stream << '(';
  for(T const &val : _array)
    _stream << val << ',';
  return _stream << ')';
}
}

int main(int argc, char **argv)
try
{
  fcppt::args_vector const args(fcppt::args(argc,argv));
  return
    fcppt::optional::maybe(
      fcppt::container::at_optional(args,1u),
      []{
        std::cerr << "usage: level_reader <level-file>\n";
        return EXIT_FAILURE;
      },
      [](fcppt::reference<fcppt::string const> file_name)
      {
        std::filesystem::path const file_path{file_name.get()};
        std::ifstream file_stream{file_path};
        return
          fcppt::either::match(
            libstryker::level::read(file_stream),
            [](alda::raw::stream::error const &_error)
            {
              fcppt::io::cerr() << FCPPT_TEXT("Reading failed: ") << _error.get() << FCPPT_TEXT('\n');
              return EXIT_FAILURE;
            },
            [](libstryker::level::record const &_level)
            {
              std::cout << _level << '\n';
              return EXIT_SUCCESS;
            }
          );
      });
}
catch(std::exception const &error)
{
  std::cerr << error.what() << '\n';
  return EXIT_FAILURE;
}
