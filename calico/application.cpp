#include "application.h"
#include <spdlog/sinks/stdout_color_sinks.h>

calico::application::application(const std::string &name, const int width,
                                 const int height)
    : _target_fps(60), _window(nullptr) {
  // create and register services
  _log = spdlog::stdout_color_mt("console");
  _event_bus = std::make_shared<entt::dispatcher>();

  locator::logger::set(_log);
  locator::event_bus::set(_event_bus);

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    _log->critical("unable to init SDL: {}", SDL_GetError());
    std::terminate();
  }

  // Create calico_window
  _window = std::make_unique<calico_window>(name, width, height);
}

calico::application::~application() { SDL_Quit(); }

void calico::application::run() {
  // game loop initialization
  const int32_t tick_interval = 1000 / _target_fps;
  uint32_t last_update_time = 0;
  int32_t delta_time = 0;

  _event_bus->sink<calico_sdl_event>()
      .connect<&application::handle_global_sdl_event>(this);

  while (!_window->should_close()) {
    // handle throttling and delta time
    const uint32_t current_time = SDL_GetTicks();
    delta_time = current_time - last_update_time;

    if (const int32_t time_to_sleep = tick_interval - delta_time;
        time_to_sleep > 0) {
      SDL_Delay(time_to_sleep);
    }
    last_update_time = current_time;

    // poll and register SDL events
    _window->poll_events();

    // process tick events
    _event_bus->update();
  }
}

void calico::application::handle_global_sdl_event(const calico_sdl_event &e) {
  if (e.sdl_event.type == SDL_QUIT) {
    _window->set_should_close(true);
  }
}
