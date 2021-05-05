#include <cassert>
#include <iostream>

class Fibonacci {
 public:
  static int get(int n) {
    assert(n >= 0);
      int fzero=0,fone=1,ftwo;
    for (int i=1;i<n;i++)
    {
        ftwo=fzero+fone; // 1 1 2
        fzero=fone; // 1
        fone=ftwo; // 2
    }
      n=ftwo;
    return fone;
  }
};