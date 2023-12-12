//-*-c++-*-
#ifndef _Filter_h_
#define _Filter_h_
#include <cstdint>
using namespace std;

class Filter {
  int8_t divisor;
  int8_t dim;
  int8_t *data;

public:
  Filter(int8_t _dim);
  int get(int r, int c);
  void set(int r, int c, int value);

  int8_t getDivisor();
  void setDivisor(int value);

  int8_t getSize();
  void info();
};

#endif