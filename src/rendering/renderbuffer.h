#ifndef RENDERING_RENDERBUFFER_H
#define RENDERING_RENDERBUFFER_H

#include <cstring>
#include <raylib.h>

namespace fps::rendering {
   class renderbuffer {
      int width_, height_;
      std::unique_ptr<unsigned char[]> buffer_;
      std::unique_ptr<float[]> zbuffer_;

   public:

      renderbuffer(int w, int h) :
         width_{w},
         height_{h},
         buffer_{ std::make_unique<unsigned char[]>(width_ * height_ * 4) },
         zbuffer_{ std::make_unique<float[]>(width_ * height_) }
      {
         clear();
      }

      auto buffer() const noexcept { return buffer_.get(); }
      auto buffer() noexcept { return buffer_.get(); }

      auto width() const noexcept { return width_; } 
      auto height() const noexcept { return height_; }

      void clear(); 
      void draw_pixel(int x, int y, Color const &col);
      void draw_pixel(int x, int y, float z, Color const &col);
   };
}

#endif
