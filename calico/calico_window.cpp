#include "calico_window.h"
#include "event.h"
#include <entt/entt.hpp>

calico::calico_window::calico_window(const std::string &name, int width,
                                     int height)
    : _width(width), _height(height), _should_close(false) {
  _log_weak = locator::logger::get();
  _event_bus_weak = locator::event_bus::get();

  _log = _log_weak.lock();
  _event_bus = _event_bus_weak.lock();

  _sdl_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, _width, _height,
                                 SDL_WINDOW_SHOWN);
  if (_sdl_window == nullptr) {
    _log->critical("unable to create SDL calico_window: {}", SDL_GetError());
    std::terminate();
  }
}

calico::calico_window::~calico_window() {
  _log_weak.reset();
  _event_bus_weak.reset();
  SDL_DestroyWindow(_sdl_window);
}

bool calico::calico_window::should_close() const { return _should_close; }

void calico::calico_window::set_should_close(bool val) { _should_close = val; }

void calico::calico_window::poll_events() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    _event_bus->enqueue<calico_sdl_event>({event});
  }
}
