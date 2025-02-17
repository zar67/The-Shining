//
// Created by Zoe on 16/12/2019.
//

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#include "../game/SceneObjects/GameObject.h"
#include "Room.h"
#include <vector>

class Map
{
 public:
  Map() = default;
  ~Map();

  void setupRoomCollision(int game_width, int game_height);
  void handlePlayerCollision(Player* player);
  void handleObjectCollision(std::vector<GameObject*> colliders);
  static void fixCollision(GameObject* object,
                           CollisionComponent* collided,
                           CollisionComponent::CollisionSide side);

  bool moveNorth();
  bool moveEast();
  bool moveSouth();
  bool moveWest();

  Room* getRoom(int id);
  Room* getCurrentRoom();
  void renderCurrentRoom(ASGE::Renderer* renderer);
  bool updateCurrentRoom(ASGE::Renderer* renderer,
                         AudioManager* audio,
                         double delta_time,
                         Player* player,
                         int game_width,
                         int game_height);
  void renderMiniMap(ASGE::Renderer* renderer);
  void generateStartingRoom(ASGE::Renderer* renderer);
  void generateNewRoom(ASGE::Renderer* renderer, int x_index, int y_index);
  void generateRooms(ASGE::Renderer* renderer, int game_width, int game_height);
  void setupMinimap(ASGE::Renderer* renderer, int game_width, int game_height);

  std::vector<GameObject*> getEnemies(bool include_objects);
  AxePsycho* axePsycho();

 private:
  void
  generateItemRooms(ASGE::Renderer* renderer, int game_width, int game_height);
  void setupBoundingBox(CollisionComponent* component,
                        float x_pos,
                        float y_pos,
                        float width,
                        float height);
  void checkNorthDoorCollision(Player* player);
  void checkEastDoorCollision(Player* player);
  void checkSouthDoorCollision(Player* player);
  void checkWestDoorCollision(Player* player);
  void
  generateEnemies(ASGE::Renderer* renderer, int game_width, int game_height);
  void updateMiniMap();
  std::string needNorthDoor(int x_pos, int y_pos);
  std::string needEastDoor(int x_pos, int y_pos);
  std::string needSouthDoor(int x_pos, int y_pos);
  std::string needWestDoor(int x_pos, int y_pos);
  bool checkRoomName(std::string name, std::string required_doors);
  bool roomChanged();

  const int STARTING_ROOM = 12;

  int map_size = 5;
  Room rooms[5][5];
  std::vector<GameObject*> mini_map;
  std::vector<int> mini_map_ids;

  int current_room = STARTING_ROOM;
  int last_room = STARTING_ROOM;

  CollisionComponent* room_wall_collision[8]{};
  CollisionComponent* room_door_collision[4]{};

  AxePsycho axe_psycho = AxePsycho();
  int game_width = 0;
  int game_height = 0;
};

#endif // PROJECT_MAP_H
