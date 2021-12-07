#pragma once

#include "event.h"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

namespace calico {

class window {
public:
  window(const std::string &name, int width, int height);
  ~window();

  void poll_events() const;

  [[nodiscard]] bool should_close() const;
  [[nodiscard]] SDL_GLContext get_context() const;

private:
  int width_;
  int height_;
  bool should_close_;

  std::shared_ptr<spdlog::logger> log_;
  std::weak_ptr<spdlog::logger> log_weak_;

  std::shared_ptr<entt::dispatcher> event_bus_;
  std::weak_ptr<entt::dispatcher> event_bus_weak_;

  SDL_Window *sdl_window_;
  SDL_GLContext sdl_gl_context_;

  void handle_global_window_events(const global_window_event &e);
};

} // namespace calico
