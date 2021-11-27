#include "ppm.hpp"
#include <cstdio>

int main() {
  FILE* file = fopen("output.pbm", "wb");
  ppm::BwPpmImage image(100, 100);  

  for(int x = 40; x < 60; x++)
    for(int y = 40; y < 60; y++)
      image.set_pixel(x, y, true);

  image.write_binary(file);

  return 0;
}
