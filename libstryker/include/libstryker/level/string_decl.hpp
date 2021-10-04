#ifndef LIBSTRYKER_LEVEL_STRING_DECL_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_STRING_DECL_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <cstddef>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace libstryker::level
{

template<
  std::size_t N
>
struct string
{
  using element_type = std::string;
};

}

#endif
