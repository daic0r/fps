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

using namespace fps::math;

const std::array<fps::math::vec4f, 3> triangle = {
   fps::math::vec4f{ 0.0f, 10.5f, -40.0f, 1.0f },
   fps::math::vec4f{ -10.5f, 0.5f, -40.0f, 1.0f },
   fps::math::vec4f{ 10.5f, -10.5f, -40.0f, 1.0f },
};

int main() {

   const auto width = 800;
   const auto height = 600;

   InitWindow(width, height, "Matrix Test");
   SetTargetFPS(60);

   const auto persp = matrix::perspective(std::numbers::pi_v<float> / 4.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
   const auto viewp = matrix::viewport(0, 0, width, height);

   std::cout << "Viweport matrix:\n";
   for (auto i = 0; i < 4; ++i) {
      for (auto j = 0; j < 4; ++j) {
         std::cout << viewp[i,j] << ' ';
      }
      std::cout << '\n';
   }

   Image img = GenImageColor(width, height, BLACK);
   ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
   auto tex = LoadTextureFromImage(img);
   fps::rendering::renderbuffer screen{ width, height };
   std::memset(screen.buffer(), 0, 800*600*4);

   auto p1 = persp * triangle[0];
   auto p2 = persp * triangle[1];
   auto p3 = persp * triangle[2];
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

   std::cout << std::endl;
   auto bres = fps::math::make_bresenham_interpolator(9, 9, 16, 13);
   auto ret = bres();
   while (ret) {
      std::cout << "(" << ret->first << ", " << ret->second << ")\n";
      ret = bres();
   }


         
   int avg_time = 0;
   while (!WindowShouldClose()) {
      BeginDrawing();
         //ClearBackground(BLACK);


         //DrawTriangle(Vector2{ p1[0], p1[1] }, Vector2{ p2[0], p2[1] }, Vector2{ p3[0], p3[1] }, RED);
         //draw_triangle(p1, p2, p3);
         auto start = std::chrono::steady_clock::now();
         // fps::rendering::draw_line(screen, p1[0], p1[1], p2[0], p2[1], RED);
         // fps::rendering::draw_line(screen, p2[0], p2[1], p3[0], p3[1], RED);
         // fps::rendering::draw_line(screen, p3[0], p3[1], p1[0], p1[1], RED);

         fps::rendering::draw_triangle(screen, p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], RED, GREEN, BLUE, true);
         auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);
         avg_time += dur.count();
         if (avg_time > 0)
            avg_time /= 2;
         UpdateTexture(tex, screen.buffer());
         DrawTexture(tex, 0, 0, WHITE);
         //fps::rendering::draw_triangle(400, 600, 300, 500, 500, 300, GREEN);
      EndDrawing();
   }

   UnloadTexture(tex);
   CloseWindow();

   std::cout << "Triangle drawing took on average " << avg_time << "ns" << std::endl;

   return 0;
}
