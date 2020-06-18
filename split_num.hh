#ifndef split_num_hh
#define split_num_hh split_num_hh

#include <utility>
namespace coin {
  template<typename part_t, typename whole_t>
    struct split_num
    {
      union data_t {
        std::pair<part_t, part_t> parts;
        whole_t whole;
        data_t(const part_t &part1, const part_t &part2)
          : parts(part1, part2)
        {
        };
        data_t(const whole_t &whole=0)
          : whole(whole)
        {
        };
        data_t &operator=(const data_t &rhs)
        {
          whole=rhs.whole;
          return *this;
        };
      } data;
      split_num(whole_t whole=0)
        : data(whole)
      {
      };
      split_num(part_t part1, part_t part2)
        : data(part1,part2)
      {
      }
      part_t part1() const {
        return data.parts.second;
      };
      part_t part2() const {
        return data.parts.first;
      };
      whole_t whole() const {
        return data.whole;
      };

    };
}
#endif
