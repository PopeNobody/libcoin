#include "stl.hh"
#include <sstream>
#include "sha256.hh"
#include "coin.hh"


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
coin::word_list coin::split_words(const string &phrase)
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
uint32_t coin::bitcoin_checksum(const data_slice &data)
{
  const auto hash = bitcoin_hash(data);
  return from_little_endian_unsafe<uint32_t>(hash.begin());
};
vector<string> tests = {
"408b285c123836004f4b8842c89324c1f01382450c0d439af345ba7fc49acf705489c6fc77dbd4e3dc1dd8cc6bc9f043db8ada1e243c4a0eafb290d399480840",
"hd_private{hd_key{xprv9s21ZrQH143K4VHfAaPWRTm4aoHAZhJHunsZZTQptR82FSTZRjBGXBP8kQKHrUVUE8vMM2Z3h7UoG9x9XCt9FHQ1t1nHU7zQDqrEszAg28q}}",
"hd_private{hd_key{xprv9uX3hyBN8aRi2t9JHqRSd4BksMdj5ZnyaFbWfs3vd94yBAE1ZFkLiA7HR1tBDVQcS6SAQTzzXFjqj5w4beUDnELGENUHKEGX6o1qRCAkYQk}}"
};

#define throw_runtime_error(x) do{ \
  ostringstream msg; \
  msg << x; \
  throw runtime_error(msg.str()); \
}while(0);
template<typename res_t>
void check_result(int step, const res_t &rhs)
{
  ostringstream res;
  res << rhs;
  if(step >= tests.size() )
    throw_runtime_error("no test for step " << step);
  if( res.str() == tests[step] )
    return;
  throw_runtime_error( 
      "failed."<< 
      "  step:     " << step << endl <<
      "  expected: " << tests[step] << endl <<
      "  got:      " << res.str() << endl <<
      "sorry."
      );
}
int app_main(const vector<string> &args)
{
  int num=0;
  using namespace coin;
  auto m = hd_root_t::from_mnemonic(mnemonic);
  auto &root = m.root();
  check_result(0, m.seed());
  check_result(1, m.root());
  cout << m << endl;
  auto purpose=m.root().derive_private(44,true);
  cout << "m.root():         " << m.root() << endl;
  cout << "purpose:          " << purpose << endl;
  return 0;
}

int main(int argc, char**argv)
{
	vector<string> args;
	try {
		return app_main(args);
	} catch ( exception & ex ) {
		cout << ex << endl;
	};
};
