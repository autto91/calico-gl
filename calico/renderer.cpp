#include "renderer.h"

calico::renderer::renderer(const SDL_GLContext ctx) { sdl_gl_context_ = ctx; }

calico::renderer::~renderer() = default;
