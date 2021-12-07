#include "window.h"
#include "event.h"
#include "locator.h"

#include <gl/glew.h>

#include <SDL2/SDL_opengl.h>
#include <entt/entt.hpp>

calico::window::window(const std::string &name, const int width,
                       const int height)
    : width_(width), height_(height), should_close_(false) {
  log_weak_ = locator::logger::get();
  event_bus_weak_ = locator::event_bus::get();

  log_ = log_weak_.lock();
  event_bus_ = event_bus_weak_.lock();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  sdl_window_ = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, width_, height_,
                                 SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (sdl_window_ == nullptr) {
    log_->critical("unable to create SDL window: {}", SDL_GetError());
    std::terminate();
  }

  sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
  if (const auto glew_status = glewInit(); glew_status != GLEW_OK) {
    log_->critical("unable to create OpenGL context: {}",
                   glewGetErrorString(glew_status));
    std::terminate();
  }

  event_bus_->sink<global_window_event>()
      .connect<&window::handle_global_window_events>(this);
}

calico::window::~window() {
  log_weak_.reset();
  event_bus_weak_.reset();
  SDL_DestroyWindow(sdl_window_);
}

bool calico::window::should_close() const { return should_close_; }

void calico::window::poll_events() const {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    event_bus_->enqueue<global_window_event>({{false}, event});
  }
}

SDL_GLContext calico::window::get_context() const { return sdl_gl_context_; }

void calico::window::handle_global_window_events(const global_window_event &e) {
  if (e.sdl_event.type == SDL_QUIT) {
    should_close_ = true;
  }
}
