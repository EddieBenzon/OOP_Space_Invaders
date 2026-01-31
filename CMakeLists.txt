cmake_minimum_required(VERSION 3.16)
project(SpaceInvaders LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch raylib if not installed
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(Space-Invaders
    main.cpp
    Enemy.cpp
    EnemyLaser.cpp
    EnemyUFO.cpp
    Entity.cpp
    Explosion.cpp
    GameManager.cpp
    Laser.cpp
    Obstacle.cpp
    Player.cpp
    SpaceShip.cpp
)

target_include_directories(Space-Invaders PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Space-Invaders PRIVATE raylib)

if (UNIX)
    target_link_libraries(Space-Invaders PRIVATE m pthread dl)
endif()
