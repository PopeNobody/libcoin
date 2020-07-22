#include "hd_private.hh"
#include "coin.hh"
#include "hash.hh"
#include "secp256k1.h"

secp256k1_context *context()
{
  static secp256k1_context *res;
  if(!res)
    res=secp256k1_context_create( SECP256K1_CONTEXT_VERIFY);

  return res;
};
using coin::byte_array;
typedef coin::hash_digest ec_secret;
typedef coin::byte_array<33> ec_compressed;
typedef coin::byte_array<65> ec_uncompressed;

constexpr int to_flags(bool compressed)
{
    return compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
}
template <size_t Size>
bool parse(secp256k1_pubkey& out,
    const byte_array<Size>& point)
{
    return secp256k1_ec_pubkey_parse(context(), &out, point.data(), Size) == 1;
}
template <size_t Size>
bool serialize(byte_array<Size>& out,
    const secp256k1_pubkey point)
{
    auto size = Size;
    const auto flags = to_flags(Size == sizeof(ec_compressed));
    secp256k1_ec_pubkey_serialize(context(), out.data(), &size, &point, flags);
    return size == Size;
}
template <size_t Size>
bool do_ec_add(byte_array<Size>& in_out, const ec_secret& secret)
{
    secp256k1_pubkey pubkey;
    if(!parse(pubkey, in_out))
      throw runtime_error("failed to parse result");
    if(secp256k1_ec_pubkey_tweak_add(context(), &pubkey, secret.data()) != 1)
      throw runtime_error("tweak_add failed");
    if(!serialize(in_out, pubkey))
      throw runtime_error("serialize failed");
    return true;
}
bool ec_add(ec_compressed& point, const ec_secret& scalar)
{
    return do_ec_add(point, scalar);
}

bool ec_add(ec_uncompressed& point, const ec_secret& scalar)
{
    return do_ec_add(point, scalar);
}
ostream &coin::operator<<(ostream &lhs, const hd_private &rhs)
{
  return lhs << "hd_private{" << rhs.to_hd_key() << "}";
};
#define xexpose(x) do{ cerr << #x << ": " << x << endl; } while(0)
coin::hd_private coin::hd_private::derive_private(int idx, bool hard)const
{
  const static array<byte_t,1> depth = { 0 };


  if(hard)
    idx+=first_hardened_key;
  assert(hard);
  auto data = coin::splice(depth,secret(),to_big_endian(idx));
  xexpose(data);
// : coin::splice(point(), to_big_endian(idx));

  xexpose(chain());
  const auto parts = hmac_sha512_hash(data,chain()).parts();
  xexpose(parts.first);
  xexpose(parts.second);

  auto child = secret();
  xexpose(child);
  if(!do_ec_add(child,parts.first))
    throw runtime_error("failed to add child and secret");

#if 0
  if(lineage().depth() == 255)
    throw runtime_error("depth > 255 not allowed!");
#endif
  return *this;
};
coin::hd_key coin::hd_private::to_hd_key() const
{
  data_chunk hd_key;
  size_t ls=hd_key.size();
  for( auto ch : to_big_endian(lineage().prefix().part1()) )
    hd_key.push_back(ch);
  hd_key.push_back(lineage().depth());
  for( auto ch : to_big_endian(lineage().parent_fp()) )
    hd_key.push_back(ch);
  for( auto ch : to_big_endian(lineage().child_no()) )
    hd_key.push_back(ch);
  for( auto ch : chain() )
    hd_key.push_back(ch);
  hd_key.push_back(0);
  for( auto ch : secret() )
    hd_key.push_back(ch);
  auto sha256_1 = sha256_hash(hd_key);
  auto sha256_2 = sha256_hash(sha256_1);
  auto checksum = from_little_endian_unsafe<uint32_t>(sha256_2.begin());
  auto form = to_little_endian(checksum);
  for( auto ch : form )
    hd_key.push_back(ch);
  return hd_key;
};
