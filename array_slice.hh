#ifndef array_slice_hh
#define array_slice_hh array_slice_hh

#include "stl.hh"

namespace coin {
  //  typedef uint8_t byte_t;
  template <typename Iterable>
    class array_slice
    {
      public:
        typedef size_t size_type;

        array_slice(const char *begin, const char *end)
          : begin_((Iterable*)begin), end_((Iterable*)end)
        {
        };
        template <typename value_t>
          array_slice(const vector<value_t> &cont)
          : array_slice(cont.data(),cont.data()+cont.size())
          {
          };
        template <typename value_t, size_t size>
          array_slice(const array<value_t,size> &cont)
          : array_slice(cont.data(),cont.data()+size)
          {
          };

        array_slice(const Iterable* begin, const Iterable* end)
          : begin_(begin), end_(end)
        {
        };

        const Iterable* begin() const
        {
          return begin_;
        }
        const Iterable* end() const
        {
          return end_;
        }
        const Iterable* data() const
        {
          return begin_;
        }
        size_type size() const
        {
          return end_-begin_;
        };
        bool empty() const
        {
          return size()==0;
        }

      private:
        const Iterable* begin_;
        const Iterable* end_;
    };
};

#endif
