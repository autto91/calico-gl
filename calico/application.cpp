#include "application.h"
#include "locator.h"

#include <spdlog/sinks/stdout_color_sinks.h>

calico::application::application(const std::string &name, const int width,
                                 const int height)
    : target_fps_(60), window_(nullptr) {
  // create and register services
  log_ = spdlog::stdout_color_mt("console");
  event_bus_ = std::make_shared<entt::dispatcher>();

  locator::logger::set(log_);
  locator::event_bus::set(event_bus_);

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    log_->critical("unable to init SDL: {}", SDL_GetError());
    std::terminate();
  }

  // Create window
  window_ = std::make_unique<window>(name, width, height);
  renderer_ = std::make_unique<renderer>(window_->get_context());
}

calico::application::~application() { SDL_Quit(); }

void calico::application::run() const {
  // game loop initialization
  const int32_t tick_interval = 1000 / target_fps_;
  uint32_t last_update_time = 0;
  int32_t delta_time = 0;

  while (!window_->should_close()) {
    // handle throttling and delta time
    const uint32_t current_time = SDL_GetTicks();
    delta_time = current_time - last_update_time;

    if (const int32_t time_to_sleep = tick_interval - delta_time;
        time_to_sleep > 0) {
      SDL_Delay(time_to_sleep);
    }
    last_update_time = current_time;

    // poll and register SDL events
    window_->poll_events();

    // process tick events
    event_bus_->update();
  }
}
