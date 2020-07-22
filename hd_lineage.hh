#ifndef hd_lineage_hh
#define hd_lineage_hh hd_lineage_hh

#include "stl.hh"
#include "split_num.hh"

namespace coin {
  typedef split_num<uint32_t,uint64_t> prefix_t;
  struct hd_lineage_t
  {
    struct data_t {
      prefix_t prefix;
      uint8_t depth;
      uint32_t parent_fp;
      uint32_t child_no;
    } data;

    hd_lineage_t()
    {
    };
    hd_lineage_t(
        const prefix_t &prefix, uint8_t depth, 
        uint32_t parent_fp, uint32_t child_no
        )
      : data( { prefix, depth, parent_fp, child_no } )
    {
    };
    hd_lineage_t &operator=(const hd_lineage_t &rhs)
    {
      data=rhs.data;  
      return *this;
    };
    const prefix_t &prefix() const
    {
      return data.prefix;
    };
    const uint8_t depth() const
    {
      return data.depth;
    };
    const uint32_t parent_fp() const
    {
      return data.parent_fp;
    };
    const uint32_t child_no() const
    {
      return data.child_no;
    };
  };
  ostream &operator<<(ostream &lhs, const hd_lineage_t &rhs);
};
#endif
