#ifndef coin_hh
#define coin_hh coin_hh

#include "stl.hh"
#include "hash.hh"
#include "split_num.hh"

namespace coin {

  static constexpr size_t dictionary_size = 2048;
  static constexpr size_t byte_bits = 8;
  static constexpr size_t mnemonic_seed_multiple = 4;
  static constexpr size_t mnemonic_word_multiple = 3;
  static constexpr size_t entropy_bit_divisor = 32;
  static constexpr size_t bits_per_word = 11;
  static constexpr size_t hmac_iterations = 2048;
  
  //  typedef uint8_t byte_t;
  template <typename Iterable>
    class array_slice
    {
      public:
        typedef std::size_t size_type;

        template <typename Container>
          array_slice(const Container& container)
          : begin_((Iterable*)&container[0]), end_((Iterable*)&container[0]+container.size())
          {
          };

        array_slice(const Iterable* begin, const Iterable* end)
          : begin_(begin), end_(end)
        {
        };

        const Iterable* begin() const
        {
          return begin_;
        }
        const Iterable* end() const
        {
          return end_;
        }
        const Iterable* data() const
        {
          return begin_;
        }
        size_type size() const
        {
          return end_-begin_;
        };
        bool empty() const
        {
          return size()==0;
        }

      private:
        const Iterable* begin_;
        const Iterable* end_;
    };
  typedef vector<byte_t> data_chunk;
  typedef array_slice<byte_t> data_slice;
  typedef array_slice<char> text_slice;
  typedef std::initializer_list<data_slice> loaf;
  string encode_base58(const data_slice &slice);
  string encode_base16(const data_slice &slice);

  hash_digest sha256_hash(const data_slice &data);
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
    hd_lineage_t(const prefix_t &prefix, uint8_t depth, uint32_t parent_fp, uint32_t child_no)
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
  inline ostream &operator<<(ostream &lhs, const hd_lineage_t &rhs)
  {
    lhs
      << typeid(rhs)
      << "{ prefixes: {" 
      << rhs.prefix().part1() << "," << rhs.prefix().part2()
      << "}, depth: " << int(rhs.depth())
      << ", pfp: " << rhs.parent_fp()
      << ", cno: " << rhs.child_no()
      << "}";
    return lhs;
  };
  
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

  struct hd_key : public data_chunk
  {
    hd_key(const data_chunk &data)
      :data_chunk(data)
    {
    };
  };
  inline ostream &operator<<(ostream &lhs, const hd_key &rhs)
  {
    return lhs << encode_base58(rhs);
  };
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
  struct hd_root_t
  {
    struct dat_t {
      long_hash seed;
      hd_private root;
    } data;
    hd_root_t();
    public:
    const hd_private &root() const
    {
      return data.root;
    };
    const long_hash &seed() const
    {
      return data.seed;
    };
    static hd_root_t from_mnemonic(const word_list &words);
    static hd_root_t from_mnemonic(const string &phrase);
    static hd_root_t from_entropy();
    static hd_root_t from_seed(const long_hash &seed);
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
  struct stext_t : public array<char,4>
  {
  };
  const stext_t &to_hex(unsigned ch);
  inline string encode_base16(const data_slice &slice)
  {
    char buf[slice.size()*2+2];
    memset(buf,0,sizeof(buf));
    auto beg(slice.begin()), end(slice.end());
    auto dst(buf);
    while(beg!=end) {
      auto text=to_hex(*beg++);
      auto beg(text.begin());
      while(beg!=text.end() && *beg)
        *dst++=*beg++;
      *dst=0;
    };
    return string(buf);
  };
  size_t count_leading_zeros(const data_slice &rhs);
  size_t count_leading_zeros(const string &rhs);
  void pack_value(data_chunk& indexes, size_t carry);
  inline ostream &operator<<(ostream &lhs, const data_slice &rhs)
  {
    string text=encode_base16(rhs);
    return lhs << text;
  };
  inline ostream &operator<<(ostream &lhs, const hd_private &rhs)
  {
    lhs
      << "hd_private{\n"
      << "  secret:   " << rhs.secret() << ",\n"
      << "  chain:    " << rhs.chain() << ",\n"
      << "  " << rhs.lineage() << "\n"
      << "}";
    return lhs;
  };
  inline ostream &operator<<(ostream &lhs, const hd_root_t &rhs)
  {
    lhs << "hd_root_t(seed=" << rhs.seed() << ", root=" << rhs.root() << ")";
    return lhs;
  };
  long_digest hmac_sha512_hash(const data_slice& data, const data_slice& key);
  word_list split(const string &phrase);
};

#endif
