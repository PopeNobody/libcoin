#include "coin.hh"

static const string passphrase_prefix="mnemonic";
string coin::normalize(const string &str)
{
  string res(str);
  boost::algorithm::trim(res);
  return res;
};
coin::long_digest coin::decode_mnemonic(const word_list& mnemonic)
{
  using coin::hmac_iterations;
  string phrase;
  {
    ostringstream text;
    auto beg(mnemonic.begin()), end(mnemonic.end());
    if(beg!=end) {
      text << normalize(*beg);
      while(++beg!=end)
        text << " " << normalize(*beg);
    }
    phrase=text.str();
  };
  const auto& salt = passphrase_prefix;
  auto res = pkcs5_pbkdf2_hmac_sha512(
      to_chunk(phrase),
      to_chunk(salt),
      hmac_iterations);
  return res;
}
