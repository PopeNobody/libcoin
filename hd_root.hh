#ifndef hd_root_hh
#define hd_root_hh hd_root_hh

#include "hash.hh"
#include "stl.hh"
#include "hd_private.hh"

namespace coin {
  struct hd_root_t
  {
    struct dat_t {
      long_digest seed;
      hd_private root;
    } data;
    hd_root_t();
    public:
    const hd_private &root() const
    {
      return data.root;
    };
    const long_digest &seed() const
    {
      return data.seed;
    };
    static hd_root_t from_mnemonic(const word_list &words);
    static hd_root_t from_mnemonic(const string &phrase);
    static hd_root_t from_entropy();
    static hd_root_t from_seed(const long_digest &seed);
  };
};

#endif
