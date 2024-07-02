#include "renderbuffer.h"

namespace fps::rendering {

   void renderbuffer::clear() {
      std::memset(buffer_.get(), 0, width_ * height_ * 4);
      std::memset(zbuffer_.get(), std::numeric_limits<float>::max(),
            width_ * height_ * sizeof(float));
   }
   void renderbuffer::draw_pixel(int x, int y, Color const &col) {
      auto const offset = (x + y * width()) * 4;
      std::memcpy(buffer() + offset, &col, sizeof(Color));
   }
   void renderbuffer::draw_pixel(int x, int y, float z, Color const &col) {
      if (z < zbuffer_[x + y * width()]) {
         zbuffer_[x + y * width()] = z;
         draw_pixel(x, y, col);
      }
   }
}
