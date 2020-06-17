#include "stl.hh"
#include <sstream>
#include "dictionary.hh"


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
using coin::word_t;
using coin::dictionary_t;


static vector<word_t> mk_mnemonic() {
  word_t mnemonic[24] = {
    "abandon", "abandon", "abandon", "abandon", "abandon", "abandon",
    "abandon", "abandon", "abandon", "abandon", "abandon", "abandon",
    "abandon", "abandon", "abandon", "abandon", "abandon", "abandon",
    "abandon", "abandon", "abandon", "abandon", "abandon", "art",
  };
  vector<word_t> res(mnemonic,mnemonic+24);
  return res;
};
const static vector<word_t> mnemonic = mk_mnemonic();
ostream &operator<<(ostream &lhs, const type_info &rhs)
{
  return lhs << cplus_demangle(rhs.name(),0);
};
typedef uint8_t byte_t;
template<typename itr_t>
vector<byte_t> mnemonic_to_seed(itr_t beg, itr_t end)
{
  ostringstream phrase;
  if(beg!=end) {
    phrase << *beg++;
    while(beg!=end)
      phrase << "test\n";
  };
#if 0
    // Bound parameters.
    const dictionary_list& language = get_language_option();
    const auto& passphrase = get_passphrase_option();
    const auto& words = get_words_argument();

    const auto word_count = words.size();

    if ((word_count % wallet::mnemonic_word_multiple) != 0)
    {
        error << BX_MNEMONIC_TO_SEED_LENGTH_INVALID_SENTENCE << std::endl;
        return console_result::failure;
    }

    const auto valid = validate_mnemonic(words, language);

    if (!valid && language.size() == 1)
    {
        // This is fatal because a dictionary was specified explicitly.
        error << BX_MNEMONIC_TO_SEED_INVALID_IN_LANGUAGE << std::endl;
        return console_result::failure;
    }

    if (!valid && language.size() > 1)
        error << BX_MNEMONIC_TO_SEED_INVALID_IN_LANGUAGES << std::endl;

#ifdef WITH_ICU
    // Any word set divisible by 3 works regardless of language validation.
    const auto seed = decode_mnemonic(words, passphrase);
#else
    if (!passphrase.empty())
    {
        error << BX_MNEMONIC_TO_SEED_REQUIRES_ICU << std::endl;
        return console_result::failure;
    }

    // Decoding with passphrase requires ICU normalization.
    const auto seed = decode_mnemonic(words);
#endif

    output << base16(seed) << std::endl;
    return console_result::okay;
#endif
    vector<byte_t> res;
    return res;
}
const char *to_hex(char ch)
{
  typedef char pair_t[3];
  array<pair_t,256> pairs;
  if(!pairs[0][0])
  {
    for(int i=0;i<256;i++)
    {
      snprintf(pairs[i],sizeof(pairs[i]),"%02x",i);
    };
    for(int i=0;i<256;i++)
    {
      cerr << setw(5) << pairs[i] << endl;
    };
  };
  return pairs[ch];
};
ostream &operator<<(ostream &lhs, vector<byte_t> rhs)
{
  vector<char> base16;
  int count=0;
  for(auto ch : rhs)
  {
    lhs << to_hex(ch);
    ++count;
    if( (count%64)==0 ) {
      lhs << "\n";
    } else if ( (count%16) == 0 ) {
      lhs << " ";
    };
  };
  return lhs;
};
#define xexpose(x) do{ cerr << #x << ": " << (x) << endl; }while(0)
int app_main(const vector<string> &args)
{
  int num=0;
  cout << "seed: " << mnemonic_to_seed(mnemonic.begin(),mnemonic.end()) << endl;
#if 0
  cout << "typeid(mnemonic).name(): " << cplus_demangle(typeid(mnemonic).name(), gnu_v3_demangling) << endl;
  cerr << endl;
  for( auto &word : mnemonic ) {
    cerr << setw(4) << ++num << setw(10) << word;
    if( num % 6 )
      continue;
    cerr << endl;
  };
	cerr << "Hello, World!" << endl;
#endif
	return 1;
}
ostream &operator<<(ostream &lhs, const exception &rhs)
{
	lhs << rhs.what();
	return lhs;
};
int main(int argc, char**argv)
{
	vector<string> args;
	try {
		check_entropy();
		return app_main(args);
	} catch ( exception & ex ) {
		cout << ex << endl;
	};
};
