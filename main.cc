#include "stl.hh"
#include <sstream>
#include "sha256.hh"
#include "coin.hh"


namespace coin {
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
      auto dst(res.begin());
      std::copy(left.begin(),left.end(),dst);
      std::copy(middle.begin(),middle.end(),dst);
      std::copy(right.begin(),right.end(),dst);
      return res;
    };
}
#define xexpose(x) do{ \
  using namespace std; \
  ostringstream str; \
  str << __FILE__ << ":" << __LINE__ << ": "; \
  str << #x << "  => " << (x) << endl; \
  cerr << str.str(); \
} while(0);

void check_entropy()
{
  int fd;
  int c;

  if ((fd = open("/dev/random", O_RDONLY)) == -1) {
    cerr
      << "Error opening /dev/random:"
      << strerror(errno)
      << endl;
    exit(1);
  }
  if (ioctl(fd, RNDGETENTCNT, &c) == 0 && c < 160) {
    cerr << "too little entropy" << endl;
    exit(1);
  }
  if(close(fd)) {
    cerr << "close(/dev/random): " << strerror(errno);
    exit(1);
  };
};
using namespace std;
using coin::dictionary_t;


static vector<string> mk_mnemonic() {
  const static vector<string> mnemonic = {
    { "abandon" }, { "abandon" }, { "abandon" }, { "abandon" },
    { "abandon" }, { "abandon" }, { "abandon" }, { "abandon" },
    { "abandon" }, { "abandon" }, { "abandon" }, { "abandon" },
    { "abandon" }, { "abandon" }, { "abandon" }, { "abandon" },
    { "abandon" }, { "abandon" }, { "abandon" }, { "abandon" },
    { "abandon" }, { "abandon" }, { "abandon" }, { "art" },
  };
  return mnemonic;
};
const static vector<string> mnemonic = mk_mnemonic();
typedef uint8_t byte_t;

const coin::stext_t &coin::to_hex(unsigned ch)
{
  static array<stext_t,256> pairs = {
    "00", "01", "02", "03", "04", "05", "06", "07",
    "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", 
    "10", "11", "12", "13", "14", "15", "16", "17",
    "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", 
    "20", "21", "22", "23", "24", "25", "26", "27",
    "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", 
    "30", "31", "32", "33", "34", "35", "36", "37",
    "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", 
    "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", 
    "50", "51", "52", "53", "54", "55", "56", "57",
    "58", "59", "5a", "5b", "5c", "5d", "5e", "5f", 
    "60", "61", "62", "63", "64", "65", "66", "67",
    "68", "69", "6a", "6b", "6c", "6d", "6e", "6f", 
    "70", "71", "72", "73", "74", "75", "76", "77",
    "78", "79", "7a", "7b", "7c", "7d", "7e", "7f", 
    "80", "81", "82", "83", "84", "85", "86", "87",
    "88", "89", "8a", "8b", "8c", "8d", "8e", "8f", 
    "90", "91", "92", "93", "94", "95", "96", "97",
    "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", 
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", 
    "b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7",
    "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", 
    "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7",
    "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", 
    "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "da", "db", "dc", "dd", "de", "df", 
    "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7",
    "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef", 
    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
    "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff", 
  };
  return pairs[ch];
};
coin::word_list coin::split(const string &phrase)
{
  word_list res;
  auto b(phrase.begin()), e(phrase.end());
  while(b!=e) {
    while(b!=e && isspace(*b))
      ++b;
    auto s(b);
    while(s!=e && !isspace(*s))
      ++s;
    if(b!=s)
      res.push_back(string(b,s));
    b=s;
  };
  return res;
};
string coin::join(const word_list &words)
{
  ostringstream phrase;
  for(auto const &word : words)
  {
    phrase << word << "   ";
    string tmp = phrase.str();
  };
  string res=phrase.str();
  res.pop_back();
  return res;
};

//   uint32_t coin::to_prefix(uint64_t prefixes)
//   {
//     return uint32_t(prefixes>>32);
//   };
//   uint64_t coin::to_prefixes(uint32_t pri_pfix, uint32_t pub_pfix)
//   {
//     return (uint64_t(pri_pfix)<<32 | pub_pfix);
//   };


namespace coin {
  hash_digest bitcoin_hash(const data_slice &data);
  uint32_t bitcoin_checksum(const data_slice &data);
  hash_digest sha256_hash(const data_slice &data);
};
coin::hash_digest coin::sha256_hash(const data_slice &data)
{
  hash_digest res;
  SHA256_(data.data(),data.size(),res.data());
  return res;
};
coin::hash_digest coin::bitcoin_hash(const data_slice &data)
{
  return sha256_hash(sha256_hash(data));
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
uint32_t coin::bitcoin_checksum(const data_slice &data)
{
  const auto hash = bitcoin_hash(data);
  return from_little_endian_unsafe<uint32_t>(hash.begin());
};
coin::hd_private coin::hd_private::derive_private(int idx, bool hard)const
{
  static constexpr auto first=(1<<31);
  if(hard)
    idx+=first;
  const static array<byte_t,1> depth = { 0 };

  assert(hard);
  const auto data = coin::splice(depth,secret(),to_big_endian(idx));

//       :
//       coin::splice(point(), to_big_endian(idx));

  const auto parts = split(hmac_sha512_hash(data,chain()));
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
template<typename int_t>
coin::byte_array<sizeof(int_t)>  to_little_endian(int_t val)
{
  coin::byte_array<sizeof(int_t)> res;
  for(int i=0;i<res.size();i++)
  {
    res[res.size()-1-i]=val&0xff;
    val >>= 8;
  };
  return res;
};
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
coin::hd_root_t::hd_root_t()
{
}
coin::hd_root_t coin::hd_root_t::from_mnemonic(const string &phrase)
{
  return from_mnemonic(split(phrase));
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
  auto part_hash=split(full_hash);
  hd_root_t res;
  res.data.seed=seed;
  res.data.root=hd_private(part_hash.first,part_hash.second,lineage);
  return res;
}
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
int app_main(const vector<string> &args)
{
  int num=0;
  using namespace coin;
#if 0
  union {
    array<byte_t,4> bytes;
    unsigned        value;
  };
  value=0x1a1b1c1d;
  cout << hex << value << endl;
  cout << bin_and_hex <4>(bytes) << endl;
  bytes[0]=0x00;
  bytes[1]=0x01;
  bytes[2]=0x02;
  bytes[3]=0x03;
  cout << bin_and_hex<4>(bytes) << endl;
//     bah.set_bin(bytes);
#endif
  auto m = hd_root_t::from_mnemonic(mnemonic);
  auto &root = m.root();
  auto hd_key=m.root().to_hd_key();
  auto slice = array_slice(hd_key.data(),hd_key.data()+hd_key.size());
  auto purpose=m.root().derive_private(44,true);
  cout << "private_root_key: " << encode_base58(hd_key) << endl;
  cout << "m.root():         " << m.root() << endl;
  cout << "purpose:          " << purpose << endl;
  return 0;
}

#if 0
ostream &coin::operator<<(ostream &lhs, const coin::long_digest &rhs)
{
  char buf[sizeof(rhs)*2+2];
  char *pos=buf;
  for(auto ch : rhs)
  {
    const char *hex=to_hex(ch);
    *pos++=*hex++;
    *pos++=*hex++;
  };
  *pos++=0;
  return lhs<<buf;
};
#endif

int main(int argc, char**argv)
{
	vector<string> args;
	try {
		return app_main(args);
	} catch ( exception & ex ) {
		cout << ex << endl;
	};
};
