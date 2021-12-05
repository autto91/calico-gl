#pragma once

#include "calico_window.h"
#include "event.h"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

namespace calico {

class application {
public:
  application(const std::string &name, int width, int height);
  ~application();

  void run();

private:
  int _target_fps;

  std::unique_ptr<calico_window> _window;

  std::shared_ptr<spdlog::logger> _log;
  std::shared_ptr<entt::dispatcher> _event_bus;

  void handle_global_sdl_event(const calico_sdl_event &e);
};

} // namespace calico
