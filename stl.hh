#ifndef stl_hh
#define stl_hh stl_hh

#include <assert.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <stdexcept>

#include <linux/random.h>
#include <sys/ioctl.h>

#include <boost/range.hpp>
#include <boost/core/demangle.hpp>

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
using std::setfill;
using std::setprecision;
using boost::range;
using boost::core::demangle;

typedef vector<string> word_list;

#endif
