#ifndef hd_private_hh
#define hd_private_hh hd_private_hh

#include "hd_lineage.hh"
#include "hd_key.hh"

namespace coin {
  struct hd_private {
    struct data_t {
      hash_digest secret;
      hash_digest chain;
      hd_lineage_t lineage;
      data_t(const hash_digest &secret, const hash_digest &chain, const hd_lineage_t &lineage)
        : secret(secret), chain(chain), lineage(lineage)
      {
      };
      data_t()
      {
      };
      data_t &operator=(const data_t &rhs)
      {
        secret=rhs.secret;
        chain=rhs.chain;
        lineage=rhs.lineage;
        return *this;
      };
    } data;
    hd_private()
    {
    };
    hd_private(const hash_digest &secret, const hash_digest &chain, const hd_lineage_t &lineage)
      : data(secret,chain,lineage)
    {
    };
    hd_private &operator=(const hd_private &rhs)
    {
      data=rhs.data;
      return *this;
    };
    hd_private derive_private(int idx, bool hard)const;
    const hd_lineage_t &lineage() const {
      return data.lineage;
    };
    const hash_digest &chain() const {
      return data.chain;
    };
    const hash_digest &secret() const {
      return data.secret;
    };
    hd_key to_hd_key() const;
  };
}
#endif
