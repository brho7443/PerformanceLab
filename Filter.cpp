
#include "Filter.h"
#include <iostream>
#include <cstdint>

Filter::Filter(int8_t _dim)
{
  divisor = 1;
  dim = _dim;
  data = new int8_t[dim * dim];
}

int Filter::get(int r, int c)
{
  return data[ r * dim + c ];
}

void Filter::set(int r, int c, int value)
{
  data[ r * dim + c ] = value;
}

int8_t Filter::getDivisor()
{
  return divisor;
}

void Filter::setDivisor(int value)
{
  divisor = value;
}

int8_t Filter::getSize()
{
  return dim;
}

void Filter::info()
{
  cout << "Filter is.." << endl;
  for (int8_t col = 0; col < dim; col++) {
    for (int8_t row = 0; row < dim; row++) {
      int8_t v = get(row, col);
      cout << v << " ";
    }
    cout << endl;
  }
}