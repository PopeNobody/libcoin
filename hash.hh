#ifndef LIBBITCOIN_SYSTEM_HASH_HPP
#define LIBBITCOIN_SYSTEM_HASH_HPP

#include "stl.hh"
#include "array_slice.hh"

namespace coin {
  typedef uint8_t byte_t;

// Common bitcoin hash container sizes.
static constexpr size_t hash_size = 32;
static constexpr size_t half_hash_size = hash_size / 2;
static constexpr size_t quarter_hash_size = half_hash_size / 2;
static constexpr size_t long_hash_size = 2 * hash_size;
static constexpr size_t short_hash_size = 20;
static constexpr size_t mini_hash_size = 6;


template<size_t size>
  struct byte_array : public array<byte_t,size>
  {
  };
// Common bitcoin hash containers.

typedef byte_array<half_hash_size> half_hash;
typedef byte_array<quarter_hash_size> quarter_hash;
typedef byte_array<long_hash_size> long_hash;
typedef byte_array<hash_size> hash_digest;
typedef array_slice<byte_t> data_slice;
typedef array_slice<char> text_slice;

struct long_digest
{
  typedef long_hash whole_t;
  typedef hash_digest part_t;
  typedef pair<part_t,part_t> split_t;
  union {
    whole_t whole;
    split_t split;
  };
  long_digest(const whole_t &whole = whole_t())
    : whole(whole)
  {
  };
  long_digest &operator=(const long_digest &rhs)
  {
    whole=rhs.whole;
    return *this;
  };
  operator const data_slice() const
  {
    return data_slice(whole);
  };
  uint8_t *begin() {
    return whole.begin();
  };
  uint8_t *end() {
    return whole.end();
  };
  uint8_t *data() {
    return whole.data();
  };
  const uint8_t *begin() const {
    return whole.begin();
  };
  const uint8_t *end() const {
    return whole.end();
  };
  const size_t size() const {
    return whole.size();
  };
  const part_t &part1() const
  {
    return split.first;
  };
  const part_t &part2() const
  {
    return split.second;
  };
  const split_t &parts() const
  {
    return split;
  };
};
ostream &operator<<(ostream &lhs, const data_slice &rhs);
inline ostream &operator<<(ostream &lhs, const long_digest &rhs)
{
  return lhs << data_slice(rhs);
};
inline ostream &operator<<(ostream &lhs, const hash_digest &rhs)
{
  return lhs << data_slice(rhs);
};

typedef byte_array<short_hash_size> short_hash;
typedef byte_array<mini_hash_size> mini_hash;

// Lists of common bitcoin hashes.
typedef std::vector<hash_digest> hash_list;
typedef std::vector<half_hash> half_hash_list;
typedef std::vector<quarter_hash> quarter_hash_list;
typedef std::vector<long_hash> long_hash_list;
typedef std::vector<short_hash> short_hash_list;
typedef std::vector<mini_hash> mini_hash_list;

// Alias for boost big integer types.
typedef boost::multiprecision::uint128_t uint128_t;
typedef boost::multiprecision::uint256_t uint256_t;

} // namespace libbitcoin

// Extend std and boost namespaces with our hash wrappers.
//-----------------------------------------------------------------------------

namespace std
{
template <size_t Size>
struct hash<coin::byte_array<Size>>
{
    size_t operator()(const coin::byte_array<Size>& hash) const
    {
        return boost::hash_range(hash.begin(), hash.end());
    }
};
} // namespace std

namespace boost
{
template <size_t Size>
struct hash<coin::byte_array<Size>>
{
    size_t operator()(const coin::byte_array<Size>& hash) const
    {
        return boost::hash_range(hash.begin(), hash.end());
    }
};
} // namespace boost

//#include <bitcoin/system/impl/math/hash.ipp>

#endif
