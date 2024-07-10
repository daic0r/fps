#ifndef MODEL_VERTEXSHADER_H
#define MODEL_VERTEXSHADER_H

#include <interfaces/vertexshader_iface.h>

namespace fps::rendering {
   class model_vertexshader : public vertexshader_iface {
   public:
      virtual ~model_vertexshader() = default;
      std::optional<vertex_shader_output> run(fps::math::triangle<math::vec4, float>, const math::matrix& model, const math::matrix& view, const math::matrix& projection) override;
   };
}

#endif
