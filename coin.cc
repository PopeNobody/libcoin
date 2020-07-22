#include "coin.hh"
ostream &coin::operator<<(ostream &lhs, const data_slice &rhs)
{
  string text=encode_base16(rhs);
  return lhs << text;
};
string coin::encode_base16(const data_slice &slice)
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
