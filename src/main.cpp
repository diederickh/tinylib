/*

  Testing tinylib.

 */

#define ROXLU_USE_MATH
#include "tinylib.h"

#include <stdio.h>
#include <iostream>

int main() {

  printf("\n--------------\n");
  printf("TINYLIB\n");
  printf("--------------\n\n");

  vec2 v(10, 10);
  v.print();

  mat4 m;
  m.print();

  return EXIT_SUCCESS;
}
