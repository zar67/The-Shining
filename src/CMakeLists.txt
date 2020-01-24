cmake_minimum_required(VERSION 3.11.4)
project(TheShining)
set(GAMEDATA_FOLDER "data")
set(ENABLE_ENET  OFF  CACHE BOOL "Adds Networking"   FORCE)
set(ENABLE_SOUND OFF   CACHE BOOL "Adds SoLoud Audio" FORCE)
set(ENABLE_JSON  ON   CACHE BOOL "Adds JSON to the Project" FORCE)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## out of source builds ##
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
## out of source builds ##

## add the files to be compiled here
set(HEADER_FILES
        game/main.cpp
        game/game.cpp
        map/Room.cpp
        map/Map.cpp
        game/SceneObjects/GameObject.cpp
        game/Components/SpriteComponent.cpp
        game/Components/CollisionComponent.cpp
        game/SceneObjects/Player.cpp
        game/SceneObjects/Enemies/Enemy.cpp
        game/SceneObjects/Enemies/Demon.cpp
        game/SceneObjects/Projectile.cpp
        game/SceneObjects/Enemies/Ghost.cpp
        game/SceneObjects/Enemies/AxePsycho.cpp
        game/UIHandler/Scene.cpp
        game/UIHandler/SceneManager.cpp
        game/UIHandler/Scenes/MainMenu.cpp
        game/UIHandler/Scenes/GameOverMenu.cpp
        game/UIHandler/Scenes/ShopMenu.cpp
        game/UIHandler/Scenes/GameScene.cpp
        game/Components/ShootingComponent.cpp
        game/SceneObjects/Items.cpp
        game/game.h
        map/Room.h
        map/Map.h
        game/SceneObjects/GameObject.h
        game/Components/SpriteComponent.h
        game/Components/CollisionComponent.h
        game/SceneObjects/Player.h
        game/SceneObjects/Enemies/Enemy.h
        game/SceneObjects/Enemies/Demon.h
        game/SceneObjects/Projectile.h
        game/SceneObjects/Enemies/Ghost.h
        game/SceneObjects/Enemies/AxePsycho.h
        game/Components/ShootingComponent.h
        game/SceneObjects/Items.h
        game/SceneObjects/InteractableObjects.cpp
        game/SceneObjects/InteractableObjects.h)

## the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

## these are the build directories
get_target_property(CLIENT ${PROJECT_NAME} NAME)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${CLIENT}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${CLIENT}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${CLIENT}/bin")

## important build scripts
include(build/compilation)
include(libs/asge)
include(libs/json)
include(libs/soloud)
include(tools/itch.io)

## hide console unless debug build ##
if (NOT CMAKE_BUILD_TYPE STREQUAL  "Debug" AND WIN32)
    target_compile_options(${PROJECT_NAME} -mwindows)
endif()


