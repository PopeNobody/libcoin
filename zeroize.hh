#ifndef zeroize_hh
#define zeroize_hh zeroize_hh

namespace coin {
  inline void zeroize(char *buf, size_t len)
  {
    auto beg(buf), end(buf+len);
    while(beg!=end)
      *beg++=0;
  }
  inline void zeroize(void *buf, size_t len)
  {
    zeroize((char*)buf,len);
  }
}

#endif
