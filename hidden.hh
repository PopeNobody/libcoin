
#if 0
namespace coin {
  data_chunk hd_priv_derive(const data_chunk &in, int idx, bool hard);
};

data_chunk coin::hd_priv_derive(const coin::data_chunk &in, int idx, bool hard)
{
  static constexpr auto first=(1<<31);
  if(hard)
    idx+=first;
  const static array<char,1> depth = { 0 };

#if 0
  const auto data = hard ?
    splice(depth,secret_,to_big_endian(idx))
    :
    splice(point_, to_big_endian(index));

  const auto parts = split(hmac_sha512_hash(data,chain_));
  cout << "chain: " << parts.first << endl;
  cout << "secret: " << parts.second << endl;
#endif
//     auto child=secret_;
//     if(!ec_add(child,parts.first))
//       return {};
//   
//     if(lineage_.depth == max_uint8)
//       return {};
//   
//     uint32_t;
};
coin::hd_root_t coin::hd_new(const coin::long_digest &seed)
{


  static const auto magic = to_chunk("Bitcoin seed");

  const auto intermediate = split(hmac_sha512_hash(seed,magic));
  int kid_num=0;
  int depth=0;
  int parent_fingerprint=0;
  auto hash_code = hmac_sha512_hash(seed,magic);
  auto inter = split(hash_code);
  hash_digest secret = inter.first;
  hash_digest chain = inter.second;
  hd_lineage_t &lineage(lineage);

  hd_private root_key(secret, chain, lineage);


  return data;
};
#endif
#if 0
namespace coin {
  template<size_t _size>
  struct bin_and_hex
  {
    union {
      array<byte_t,_size> bytes;
      uint32_t value;
    };
    array<char,_size*2> chars;
    bin_and_hex(const array<byte_t,_size> & _bytes)
    {
      fill(chars.begin(),chars.end(),0);
      for(int i=0;i<_size;i++)
      {
        auto val = _bytes[i];
        auto text = to_hex(val);
        chars[2*i+0]=text[0];
        chars[2*i+1]=text[1];
        cout.write(chars.begin(),chars.size());
        cout << endl;
      };
    };
    const char& operator[](unsigned idx) const
    {
      return chars[idx];
    };
    size_t constexpr size() const {
      return _size;
    };
  };
  template<size_t _size>
  ostream &operator<<(ostream &lhs, bin_and_hex<_size> &rhs)
  {
    for(auto b(rhs.chars.begin()), e(rhs.chars.end()); b!=e; b++)
    {
      lhs << '.' << *b;
    };
    return lhs;
  };
};
#endif
