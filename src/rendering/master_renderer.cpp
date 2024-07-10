#include "master_renderer.h"
#include "../math/clipping.h"
#include "utils.h"
#include <cassert>
#include <iostream>

namespace fps::rendering {
   using namespace fps::math;

   void master_renderer::render_model(std::size_t id,
         std::span<matrix> instances) noexcept {
      auto const &model = models_[id];
      auto const pv_matrix = projection_matrix_ * view_matrix_;
      for (auto const &model_matrix : instances) {
         for (std::size_t i = 0; i < model.indices_.size(); i += 3) {
            clip_triangles_.clear();

            auto const &v0 = model.vertices_[model.indices_[i + 0]];
            auto const &v1 = model.vertices_[model.indices_[i + 1]];
            auto const &v2 = model.vertices_[model.indices_[i + 2]];

            triangle<vec4, float> tri{v0, v1, v2};
            auto const ret = model.vertex_shader_->run(tri, model_matrix, view_matrix_, projection_matrix_);
            if (not ret.has_value()) {
               continue;
            }
            tri = ret->triangle;

            // Color color = WHITE;
            // color.r = static_cast<unsigned char>(fIncidence * 255.0f);
            // color.g = static_cast<unsigned char>(fIncidence * 255.0f);
            // color.b = static_cast<unsigned char>(fIncidence * 255.0f);
            // color.a = 255;

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

            clip_triangles_.emplace_front(tri, Color{ 255, 255, 255, 255 });
            for (auto const &pln : ndc_planes_) {
               auto iter = clip_triangles_.begin();
               while (iter != clip_triangles_.end()) {
                  auto const [clipped, one, two] = clip(iter->first, pln);
                  // completely behind plane? -> triangle invisible
                  if (not clipped and not one and not two) {
                     goto done;
                  } else if (clipped and one) {
                     assert(test_equal((*one)[0][3], 1.0f));
                     assert(test_equal((*one)[1][3], 1.0f));
                     assert(test_equal((*one)[2][3], 1.0f));
                     iter->first = *one;
                     iter->second = Color{ 255, 0, 0, 255 };
                     if (two) {
                        assert(test_equal((*two)[0][3], 1.0f));
                        assert(test_equal((*two)[1][3], 1.0f));
                        assert(test_equal((*two)[2][3], 1.0f));
                        iter = clip_triangles_.emplace_after(iter, *two, Color{ 0, 255, 0, 255 });
                     }
                  }
                  ++iter;
               }
            }

            //assert(the_normal[2] > 0.0f);

            // into screenspace
            for (auto &[triangle, color] : clip_triangles_) {
               triangle.clamp_all_vertices(-1.0f, 1.0f);

               triangle.transform(viewport_matrix_);
               //
               // constexpr static auto EPSILON = 0.001f;
               // assert(test_equal(triangle[0][0], 0.0f, EPSILON) or
               // test_equal(triangle[0][0], (float)renderbuffer_.width()-1, EPSILON)
               // or (triangle[0][0] > 0.0f and triangle[0][0] <
               // renderbuffer_.width()-1)); assert(test_equal(triangle[0][1], 0.0f,
               // EPSILON) or test_equal(triangle[0][1],
               // (float)renderbuffer_.height()-1, EPSILON) or (triangle[0][1] > 0.0f
               // and triangle[0][1] < renderbuffer_.height()-1));
               // assert(test_equal(triangle[1][0], 0.0f, EPSILON) or
               // test_equal(triangle[1][0], (float)renderbuffer_.width()-1, EPSILON)
               // or (triangle[1][0] > 0.0f and triangle[1][0] <
               // renderbuffer_.width()-1)); assert(test_equal(triangle[1][1], 0.0f,
               // EPSILON) or test_equal(triangle[1][1],
               // (float)renderbuffer_.height()-1, EPSILON) or (triangle[1][1] > 0.0f
               // and triangle[1][1] < renderbuffer_.height()-1));
               // assert(test_equal(triangle[2][0], 0.0f, EPSILON) or
               // test_equal(triangle[2][0], (float)renderbuffer_.width()-1, EPSILON)
               // or (triangle[2][0] > 0.0f and triangle[2][0] <
               // renderbuffer_.width()-1)); assert(test_equal(triangle[2][1], 0.0f,
               // EPSILON) or test_equal(triangle[2][1],
               // (float)renderbuffer_.height()-1, EPSILON) or (triangle[2][1] > 0.0f
               // and triangle[2][1] < renderbuffer_.height()-1));

               assert((int)triangle[0][0] >= 0 and
                     (int) triangle[0][0] < renderbuffer_.width() and
                     (int) triangle[0][1] >= 0 and
                     (int) triangle[0][1] < renderbuffer_.height());
               assert((int)triangle[1][0] >= 0 and
                     (int) triangle[1][0] < renderbuffer_.width() and
                     (int) triangle[1][1] >= 0 and
                     (int) triangle[1][1] < renderbuffer_.height());
               assert((int)triangle[2][0] >= 0 and
                     (int) triangle[2][0] < renderbuffer_.width() and
                     (int) triangle[2][1] >= 0 and
                     (int) triangle[2][1] < renderbuffer_.height());

               // assert(triangle[0][0] >= 0.0f and triangle[0][0] <=
               // renderbuffer_.width() and triangle[0][1] >= 0.0f and triangle[0][1]
               // <= renderbuffer_.height()); assert(triangle[1][0] >= 0.0f and
               // triangle[1][0] <= renderbuffer_.width() and triangle[1][1] >= 0.0f
               // and triangle[1][1] <= renderbuffer_.height()); assert(triangle[2][0]
               // >= 0.0f and triangle[2][0] <= renderbuffer_.width() and
               // triangle[2][1] >= 0.0f and triangle[2][1] <= renderbuffer_.height());
               renderbuffer_.draw_triangle(
                     triangle[0][0], triangle[0][1], triangle[0][2], triangle[1][0],
                     triangle[1][1], triangle[1][2], triangle[2][0], triangle[2][1],
                     triangle[2][2], color * ret->fIncidence, color * ret->fIncidence, color * ret->fIncidence, true);
            }
done:
         }
      }
   }

} // namespace fps::rendering
