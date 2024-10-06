cmake_minimum_required(VERSION 3.26)
project(TryingSFML)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.0)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(TryingSFML src/main.cpp
        header/Vec2.h
        header/EntityManager.h
        header/Game.h
        src/EntityManager.cpp
        src/Game.cpp
        src/Vec2.cpp
        header/Entity.h
        src/Entity.cpp
        header/Components.h)

target_link_libraries(TryingSFML sfml-graphics)