#include "hd_lineage.hh"

ostream &coin::operator<<(ostream &lhs, const coin::hd_lineage_t &rhs)
{
  lhs
    << typeid(rhs)
    << "{ prefixes: {" 
    << rhs.prefix().part1() << "," << rhs.prefix().part2()
    << "}, depth: " << int(rhs.depth())
    << ", pfp: " << rhs.parent_fp()
    << ", cno: " << rhs.child_no()
    << "}";
  return lhs;
};
