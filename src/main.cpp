#include "math/matrix.h"
#include <cstring>
#include <iostream>
#include <numbers>
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
#include "shaders/model_vertexshader.h"
#include <SDL2/SDL.h>
#include <rendering/fps_camera.h>
#include <importers/ModelImporterOBJ.h>

using namespace fps::math;

const std::array<fps::math::vec4f, 3> ar_triangle = {
   // fps::math::vec4f{ 0.0f, 10.5f, -40.0f, 1.0f },
   // fps::math::vec4f{ -10.5f, 0.5f, -40.0f, 1.0f },
   // fps::math::vec4f{ 10.5f, -10.5f, -40.0f, 1.0f },

   vec4f{ -10.0, -10.0, -40.0, 1.0 },
   vec4f{ 10.0, -10.0, -40.0, 1.0 },
   vec4f{ 0.0, 10.0, -40.0, 1.0 }
};

auto cube = fps::rendering::model {
   std::vector<fps::math::vertex>{
      vertex{ -10.0, -10.0, -10.0, 1.0 }, // back bottom left
      vertex{ 10.0, -10.0, -10.0, 1.0 }, // back bottom right
      vertex{ 10.0, 10.0, -10.0, 1.0 }, // back top right
      vertex{ -10.0, 10.0, -10.0, 1.0 }, // back top left
      vertex{ -10.0, -10.0, 10.0, 1.0 }, // 4 front bottom left
      vertex{ 10.0, -10.0, 10.0, 1.0 }, // 5 front bottom right
      vertex{ 10.0, 10.0, 10.0, 1.0 }, // 6 front top right
      vertex{ -10.0, 10.0, 10.0, 1.0 } // 7 front top left
   },
   std::vector<std::size_t>{
      // counter-clockwise indices
      // right-handed system
      // negative z-axis is into the screen
      1, 0, 3, 3, 2, 1, // back
      4, 5, 6, 6, 7, 4, // front
      0, 4, 7, 7, 3, 0, // left
      5, 1, 2, 2, 6, 5, // right
      3, 7, 6, 6, 2, 3, // top
      0, 1, 5, 5, 4, 0 // bottom
   }
};

void check_normals() {
   for (int i = 0; i < cube.indices_.size(); i += 6) {
      auto tri = triangle{ cube.vertices_[cube.indices_[i + 0]], cube.vertices_[cube.indices_[i + 1]], cube.vertices_[cube.indices_[i + 2]] };
      auto tri2 = triangle{ cube.vertices_[cube.indices_[i + 3]], cube.vertices_[cube.indices_[i + 4]], cube.vertices_[cube.indices_[i + 5]] };
      auto n = tri.normal();
      auto n2 = tri2.normal();
      std::cout << "Normal: " << n[0] << ", " << n[1] << ", " << n[2] << '\n';
      std::cout << "Normal: " << n2[0] << ", " << n2[1] << ", " << n2[2] << '\n';
      std::cout << "=====================================================================\n";
   }
}

int main() {
   std::map<std::string, fps::rendering::model> models;
   fps::importers::ModelImporterOBJ importer{ "assets/VideoShip.obj" };
   if (importer.import(models)) {
      std::cout << "Imported " << models.size() << " models\n";
   } else {
      std::cout << "Failed to import models\n";
   }

   const auto width = 800;
   const auto height = 600;

   check_normals();

   // InitWindow(width, height, "Matrix Test");
   // SetTargetFPS(60);

   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window* window = SDL_CreateWindow("SDL2 Window",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

   SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   SDL_Texture* texture = SDL_CreateTexture(sdl_renderer,
      SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
   //SDL_SetRenderTarget(sdl_renderer, texture);

   const auto persp = matrix::perspective(std::numbers::pi_v<float> / 4.0f, static_cast<float>(width) / height, 0.1f, 1000.0f);
   const auto viewp = matrix::viewport(0, 0, width, height);

   std::cout << "Perspective matrix:\n";
   for (auto i = 0; i < 4; ++i) {
      for (auto j = 0; j < 4; ++j) {
         std::cout << persp[i,j] << ' ';
      }
      std::cout << '\n';
   }
   std::cout << "Viewport matrix:\n";
   for (auto i = 0; i < 4; ++i) {
      for (auto j = 0; j < 4; ++j) {
         std::cout << viewp[i,j] << ' ';
      }
      std::cout << '\n';
   }

   // Image img = GenImageColor(width, height, BLACK);
   // ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
   // auto tex = LoadTextureFromImage(img);
   // UnloadImage(img);
   fps::rendering::renderbuffer screen{ width, height };
   std::memset(screen.buffer(), 0, 800*600*4);

   fps::rendering::fps_camera cam{ vec3f{ 0.0f, 0.0f, 50.0f }, 0.0f, 0.0f };
   
   cube.vertex_shader_ = std::make_unique<fps::rendering::model_vertexshader>();
   models["VideoShip"].vertex_shader_ = std::make_unique<fps::rendering::model_vertexshader>();

   fps::rendering::master_renderer renderer{ screen };
   renderer.set_projection_matrix(persp);
   renderer.set_viewport_matrix(viewp);
   renderer.set_view_matrix(cam.view_matrix());
   renderer.add_model(std::move(cube));
   renderer.add_model(std::move(models["VideoShip"]));

   auto const transform_tri = [&](const auto& tri, vec4f& p1, vec4f &p2, vec4f &p3) {
      std::cout << "Triangle verticesz:\n";
      std::cout << "P1: " << tri[0][0] << ", " << tri[0][1] << ", " << tri[0][2] << ", " << tri[0][3] << '\n';
      std::cout << "P2: " << tri[1][0] << ", " << tri[1][1] << ", " << tri[1][2] << ", " << tri[1][3] << '\n';
      std::cout << "P3: " << tri[2][0] << ", " << tri[2][1] << ", " << tri[2][2] << ", " << tri[2][3] << '\n';
      p1 = persp * tri[0];
      p2 = persp * tri[1];
      p3 = persp * tri[2];
      std::cout << "Triangle vertices (before w-division):\n";
      std::cout << "P1: " << p1[0] << ", " << p1[1] << ", " << p1[2] << ", " << p1[3] << '\n';
      std::cout << "P2: " << p2[0] << ", " << p2[1] << ", " << p2[2] << ", " << p2[3] << '\n';
      std::cout << "P3: " << p3[0] << ", " << p3[1] << ", " << p3[2] << ", " << p3[3] << '\n';
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

   //auto render_tex = LoadRenderTexture(width, height);
         
   int avg_time = 0;
   std::array<matrix, 1> instances{ matrix::identity() };
   std::array<matrix, 1> instances2{ matrix::identity() };
   float fAngle = 0.0f;
   SDL_Event event;
   while (true) {
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT)
            goto quit;
         else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE)
               goto quit;
            else if (event.key.keysym.sym == SDLK_w)
               cam.move_forward(1.1f);
            else if (event.key.keysym.sym == SDLK_s)
               cam.move_forward(-1.1f);
            else if (event.key.keysym.sym == SDLK_a)
               cam.move_right(-1.1f);
            else if (event.key.keysym.sym == SDLK_d)
               cam.move_right(1.1f);
         }
         else if (event.type == SDL_MOUSEMOTION) {
            cam.inc_yaw(event.motion.xrel * 0.01f);
            cam.inc_pitch(event.motion.yrel * 0.01f);
         }
      }
      if (cam.dirty()) {
         renderer.set_view_matrix(cam.view_matrix());
      }
      instances[0] = matrix::translate(-15.0f, 0.0f, -50.0f) * matrix::rotate_y(fAngle) * matrix::rotate_x(fAngle) * matrix::rotate_z(fAngle);
      instances2[0] = matrix::translate(10.0f, 0.0f, -40.0f) * matrix::rotate_y(-fAngle) * matrix::rotate_x(-fAngle) * matrix::rotate_z(-fAngle);

      void *pixels;
      int pitch;
      SDL_LockTexture(texture, NULL, &pixels, &pitch);
      assert(pixels);
      assert(pitch == width * 4);
      screen.set_texture_ptr(static_cast<unsigned char*>(pixels));
      screen.clear();
      //DrawTriangle(Vector2{ p1[0], p1[1] }, Vector2{ p2[0], p2[1] }, Vector2{ p3[0], p3[1] }, RED);
      //draw_triangle(p1, p2, p3);
      auto start = std::chrono::steady_clock::now();
      // fps::rendering::draw_line(screen, p1[0], p1[1], p2[0], p2[1], RED);
      // fps::rendering::draw_line(screen, p2[0], p2[1], p3[0], p3[1], RED);
      // fps::rendering::draw_line(screen, p3[0], p3[1], p1[0], p1[1], RED);

      //fps::rendering::draw_triangle(screen, p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], RED, RED, RED, true);
      //fps::rendering::draw_triangle(screen, p4[0], p4[1], p5[0], p5[1], p6[0], p6[1], BLUE, BLUE, BLUE, true);
      renderer.render_model(0, instances); 
      renderer.render_model(1, instances2); 
      auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);
      avg_time += dur.count();
      if (avg_time > 0)
         avg_time /= 2;
      // UpdateTexture(tex, screen.buffer());
      //SDL_UpdateTexture(texture, NULL, screen.buffer(), width * 4);
      SDL_UnlockTexture(texture);
      //SDL_RenderClear(sdl_renderer);
      SDL_RenderCopy(sdl_renderer, texture, NULL, NULL);
      SDL_RenderPresent(sdl_renderer);
      // BeginDrawing();
      //    ClearBackground(BLACK);
      //    DrawTexture(tex, 0, 0, WHITE);
      // EndDrawing();
      fAngle += 0.01f;
      fAngle = std::fmod(fAngle, 2.0f * std::numbers::pi_v<float>);
   }
quit:

   SDL_DestroyTexture(texture);
   SDL_DestroyRenderer(sdl_renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
   // UnloadTexture(tex);
   // CloseWindow();

   std::cout << "Triangle drawing took on average " << avg_time << "ns" << std::endl;

   return 0;
}
