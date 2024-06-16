#include "math/matrix.h"
#include <iostream>
#include <numbers>
#include <raylib.h>
#include <array>

const std::array<fps::math::vertex, 3> triangle = {
   fps::math::vertex{ 0.0f, 10.5f, -50.0f },
   fps::math::vertex{ -10.5f, -10.5f, -50.0f },
   fps::math::vertex{ 10.5f, -10.5f, -50.0f },
};

int main() {
   using namespace fps::math;

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

   auto p1 = persp * triangle[0];
   auto p2 = persp * triangle[1];
   auto p3 = persp * triangle[2];
   p1 = p1 / p1.w;
   p2 = p2 / p2.w;
   p3 = p3 / p3.w;
   p1 = viewp * p1;
   p2 = viewp * p2;
   p3 = viewp * p3;
   std::cout << "Triangle vertices:\n";
   std::cout << "P1: " << p1.x << ", " << p1.y << ", " << p1.z << ", " << p1.w << '\n';
   std::cout << "P2: " << p2.x << ", " << p2.y << ", " << p2.z << ", " << p2.w << '\n';
   std::cout << "P3: " << p3.x << ", " << p3.y << ", " << p3.z << ", " << p3.w << '\n';

         
   while (!WindowShouldClose()) {
      BeginDrawing();
         ClearBackground(BLACK);


         DrawTriangle(Vector2{ p1.x, p1.y }, Vector2{ p2.x, p2.y }, Vector2{ p3.x, p3.y }, RED);
      EndDrawing();
   }

   CloseWindow();

   return 0;
}
