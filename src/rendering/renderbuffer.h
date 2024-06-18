#ifndef RENDERING_RENDERBUFFER_H
#define RENDERING_RENDERBUFFER_H

#include <memory>

namespace fps::rendering {
   class renderbuffer {
      int width_, height_;
      std::unique_ptr<unsigned char[]> buffer_;

   public:

      renderbuffer(int w, int h) :
         width_{w},
         height_{h},
         buffer_{ std::make_unique<unsigned char[]>(width_ * height_ * 4) }
      {}

      auto buffer() const noexcept { return buffer_.get(); }
      auto buffer() noexcept { return buffer_.get(); }

      auto width() const noexcept { return width_; } 
      auto height() const noexcept { return height_; }
   };
}

#endif
