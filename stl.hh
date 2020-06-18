#ifndef stl_hh
#define stl_hh stl_hh

#include <array>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <linux/random.h>
#include <sys/ioctl.h>

#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/core/demangle.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using std::exception;
using std::runtime_error;
using std::string;
using std::vector;
using std::array;
using std::ostream;
using std::endl;
using std::cout;
using std::cerr;
using std::setw;
using std::pair;
using std::setfill;
using std::setprecision;
using std::ostringstream;
using boost::core::demangle;

namespace coin {
  typedef vector<string> word_list;
  typedef uint8_t byte_t;
  typedef vector<byte_t> data_chunk;
  typedef vector<char> text_chunk;
};

namespace std {
  inline ostream &operator<<(ostream &lhs, const type_info &rhs)
  {
    return lhs << demangle(rhs.name());
  };
  inline ostream &operator<<(ostream &lhs, const exception &rhs)
  {
    return lhs << rhs.what();
  };
};

#endif
