#ifndef LIBSTRYKER_LEVEL_STRING_HPP_INCLUDED
#define LIBSTRYKER_LEVEL_STRING_HPP_INCLUDED

#include <libstryker/level/string_decl.hpp>
#include <alda/bindings/array.hpp>
#include <alda/bindings/fundamental.hpp>
#include <alda/raw/element_type.hpp>
#include <alda/raw/dispatch_type.hpp>
#include <alda/raw/make_generic.hpp>
#include <alda/raw/stream/bind.hpp>
#include <alda/raw/stream/reference.hpp>
#include <alda/raw/stream/return.hpp>
#include <alda/raw/stream/result.hpp>
#include <fcppt/config/external_begin.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace level
{

template<typename Stream, std::size_t N>
alda::raw::stream::result<Stream, libstryker::level::string<N>>
make_generic(
	alda::raw::dispatch_type<libstryker::level::string<N>>,
	alda::raw::dispatch_type<Stream>,
	alda::raw::stream::reference<Stream> _stream
)
{
  typedef alda::bindings::array<std::array<char, N>,alda::bindings::fundamental<char>> wrapped;
  return
    alda::raw::stream::bind<Stream>(
      alda::raw::make_generic<Stream, wrapped>(_stream),
      [](alda::raw::element_type<wrapped> const &_array)
      {
        return
          alda::raw::stream::return_<Stream>(
            std::string(_array.begin(),std::find(_array.begin(),_array.end(),0)));
      });
}

}
}

#endif
