#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

namespace calico {
struct calico_sdl_event {
  SDL_Event sdl_event;
};

class application {
public:
  application(const std::string &name, int width, int height);
  ~application();

  std::shared_ptr<spdlog::logger> logger;

  void run();

private:
  int window_width_;
  int window_height_;
  int target_fps_;

  bool should_close_;

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
  entt::dispatcher event_bus_;

  void register_sdl_event(SDL_Event e);
  void handle_global_sdl_event(const calico_sdl_event &e);
};

} // namespace calico
