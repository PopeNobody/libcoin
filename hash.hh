#ifndef LIBBITCOIN_SYSTEM_HASH_HPP
#define LIBBITCOIN_SYSTEM_HASH_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <array>
#include <boost/multiprecision/cpp_int.hpp>

namespace coin {
  typedef uint8_t byte_t;
#define constexpr constexpr

// Common bitcoin hash container sizes.
static constexpr size_t hash_size = 32;
static constexpr size_t half_hash_size = hash_size / 2;
static constexpr size_t quarter_hash_size = half_hash_size / 2;
static constexpr size_t long_hash_size = 2 * hash_size;
static constexpr size_t short_hash_size = 20;
static constexpr size_t mini_hash_size = 6;

template<size_t size>
  class byte_array : public std::array<byte_t, size>
{
};
// Common bitcoin hash containers.
typedef byte_array<hash_size> hash_digest;
typedef byte_array<half_hash_size> half_hash;
typedef byte_array<quarter_hash_size> quarter_hash;
typedef byte_array<long_hash_size> long_hash;
typedef long_hash long_digest;
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

//   // Null-valued common bitcoin hashes.
//   
//   constexpr hash_digest null_hash
//   {
//       {
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//       }
//   };
//   
//   constexpr half_hash null_half_hash
//   {
//       {
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//       }
//   };
//   
//   constexpr quarter_hash null_quarter_hash
//   {
//       {
//           0, 0, 0, 0, 0, 0, 0, 0
//       }
//   };
//   
//   constexpr long_hash null_long_hash
//   {
//       {
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//       }
//   };
//   
//   constexpr short_hash null_short_hash
//   {
//       {
//           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//           0, 0, 0, 0
//       }
//   };
//   
//   constexpr mini_hash null_mini_hash
//   {
//       {
//           0, 0, 0, 0, 0, 0
//       }
//   };
//   
//   inline uint256_t to_uint256(const hash_digest& hash)
//   {
//       return from_little_endian<uint256_t>(hash.begin(), hash.end());
//   }
//   
//   /// Generate a scrypt hash to fill a byte array.
//   template <size_t Size>
//   byte_array<Size> scrypt(const data_slice& data, const data_slice& salt, uint64_t N,
//       uint32_t p, uint32_t r);
//   
//   /// Generate a scrypt hash of specified length.
//   BC_API data_chunk scrypt(const data_slice& data, const data_slice& salt, uint64_t N,
//       uint32_t p, uint32_t r, size_t length);
//   
//   /// Generate a bitcoin hash.
//   BC_API hash_digest bitcoin_hash(const data_slice& data);
//   
//   /// Generate a scrypt hash.
//   BC_API hash_digest scrypt_hash(const data_slice& data);
//   
//   /// Generate a bitcoin short hash.
//   BC_API short_hash bitcoin_short_hash(const data_slice& data);
//   
//   /// Generate a ripemd160 hash
//   BC_API short_hash ripemd160_hash(const data_slice& data);
//   BC_API data_chunk ripemd160_hash_chunk(const data_slice& data);
//   
//   /// Generate a sha1 hash.
//   BC_API short_hash sha1_hash(const data_slice& data);
//   BC_API data_chunk sha1_hash_chunk(const data_slice& data);
//   
//   /// Generate a sha256 hash.
//   BC_API hash_digest sha256_hash(const data_slice& data);
//   BC_API data_chunk sha256_hash_chunk(const data_slice& data);
//   
//   /// Generate a sha256 hash.
//   /// This hash function was used in electrum seed stretching (obsoleted).
//   BC_API hash_digest sha256_hash(const data_slice& first, const data_slice& second);
//   
//   // Generate a hmac sha256 hash.
//   BC_API hash_digest hmac_sha256_hash(const data_slice& data, const data_slice& key);
//   
//   /// Generate a sha512 hash.
//   BC_API long_hash sha512_hash(const data_slice& data);
//   
//   /// Generate a hmac sha512 hash.
//   BC_API long_hash hmac_sha512_hash(const data_slice& data, const data_slice& key);
//   
//   /// Generate a pkcs5 pbkdf2 hmac sha512 hash.
//   BC_API long_hash pkcs5_pbkdf2_hmac_sha512(const data_slice& passphrase,
//       const data_slice& salt, size_t iterations);
//   
//   } // namespace system
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