cmake_minimum_required(VERSION 3.14)

project(calico)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(calico STATIC Application.h Application.cpp calico.h event.h window.cpp window.h locator.h)
target_link_libraries(calico PRIVATE SDL2::SDL2 EnTT::EnTT spdlog::spdlog spdlog::spdlog_header_only)

install(TARGETS calico DESTINATION lib)
install(FILES calico.h DESTINATION include)