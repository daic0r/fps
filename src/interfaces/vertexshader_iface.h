#ifndef VERTEXSHADER_IFACE_H
#define VERTEXSHADER_IFACE_H

#include "math/triangle.h"
#include <math/vector.h>
#include <math/matrix.h>

namespace fps::rendering {
   struct vertex_shader_output {
      double fIncidence;
      fps::math::triangle<math::vec4, float> triangle;
   };

   class vertexshader_iface {
   public:
      virtual ~vertexshader_iface() = default;
      virtual std::optional<vertex_shader_output> run(fps::math::triangle<math::vec4, float>, const math::matrix& model, const math::matrix& view, const math::matrix& projection) = 0;
   };
}

#endif
