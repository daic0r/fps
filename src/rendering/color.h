#ifndef COLOR_H
#define COLOR_H

namespace fps::rendering {
   struct Color {
      unsigned char r, g, b, a;

      auto operator<=> (Color const&) const = default;
   };
}

#endif // COLOR_H
