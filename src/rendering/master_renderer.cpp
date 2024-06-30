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
            tri.transform(pv_matrix * model_matrix);
            tri[0] = tri[0] / tri[0][3];
            tri[1] = tri[1] / tri[1][3];
            tri[2] = tri[2] / tri[2][3];
            bool bCompletelyContained{ true };
            clip_triangles_.push_back(tri);
            auto end_iter = clip_triangles_.end();
            auto new_end_iter = clip_triangles_.end();
            for (auto const& pln : ndc_planes_) {
               auto iter = clip_triangles_.begin();
               while (iter != end_iter) {
                  auto old_iter = iter;
                  auto const [clipped, one, two] = clip(*iter, pln);
                  // completely behind plane? -> triangle invisible
                  if (not clipped and not one and not two) {
                     goto done;
                  } else
                  if (clipped and one) {
                     bCompletelyContained = false;
                     new_end_iter = clip_triangles_.insert(clip_triangles_.end(), *one);
                     if (two) {
                        new_end_iter = clip_triangles_.insert(clip_triangles_.end(), *two);
                     }
                     if (*iter == tri) {
                        ++iter;
                        clip_triangles_.erase(old_iter);
                     }
                     // if (one) {
                     //    ++iter;
                     //    if (two) {
                     //       ++iter;
                     //    }
                     // }
                  }
                  if (old_iter == iter)
                     ++iter;
                  old_iter = iter;
               }
               end_iter = new_end_iter;
            }
            if (bCompletelyContained) {
               clip_triangles_.push_back(tri);
            }

            // into screenspace
            for (auto& triangle : clip_triangles_) {
               triangle.transform(viewport_matrix_);
               draw_triangle(renderbuffer_, triangle[0][0], triangle[0][1], triangle[1][0], triangle[1][1], triangle[2][0], triangle[2][1], WHITE, WHITE, WHITE, false);
            }
done:
         }
      }
   }

}
