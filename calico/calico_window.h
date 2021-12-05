#pragma once

#include "locator.h"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

namespace calico {

class calico_window {
public:
  calico_window(const std::string &name, int width, int height);
  ~calico_window();

  void set_should_close(bool val);
  [[nodiscard]] bool should_close() const;
  void poll_events();

private:
  int _width;
  int _height;
  bool _should_close;

  std::shared_ptr<spdlog::logger> _log;
  std::weak_ptr<spdlog::logger> _log_weak;

  std::shared_ptr<entt::dispatcher> _event_bus;
  std::weak_ptr<entt::dispatcher> _event_bus_weak;

  SDL_Window *_sdl_window;
};

} // namespace calico
