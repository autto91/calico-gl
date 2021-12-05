#include "application.h"
#include <spdlog/sinks/stdout_color_sinks.h>

calico::application::application(const std::string &name, const int width,
                                 const int height)
    : logger(spdlog::stdout_color_mt("console")), window_width_(width),
      window_height_(height), target_fps_(60), should_close_(false),
      window_(nullptr, SDL_DestroyWindow), event_bus_(entt::dispatcher{}) {
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    logger->critical("unable to init SDL: {}", SDL_GetError());
    std::terminate();
  }

  window_.reset(SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, window_width_,
                                 window_height_, SDL_WINDOW_SHOWN));
  if (window_ == nullptr) {
    logger->critical("unable to create SDL window: {}", SDL_GetError());
    std::terminate();
  }
}

calico::application::~application() { SDL_Quit(); }

void calico::application::run() {
  // game loop initialization
  const int32_t tick_interval = 1000 / target_fps_;
  uint32_t last_update_time = 0;
  int32_t delta_time = 0;
  event_bus_.sink<calico_sdl_event>()
      .connect<&application::handle_global_sdl_event>(this);

  while (!should_close_) {
    // handle throttling and delta time
    const uint32_t current_time = SDL_GetTicks();
    delta_time = current_time - last_update_time;

    if (const int32_t time_to_sleep = tick_interval - delta_time;
        time_to_sleep > 0) {
      SDL_Delay(time_to_sleep);
    }
    last_update_time = current_time;

    // poll and register SDL events
    SDL_Event current_event;
    if (SDL_PollEvent(&current_event)) {
      register_sdl_event(current_event);
    }

    // process tick events
    event_bus_.update();
  }
}

void calico::application::register_sdl_event(const SDL_Event e) {
  event_bus_.enqueue<calico_sdl_event>({e});
}

void calico::application::handle_global_sdl_event(const calico_sdl_event &e) {
  if (e.sdl_event.type == SDL_QUIT) {
    should_close_ = true;
  }
}
