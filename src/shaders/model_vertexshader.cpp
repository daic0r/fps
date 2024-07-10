#include "model_vertexshader.h"
#include "interfaces/vertexshader_iface.h"
#include <math/vector.h>
#include <math/triangle.h>

namespace fps::rendering {
   std::optional<vertex_shader_output> model_vertexshader::run(fps::math::triangle<math::vec4, float> tri, const math::matrix& model, const math::matrix& view, const math::matrix& projection) {
      using namespace fps::math;
      tri.transform(view * model);
      auto the_normal = tri.normal();
      const auto fIncidence = the_normal.dot(vec3f{0.0f, 0.0f, 1.0f});
      if (fIncidence <= 0.0f) {
         return std::nullopt;
      }
      tri.transform(projection);
      return std::make_optional<vertex_shader_output>(vertex_shader_output{fIncidence, tri});
   }
}

