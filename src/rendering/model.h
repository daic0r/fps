#ifndef MODEL_H
#define MODEL_H

#include "../math/vector.h"
#include "interfaces/vertexshader_iface.h"

namespace fps::rendering {
   using namespace fps::math;

   struct model {
      std::vector<vertex> vertices_;
      std::vector<std::size_t> indices_;
      std::vector<vec3f> normals_;
      std::vector<vec2f> texCoords_;

      std::unique_ptr<vertexshader_iface> vertex_shader_;
   };

}

#endif
