#include "math/matrix.h"
#include <cstring>
#include <iostream>
#include <numbers>
#include <raylib.h>
#include <array>
#include <cmath>
#include "rendering/utils.h"
#include <chrono>
#include <format>
#include "math/plane.h"
#include "math/triangle.h"
#include "math/clipping.h"
#include "rendering/master_renderer.h"
#include "rendering/model.h"

using namespace fps::math;

const std::array<fps::math::vec4f, 3> ar_triangle = {
   // fps::math::vec4f{ 0.0f, 10.5f, -40.0f, 1.0f },
   // fps::math::vec4f{ -10.5f, 0.5f, -40.0f, 1.0f },
   // fps::math::vec4f{ 10.5f, -10.5f, -40.0f, 1.0f },

   vec4f{ -10.0, -10.0, -40.0, 1.0 },
   vec4f{ 10.0, -10.0, -40.0, 1.0 },
   vec4f{ 0.0, 10.0, -40.0, 1.0 }
};

const auto cube = fps::rendering::model {
   std::vector<fps::math::vertex>{
      vertex{ -10.0, -10.0, -10.0, 1.0 },
      vertex{ 10.0, -10.0, -10.0, 1.0 },
      vertex{ 10.0, 10.0, -10.0, 1.0 },
      vertex{ -10.0, 10.0, -10.0, 1.0 },
      vertex{ -10.0, -10.0, 10.0, 1.0 }, // 4
      vertex{ 10.0, -10.0, 10.0, 1.0 },
      vertex{ 10.0, 10.0, 10.0, 1.0 }, // 6
      vertex{ -10.0, 10.0, 10.0, 1.0 }
   },
   // counter-clockwise indices
   std::vector<std::size_t>{
      0, 1, 2, 2, 3, 0, // front
      5, 4, 7, 7, 6, 5, // back 
      0, 4, 3, 3, 4, 7, // left
      5, 1, 6, 6, 1, 2, // right
      3, 2, 7, 7, 2, 6, // top
      0, 1, 4, 4, 1, 5  // bottom
      // 4, 5, 6, 6, 7, 4,
      // 1, 0, 2, 2, 0, 3,
      // 2, 3, 6, 6, 3, 7,
      // 6, 7, 5, 5, 7, 4,
      // 5, 4, 1, 1, 4, 0,
      // 0, 4, 3, 3, 4, 7,
   }
};

int main() {

   const auto width = 800;
   const auto height = 600;

   InitWindow(width, height, "Matrix Test");
   SetTargetFPS(60);

   const auto persp = matrix::perspective(std::numbers::pi_v<float> / 4.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
   const auto viewp = matrix::viewport(0, 0, width, height);

   std::cout << "Perspective matrix:\n";
   for (auto i = 0; i < 4; ++i) {
      for (auto j = 0; j < 4; ++j) {
         std::cout << persp[i,j] << ' ';
      }
      std::cout << '\n';
   }

   Image img = GenImageColor(width, height, BLACK);
   ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
   auto tex = LoadTextureFromImage(img);
   fps::rendering::renderbuffer screen{ width, height };
   std::memset(screen.buffer(), 0, 800*600*4);

   fps::rendering::master_renderer renderer{ screen };
   renderer.set_projection_matrix(persp);
   renderer.set_viewport_matrix(viewp);
   renderer.set_view_matrix(matrix::identity());
   renderer.add_model(cube);

   auto const transform_tri = [&](const auto& tri, vec4f& p1, vec4f &p2, vec4f &p3) {
      std::cout << "Triangle verticesz:\n";
      std::cout << "P1: " << tri[0][0] << ", " << tri[0][1] << ", " << tri[0][2] << ", " << tri[0][3] << '\n';
      std::cout << "P2: " << tri[1][0] << ", " << tri[1][1] << ", " << tri[1][2] << ", " << tri[1][3] << '\n';
      std::cout << "P3: " << tri[2][0] << ", " << tri[2][1] << ", " << tri[2][2] << ", " << tri[2][3] << '\n';
      p1 = persp * tri[0];
      p2 = persp * tri[1];
      p3 = persp * tri[2];
      p1 = p1 / p1[3];
      p2 = p2 / p2[3];
      p3 = p3 / p3[3];
      std::cout << "Triangle vertices:\n";
      std::cout << "P1: " << p1[0] << ", " << p1[1] << ", " << p1[2] << ", " << p1[3] << '\n';
      std::cout << "P2: " << p2[0] << ", " << p2[1] << ", " << p2[2] << ", " << p2[3] << '\n';
      std::cout << "P3: " << p3[0] << ", " << p3[1] << ", " << p3[2] << ", " << p3[3] << '\n';
      p1 = viewp * p1;
      p2 = viewp * p2;
      p3 = viewp * p3;
      std::cout << "Triangle vertices:\n";
      std::cout << "P1: " << p1[0] << ", " << p1[1] << ", " << p1[2] << ", " << p1[3] << '\n';
      std::cout << "P2: " << p2[0] << ", " << p2[1] << ", " << p2[2] << ", " << p2[3] << '\n';
      std::cout << "P3: " << p3[0] << ", " << p3[1] << ", " << p3[2] << ", " << p3[3] << '\n';
      std::cout << "=====================================================================\n";
   };
   //
   // std::cout << std::endl;
   // auto bres = fps::math::make_bresenham_interpolator(9, 9, 16, 13);
   // auto ret = bres();
   // while (ret) {
   //    std::cout << "(" << ret->first << ", " << ret->second << ")\n";
   //    ret = bres();
   // }
   //
   vec4f p1, p2, p3, p4, p5, p6;
   plane pln{ vec3f{ -1.0f, 0.0f, 0.0f }, vec3f{ 2.0f, 0.0f, 0.0f } };
   triangle tri{ vec4f{ ar_triangle[0] }, vec4f{ ar_triangle[1] }, vec4f{ ar_triangle[2] } };
   transform_tri(tri, p1, p2, p3);
   // auto clipped = fps::math::clip(tr, pln);
   // transform_tri(*std::get<1>(clipped), p1, p2, p3);
   // if (std::get<2>(clipped))
   //    transform_tri(*std::get<2>(clipped), p4, p5, p6);

         
   int avg_time = 0;
   std::array<matrix, 1> instances{ matrix::identity() };
   float fAngle = 0.0f;
   while (!WindowShouldClose()) {
      BeginDrawing();
         ClearBackground(BLACK);
         screen.clear();
         instances[0] = matrix::translate(0.0f, 0.0f, -30.0f) * matrix::rotate_y(fAngle);

         //DrawTriangle(Vector2{ p1[0], p1[1] }, Vector2{ p2[0], p2[1] }, Vector2{ p3[0], p3[1] }, RED);
         //draw_triangle(p1, p2, p3);
         auto start = std::chrono::steady_clock::now();
         // fps::rendering::draw_line(screen, p1[0], p1[1], p2[0], p2[1], RED);
         // fps::rendering::draw_line(screen, p2[0], p2[1], p3[0], p3[1], RED);
         // fps::rendering::draw_line(screen, p3[0], p3[1], p1[0], p1[1], RED);

         //fps::rendering::draw_triangle(screen, p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], RED, RED, RED, true);
         //fps::rendering::draw_triangle(screen, p4[0], p4[1], p5[0], p5[1], p6[0], p6[1], BLUE, BLUE, BLUE, true);
         renderer.render_model(0, instances); 
         auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);
         avg_time += dur.count();
         if (avg_time > 0)
            avg_time /= 2;
         UpdateTexture(tex, screen.buffer());
         DrawTexture(tex, 0, 0, WHITE);
         //fps::rendering::draw_triangle(400, 600, 300, 500, 500, 300, GREEN);
      EndDrawing();
      fAngle += 0.01f;
      fAngle = std::fmod(fAngle, 2.0f * std::numbers::pi_v<float>);
   }

   UnloadTexture(tex);
   CloseWindow();

   std::cout << "Triangle drawing took on average " << avg_time << "ns" << std::endl;

   return 0;
}
