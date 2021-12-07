cmake_minimum_required(VERSION 3.14)

project(calico)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(calico STATIC Application.h Application.cpp calico.h event.h window.cpp window.h locator.h "renderer.h" "renderer.cpp" "event.cpp")
target_link_libraries(calico PRIVATE SDL2::SDL2 EnTT::EnTT spdlog::spdlog GLEW::GLEW imgui::imgui)

install(TARGETS calico DESTINATION lib)
install(FILES calico.h DESTINATION include)