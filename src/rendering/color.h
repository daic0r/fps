#ifndef COLOR_H
#define COLOR_H

namespace fps::rendering {
   struct Color {
      unsigned char r, g, b, a;

      auto operator<=> (Color const&) const = default;

      Color operator*(float rhs) const noexcept {
         return Color{ static_cast<unsigned char>(r * rhs),
                       static_cast<unsigned char>(g * rhs),
                       static_cast<unsigned char>(b * rhs),
                       static_cast<unsigned char>(a * rhs) };
      }
   };
}

#endif // COLOR_H
