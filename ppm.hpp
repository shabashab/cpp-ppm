#include <cstdint>
#include <cstdio>
#include <cassert>

#ifndef CPP_PPM
#define CPP_PPM

namespace ppm {
  class PpmImage {
    protected:
      uint16_t width;
      uint16_t height;
    public:
      uint16_t get_width() { return this->width; }
      uint16_t get_height() { return this->height; }

    //"Abstract" methods
    public:
      virtual void write_plain(FILE* file) = 0;
      virtual void write_binary(FILE* file) = 0;
  };

  class BwPpmImage : public PpmImage {
    private:
      uint8_t* pixels;
      uint32_t row_size;
      uint32_t pixels_size;
    public:
      BwPpmImage(uint16_t width, uint16_t height);
    public:
      void write_plain(FILE* file);
      void write_binary(FILE* file);

      void set_pixel(uint16_t x, uint16_t y, bool value);
      bool get_pixel(uint16_t x, uint16_t y);
  };
}

#endif

#ifndef CPP_PPM_IMPL
#define CPP_PPM_IMPL

namespace ppm {
  
//  void print_binary(uint8_t number) {
//    for(int i = 0; i < 8; i++)
//      printf("%d", (number & 1 << (7 - i)) != 0);
//    printf("\n");
//  }

  //Implementation for BwPpmImage
  BwPpmImage::BwPpmImage(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;

    this->row_size = ((this->width / 8)) + (this->width % 8 != 0);
    this->pixels = new uint8_t[this->row_size * this->height];
  }

  void BwPpmImage::set_pixel(uint16_t x, uint16_t y, bool value) {
    uint16_t offset = (this->row_size * y) + (x / 8);
    uint8_t* pixel = &(this->pixels[offset]);

    uint8_t change_mask = (1 << (7 - (x % 8)));
    uint8_t mask_value = value != 0;

    (*pixel) = (mask_value * ((*pixel) | change_mask)) | ((mask_value == 0) * ((*pixel) & (~change_mask)));
  } 

  bool BwPpmImage::get_pixel(uint16_t x, uint16_t y) {
    uint16_t offset = (this->row_size * y) + (x / 8);
    uint8_t mask = 1 << (7 - (x % 8));

    uint8_t pixel = this->pixels[offset];
    return (pixel & mask) != 0;
  }

  void BwPpmImage::write_plain(FILE* file) {
    assert(file);

    //Writing the header of a file
    fprintf(file, "P1\n%d %d\n", this->width, this->height);

    for(uint16_t y = 0; y < this->height; y++) {
      for(uint16_t x = 0; x < this->width; x++) {
        fprintf(file, "%d ", this->get_pixel(x, y));               
      }
    }
  }

  void BwPpmImage::write_binary(FILE* file) {
    assert(file);

    fprintf(file, "P4\n%d %d\n", this->width, this->height);
    fwrite(this->pixels, sizeof(uint8_t), this->pixels_size, file);
  }
}

#endif
