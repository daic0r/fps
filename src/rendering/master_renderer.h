#ifndef MASTER_RENDERER_H
#define MASTER_RENDERER_H

#include "../math/triangle.h"
#include "../math/matrix.h"
#include "model.h"
#include "renderbuffer.h"
#include <forward_list>
#include <raylib.h>

namespace fps::rendering {
   using namespace fps::math;

   class master_renderer {
      static inline std::size_t sModelId = 0;

      static inline const std::array<plane, 6> ndc_planes_ = {
         plane{ vec4f{ 0.0f, 0.0f, -1.0f, 0.0f }, vec4f{ 0.0f, 0.0f, 1.0f, 1.0f } },
         plane{ vec4f{ 0.0f, 0.0f, 1.0f, 0.0f }, vec4f{ 0.0f, 0.0f, -1.0f, 1.0f } },
         plane{ vec4f{ -1.0f, 0.0f, 0.0f, 0.0f }, vec4f{ 1.0f, 0.0f, 0.0f, 1.0f } },
         plane{ vec4f{ 1.0f, 0.0f, 0.0f, 0.0f }, vec4f{ -1.0f, 0.0f, 0.0f, 1.0f } },
         plane{ vec4f{ 0.0f, -1.0f, 0.0f, 0.0f }, vec4f{ 0.0f, 1.0f, 0.0f, 1.0f } },
         plane{ vec4f{ 0.0f, 1.0f, 0.0f, 0.0f }, vec4f{ 0.0f, -1.0f, 0.0f, 1.0f } }
      };

      std::vector<model> models_;
      matrix viewport_matrix_;
      matrix projection_matrix_;
      matrix view_matrix_;
      renderbuffer& renderbuffer_;
      std::forward_list<triangle<vec4, float>> clip_triangles_;

   public:
      master_renderer(renderbuffer& rb) noexcept : renderbuffer_{ rb } {}

      std::size_t add_model(model m) {
         models_.push_back(std::move(m));
         return sModelId++;
      }

      void set_projection_matrix(matrix const& proj) {
         projection_matrix_ = proj;
      }

      void set_view_matrix(matrix const& view) {
         view_matrix_ = view;
      }

      void set_viewport_matrix(matrix const& view) {
         viewport_matrix_ = view;
      }

      void render_model(std::size_t id, std::span<matrix> instances) noexcept;

   };

}

#endif
