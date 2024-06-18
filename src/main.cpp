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

const std::array<fps::math::vertex, 3> triangle = {
   fps::math::vertex{ 0.0f, 10.5f, -50.0f },
   fps::math::vertex{ -10.5f, -10.5f, -50.0f },
   fps::math::vertex{ 10.5f, -10.5f, -50.0f },
};

void draw_triangle(const vertex& v1, const vertex& v2, const vertex& v3) {
   float dy = (v2.y - v1.y) / std::fabs(v2.x - v1.x);
   int add_or_sun = v2.x - v1.x >= 0.0f ? 1 : -1;
   float y = v1.y;
   for (int x = v1.x; x != v2.x; x += add_or_sun) {
      DrawPixel(x, y, RED);
      y += dy;
   }
}

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
   p1 = p1 / p1.w;
   p2 = p2 / p2.w;
   p3 = p3 / p3.w;
   std::cout << "Triangle vertices:\n";
   std::cout << "P1: " << p1.x << ", " << p1.y << ", " << p1.z << ", " << p1.w << '\n';
   std::cout << "P2: " << p2.x << ", " << p2.y << ", " << p2.z << ", " << p2.w << '\n';
   std::cout << "P3: " << p3.x << ", " << p3.y << ", " << p3.z << ", " << p3.w << '\n';
   p1 = viewp * p1;
   p2 = viewp * p2;
   p3 = viewp * p3;
   std::cout << "Triangle vertices:\n";
   std::cout << "P1: " << p1.x << ", " << p1.y << ", " << p1.z << ", " << p1.w << '\n';
   std::cout << "P2: " << p2.x << ", " << p2.y << ", " << p2.z << ", " << p2.w << '\n';
   std::cout << "P3: " << p3.x << ", " << p3.y << ", " << p3.z << ", " << p3.w << '\n';

         
   int avg_time = 0;
   while (!WindowShouldClose()) {
      BeginDrawing();
         //ClearBackground(BLACK);


         //DrawTriangle(Vector2{ p1.x, p1.y }, Vector2{ p2.x, p2.y }, Vector2{ p3.x, p3.y }, RED);
         //draw_triangle(p1, p2, p3);
         // fps::rendering::draw_line(p1.x, p1.y, p2.x, p2.y, RED);
         // fps::rendering::draw_line(p2.x, p2.y, p3.x, p3.y, RED);
         // fps::rendering::draw_line(p3.x, p3.y, p1.x, p1.y, RED);
         auto start = std::chrono::steady_clock::now();
         fps::rendering::draw_triangle(screen, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, RED);
         auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
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
