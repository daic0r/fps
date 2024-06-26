#include "master_renderer.h"
#include "../math/clipping.h"
#include "utils.h"
#include <cassert>
#include <iostream>

namespace fps::rendering {
   using namespace fps::math;

   void master_renderer::render_model(std::size_t id, std::span<matrix> instances) noexcept {
      auto const& model = models_[id];
      auto const pv_matrix = projection_matrix_ * view_matrix_;
      for (auto const& model_matrix : instances) {
         for (std::size_t i = 0; i < model.indices_.size(); i += 3) {
            clip_triangles_.clear();

            auto const& v0 = model.vertices_[model.indices_[i + 0]];
            auto const& v1 = model.vertices_[model.indices_[i + 1]];
            auto const& v2 = model.vertices_[model.indices_[i + 2]];

            triangle<vec4, float> tri{ v0, v1, v2 };
            tri.transform(view_matrix_ * model_matrix);
            // TODO: Hidden surface detection goes here

            // Projection
            tri.transform(projection_matrix_);

            // Perspective divide
            if (not test_equal(tri[0][3], 0.0f))
               tri[0] = tri[0] / tri[0][3];
            tri[0][3] = 1.0f;
            if (not test_equal(tri[1][3], 0.0f))
               tri[1] = tri[1] / tri[1][3];
            tri[1][3] = 1.0f;
            if (not test_equal(tri[2][3], 0.0f))
               tri[2] = tri[2] / tri[2][3];
            tri[2][3] = 1.0f;
            clip_triangles_.push_front(tri);
            for (auto const& pln : ndc_planes_) {
               auto iter = clip_triangles_.begin();
               while (iter != clip_triangles_.end()) {
                  assert(test_equal((*iter)[0][3], 1.0f));
                  assert(test_equal((*iter)[1][3], 1.0f));
                  assert(test_equal((*iter)[2][3], 1.0f));
                  auto const [clipped, one, two] = clip(*iter, pln);
                  // completely behind plane? -> triangle invisible
                  if (not clipped and not one and not two) {
                     goto done;
                  } else
                  if (clipped and one) {
                     assert(test_equal((*one)[0][3], 1.0f));
                     assert(test_equal((*one)[1][3], 1.0f));
                     assert(test_equal((*one)[2][3], 1.0f));
                     *iter = *one;
                     if (two) {
                        assert(test_equal((*two)[0][3], 1.0f));
                        assert(test_equal((*two)[1][3], 1.0f));
                        assert(test_equal((*two)[2][3], 1.0f));
                        iter = clip_triangles_.insert_after(iter, *two);
                     }
                  }
                  ++iter;
               }
            }

            // into screenspace
            for (auto& triangle : clip_triangles_) {
               triangle.clamp_all_vertices(-1.0f, 1.0f);

               triangle.transform(viewport_matrix_);
               //
               // constexpr static auto EPSILON = 0.001f;
               // assert(test_equal(triangle[0][0], 0.0f, EPSILON) or test_equal(triangle[0][0], (float)renderbuffer_.width()-1, EPSILON) or (triangle[0][0] > 0.0f and triangle[0][0] < renderbuffer_.width()-1));
               // assert(test_equal(triangle[0][1], 0.0f, EPSILON) or test_equal(triangle[0][1], (float)renderbuffer_.height()-1, EPSILON) or (triangle[0][1] > 0.0f and triangle[0][1] < renderbuffer_.height()-1));
               // assert(test_equal(triangle[1][0], 0.0f, EPSILON) or test_equal(triangle[1][0], (float)renderbuffer_.width()-1, EPSILON) or (triangle[1][0] > 0.0f and triangle[1][0] < renderbuffer_.width()-1));
               // assert(test_equal(triangle[1][1], 0.0f, EPSILON) or test_equal(triangle[1][1], (float)renderbuffer_.height()-1, EPSILON) or (triangle[1][1] > 0.0f and triangle[1][1] < renderbuffer_.height()-1));
               // assert(test_equal(triangle[2][0], 0.0f, EPSILON) or test_equal(triangle[2][0], (float)renderbuffer_.width()-1, EPSILON) or (triangle[2][0] > 0.0f and triangle[2][0] < renderbuffer_.width()-1));
               // assert(test_equal(triangle[2][1], 0.0f, EPSILON) or test_equal(triangle[2][1], (float)renderbuffer_.height()-1, EPSILON) or (triangle[2][1] > 0.0f and triangle[2][1] < renderbuffer_.height()-1));

               assert((int)triangle[0][0] >= 0 and (int)triangle[0][0] < renderbuffer_.width() and (int)triangle[0][1] >= 0 and (int)triangle[0][1] < renderbuffer_.height());
               assert((int)triangle[1][0] >= 0 and (int)triangle[1][0] < renderbuffer_.width() and (int)triangle[1][1] >= 0 and (int)triangle[1][1] < renderbuffer_.height());
               assert((int)triangle[2][0] >= 0 and (int)triangle[2][0] < renderbuffer_.width() and (int)triangle[2][1] >= 0 and (int)triangle[2][1] < renderbuffer_.height());

               // assert(triangle[0][0] >= 0.0f and triangle[0][0] <= renderbuffer_.width() and triangle[0][1] >= 0.0f and triangle[0][1] <= renderbuffer_.height());
               // assert(triangle[1][0] >= 0.0f and triangle[1][0] <= renderbuffer_.width() and triangle[1][1] >= 0.0f and triangle[1][1] <= renderbuffer_.height());
               // assert(triangle[2][0] >= 0.0f and triangle[2][0] <= renderbuffer_.width() and triangle[2][1] >= 0.0f and triangle[2][1] <= renderbuffer_.height());
               draw_triangle(renderbuffer_, triangle[0][0], triangle[0][1], triangle[1][0], triangle[1][1], triangle[2][0], triangle[2][1], WHITE, WHITE, WHITE, false);
            }
done:
         }
      }
   }

}
