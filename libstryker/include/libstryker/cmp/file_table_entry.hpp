#ifndef LIBSTRYKER_CMP_FILE_TABLE_ENTRY_HPP_INCLUDED
#define LIBSTRYKER_CMP_FILE_TABLE_ENTRY_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <fcppt/config/external_end.hpp>


namespace libstryker
{
namespace cmp
{

struct file_table_entry {
  typedef std::uint32_t size_type;

  file_table_entry(
    std::string _name,
    size_type const _offset,
    size_type const _size)
  :
    name_{std::move(_name)},
    offset_{_offset},
    size_{_size}
  {}

  std::string const &name() const  { return name_; }
  size_type offset() const { return offset_; }
  size_type size() const { return size_; }
private:
  std::string name_;
  size_type offset_;
  size_type size_;
};

}
}

#endif
