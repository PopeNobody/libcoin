#include "hd_key.hh"
#include "coin.hh"

ostream &coin::operator<<(ostream &lhs, const coin::hd_key &rhs)
{
  return lhs << "hd_key{" << encode_base58(rhs) << "}";
};
