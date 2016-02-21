#include <libstryker/level/read.hpp>
#include <libstryker/level/record.hpp>
#include <alda/raw/record_output.hpp>
#include <majutsu/record_output.hpp>
#include <fcppt/args.hpp>
#include <fcppt/args_vector.hpp>
#include <fcppt/reference.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/container/at_optional.hpp>
#include <fcppt/optional/maybe.hpp>
#include <fcppt/config/external_begin.hpp>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
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
{
  fcppt::args_vector const args(fcppt::args(argc,argv));
  return
    fcppt::optional::maybe(
      fcppt::container::at_optional(args,1u),
      []{
        std::cerr << "usage: level_reader <level-file>\n";
        return EXIT_FAILURE;
      },
      [](fcppt::reference<std::string const> file_name)
      {
        std::ifstream file_stream{file_name.get()};
        return
          fcppt::optional::maybe(
            libstryker::level::read(file_stream),
            []{
              std::cerr << "Reading failed\n";
              return EXIT_FAILURE;
            },
            [](
              libstryker::level::record const &_level
            ){
              std::cout << _level << '\n';
              return EXIT_SUCCESS;
            }
          );
      });
}
