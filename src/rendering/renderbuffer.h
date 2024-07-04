#ifndef RENDERING_RENDERBUFFER_H
#define RENDERING_RENDERBUFFER_H

#include <cassert>
#include <cstring>
#include "color.h"

namespace fps::rendering {
   class renderbuffer {
      int width_, height_;
      std::unique_ptr<unsigned char[]> buffer_;
      std::unique_ptr<float[]> zbuffer_;
      unsigned char* sdl_texture_{ nullptr };

   public:

      renderbuffer(int w, int h) :
         width_{w},
         height_{h},
         buffer_{ std::make_unique<unsigned char[]>(width_ * height_ * 4) },
         zbuffer_{ std::make_unique<float[]>(width_ * height_) }
      {
      }

      auto buffer() const noexcept { return buffer_.get(); }
      auto buffer() noexcept { return buffer_.get(); }

      auto width() const noexcept { return width_; } 
      auto height() const noexcept { return height_; }

      void clear(); 
      void set_texture_ptr(unsigned char* ptr) { 
         assert(ptr);
         sdl_texture_ = ptr;
         assert(sdl_texture_);
      }
      void draw_pixel(int x, int y, Color const &col);
      void draw_pixel(int x, int y, float z, Color const &col);
      void draw_triangle(int x1, int y1, float z1, int x2, int y2, float z2, int x3,
         int y3, float z3, Color col1, Color col2, Color col3,
         bool bFill = true);
   };
}

#endif
