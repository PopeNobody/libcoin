#include "hd_root.hh"
#include "coin.hh"

coin::hd_root_t::hd_root_t()
{
}
coin::hd_root_t coin::hd_root_t::from_mnemonic(const string &phrase)
{
  return from_mnemonic(split_words(phrase));
}
coin::hd_root_t coin::hd_root_t::from_mnemonic(const word_list &words)
{
  return from_seed(decode_mnemonic(words));
}
coin::hd_root_t coin::hd_root_t::from_seed(const long_digest &seed)
{
  const static auto magic = to_chunk("Bitcoin seed");
  const uint32_t pfix=76066276;
  hd_lineage_t lineage(prefix_t(0,pfix), 0, 0, 0);
  auto full_hash=hmac_sha512_hash(seed,magic);
  auto part_hash=full_hash.parts();
  hd_root_t res;
  res.data.seed=seed;
  res.data.root=hd_private(full_hash.part1(),full_hash.part2(),lineage);
  return res;
}
ostream &coin::operator<<(ostream &lhs, const hd_root_t &rhs)
{
  lhs << "hd_root_t(\n  seed=" << 
    rhs.seed() << ",\n  root=" << rhs.root() << "\n)";
  return lhs;
};
