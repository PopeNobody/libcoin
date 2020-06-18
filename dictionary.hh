#ifndef dictionary_hh
#define dictionary_hh dictionary_hh

#include <array>
#include <assert.h>

namespace coin {
  struct word_t
  {
    struct data_t {
      std::array<char,12> text;
      size_t length;
    } data;
    static const data_t &empty_data() {
      static const data_t res = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0 };
      return res;
    };
    word_t(const char *text)
      : data(empty_data)
    {
      auto orig=text;
      for(int i=0;i<sizeof(data.text);i++)
      {
        data.text[i]=*text;
        if(*text)
          ++text;
      };
      if(*text)
        throw runtime_error(string("word '")+orig+"' is too long");
    };
    const char *text()const {
      return data.text.begin();
    };
  };
  ostream &operator<<(ostream &lhs, const word_t &rhs)
  {
    lhs << rhs.text();
    return lhs;
  };
  enum { dictionary_size=2048 };
  class dictionary_t : public std::array<const char*, dictionary_size>
  {
  };
};

#endif
