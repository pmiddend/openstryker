#ifndef LIBSTRYKER_LEVEL_STRING_DECL_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_STRING_DECL_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <cstddef>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace level
{

template<
  std::size_t N
>
struct string
{
  typedef std::string element_type;
};

}
}

#endif
