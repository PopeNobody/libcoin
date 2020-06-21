#ifndef hd_hey_hh
#define hd_hey_hh hd_hey_hh

#include "hash.hh"

namespace coin
{
  struct hd_key : public data_chunk
  {
    hd_key(const data_chunk &data)
      :data_chunk(data)
    {
    };
  };
}
#endif
