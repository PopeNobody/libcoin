#ifndef coin_hh
#define coin_hh coin_hh

#include "stl.hh"
#include "hash.hh"
#include "split_num.hh"
#include "array_slice.hh"
#include "hd_lineage.hh"
#include "hd_key.hh"
#include "hd_root.hh"
#include "hd_private.hh"

namespace coin {

  static constexpr size_t dictionary_size = 2048;
  static constexpr size_t byte_bits = 8;
  static constexpr size_t mnemonic_seed_multiple = 4;
  static constexpr size_t mnemonic_word_multiple = 3;
  static constexpr size_t entropy_bit_divisor = 32;
  static constexpr size_t bits_per_word = 11;
  static constexpr size_t hmac_iterations = 2048;
  
  typedef vector<byte_t> data_chunk;
  typedef std::initializer_list<data_slice> loaf;
  string encode_base58(const data_slice &slice);
  string encode_base16(const data_slice &slice);

  hash_digest sha256_hash(const data_slice &data);
  
  template <typename int_t>
    byte_array<sizeof(int_t)> to_big_endian(int_t val)
    {
      int_t oval=val;
      byte_array<sizeof(int_t)> res;
      for(int i=0;i<sizeof(int_t);i++)
      {
        res[res.size()-1-i]=uint8_t(val&0xff);
        val >>=8;
      };
      return res;
    };
  template <typename int_t>
    byte_array<sizeof(int_t)> to_little_endian(int_t val)
    {
      int_t oval=val;
      byte_array<sizeof(int_t)> res;
      for(int i=0;i<sizeof(int_t);i++)
      {
        res[i]=uint8_t(val&0xff);
        val >>=8;
      };
      return res;
    };
  template <size_t Left, size_t Right>
    byte_array<Left + Right> splice(const std::array<uint8_t, Left>& left,
        const std::array<uint8_t, Right>& right)
    {
      byte_array<Left+Right> res;
      auto dst(res.begin());
      dst=std::copy(left.begin(),left.end(),dst);
      dst=std::copy(right.begin(),right.end(),dst);
      return res;
    };

  template <size_t Left, size_t Middle, size_t Right>
    byte_array<Left + Middle + Right> splice(const std::array<uint8_t, Left>& left,
        const std::array<uint8_t, Middle>& middle,
        const std::array<uint8_t, Right>& right)
    {
      byte_array<Left+Middle+Right> res;
      std::fill(res.begin(),res.end(),0);
      auto dst(res.begin());
      dst=std::copy(left.begin(),left.end(),dst);
      dst=std::copy(middle.begin(),middle.end(),dst);
      dst=std::copy(right.begin(),right.end(),dst);
      return res;
    };

  template<typename int_t, typename itr_t>
    int_t from_little_endian_unsafe(itr_t start)
    {
      using std::cout;
      using std::endl;
      static_assert(std::is_unsigned<int_t>::value, "Unsigned only, please!");
      int_t out=0;
      int i=0;
      while(i<sizeof(int_t)) {
        auto xxx = static_cast<int_t>(0xff&*start++);
        xxx <<= (8*i++);
        out |= xxx;
      };
      return out;
    };
  uint64_t to_prefixes(uint32_t pri_pfix, uint32_t pub_pfix);
  template <typename int_t>
    byte_array<sizeof(int_t)> to_big_endian(int_t val);
  template <typename int_t>
    byte_array<sizeof(int_t)> to_little_endian(int_t val);
  template<typename itr_t>
    text_chunk to_chunk(itr_t b, itr_t e)
    {
      text_chunk res;
      while(b!=e)
        res.push_back(*b++);
      return res;
    };
  template<typename con_t>
    inline text_chunk to_chunk(const con_t &con)
    {
      return to_chunk(con.begin(),con.end());
    };
  inline text_chunk to_chunk(const char *str)
  {
    return text_chunk(str,str+strlen(str));
  };
  inline data_chunk build_chunk(loaf slices, size_t extra_reserve=0)
  {
    size_t size = 0;
    for (const auto slice: slices)
      size += slice.size();

    data_chunk out;
    out.reserve(size + extra_reserve);
    for (const auto slice: slices)
      out.insert(out.end(), slice.begin(), slice.end());

    return out;
  }


  class dictionary_t
  {
    array<string,dictionary_size> data;
    public:
    dictionary_t(std::initializer_list<const char*> &rhs);
    const string &operator[](size_t pos) const { return data[pos]; }
  };
  word_list create_mnemonic(const data_chunk& entropy, const dictionary_t &lexicon);

  inline uint8_t bip39_shift(size_t bit)
  {
    return (1 << (byte_bits - (bit % byte_bits) - 1));
  }
  long_digest decode_mnemonic(const word_list& mnemonic);
  string join(const word_list &words);
  int pkcs5_pbkdf2_hmac_sha512(
      const byte_t *phrase, size_t phrase_len,
      const byte_t *salt, size_t salt_len,
      byte_t *key, size_t key_len,
      size_t iter
      );
  long_digest pkcs5_pbkdf2_hmac_sha512(
      const data_slice& passphrase, const data_slice &salt, size_t iter
      );

  int pkcs5_pbkdf2(const uint8_t* passphrase, size_t passphrase_length,
      const uint8_t* salt, size_t salt_length, uint8_t* key, size_t key_length,
      size_t iterations);

  string normalize(const string &str);
  struct stext_t
  { 
    array<char,4> data;
    stext_t(array<char,4> &data)
      : data(data)
    {
    };
    stext_t(const char *chs)
    {
      data[0]=*chs++;
      data[1]=*chs++;
    };
    char *begin() {
      return data.data();
    };
    char *end() {
      return begin()+2;
    };
    char &operator[](int idx) 
    {
      return *(begin()+idx);
    };
    const char *begin() const {
      return data.data();
    };
    const char *end() const {
      return begin()+2;
    };
    char operator[](int idx) const
    {
      return *(begin()+idx);
    };
  };
  const stext_t &to_hex(unsigned ch);
  string encode_base16(const data_slice &slice);
  size_t count_leading_zeros(const data_slice &rhs);
  size_t count_leading_zeros(const string &rhs);
  void pack_value(data_chunk& indexes, size_t carry);
  ostream &operator<<(ostream &lhs, const data_slice &rhs);
  ostream &operator<<(ostream &lhs, const hd_private &rhs);
  ostream &operator<<(ostream &lhs, const hd_root_t &rhs);
  long_digest hmac_sha512_hash(const data_slice& data, const data_slice& key);
  word_list split_words(const string &phrase);
};

#endif
