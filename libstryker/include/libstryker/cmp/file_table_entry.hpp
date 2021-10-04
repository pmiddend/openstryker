#ifndef LIBSTRYKER_CMP_FILE_TABLE_ENTRY_HPP_INCLUDED
#define LIBSTRYKER_CMP_FILE_TABLE_ENTRY_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <fcppt/config/external_end.hpp>


namespace libstryker::cmp
{

struct file_table_entry {
  using size_type = std::uint32_t;

  file_table_entry(
    std::string _name,
    size_type const _offset,
    size_type const _size)
  :
    name_{std::move(_name)},
    offset_{_offset},
    size_{_size}
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
