#include "math/matrix.h"
#include <iostream>

int main() {
   fps::math::matrix m;
   m[0,0] = 1.0;
   m[0,1] = 2.0;
   std::cout << m[0,0] << " " << m[0,1] << std::endl;
   return 0;
}
