#pragma once

#include "renderer.h"
#include "window.h"

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

  void run() const;

private:
  int target_fps_;

  std::unique_ptr<window> window_;
  std::unique_ptr<renderer> renderer_;

  std::shared_ptr<spdlog::logger> log_;
  std::shared_ptr<entt::dispatcher> event_bus_;
};

} // namespace calico
