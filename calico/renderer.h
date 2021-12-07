#pragma once

#include <SDL2/SDL.h>

namespace calico {

class renderer {
public:
  explicit renderer(SDL_GLContext ctx);
  ~renderer();

private:
  SDL_GLContext sdl_gl_context_;
};

} // namespace calico
