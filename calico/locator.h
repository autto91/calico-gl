#pragma once

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace calico {
struct locator {
  using logger = entt::service_locator<spdlog::logger>;
  using event_bus = entt::service_locator<entt::dispatcher>;
};
} // namespace calico
