#ifndef LIBSTRYKER_CMP_FILE_TABLE_ENTRY_HPP_INCLUDED
#define LIBSTRYKER_CMP_FILE_TABLE_ENTRY_HPP_INCLUDED

#include <fcppt/declare_strong_typedef.hpp>
#include <fcppt/strong_typedef_impl.hpp> // IWYU pragma: keep
#include <fcppt/config/external_begin.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <fcppt/config/external_end.hpp>


namespace libstryker::cmp
{

struct file_table_entry {
  using size_type = std::uint32_t;

  FCPPT_DECLARE_STRONG_TYPEDEF(size_type, offset_type);
  FCPPT_DECLARE_STRONG_TYPEDEF(size_type, count_type);

  file_table_entry(
    std::string _name,
    offset_type const _offset,
    count_type const _size)
  :
    name_{std::move(_name)},
    offset_{_offset.get()},
    size_{_size.get()}
  {}

  [[nodiscard]] std::string const &name() const  { return name_; }
  [[nodiscard]] size_type offset() const { return offset_; }
  [[nodiscard]] size_type size() const { return size_; }
private:
  std::string name_;
  size_type offset_;
  size_type size_;
};

}

#endif
