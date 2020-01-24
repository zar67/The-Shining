//
// Created by Zoe on 16/12/2019.
//

#ifndef PROJECT_ROOM_H
#define PROJECT_ROOM_H

#include "../game/SceneObjects/Enemies/Demon.h"
#include "../game/SceneObjects/Enemies/Ghost.h"
#include "../game/SceneObjects/GameObject.h"
#include "../game/SceneObjects/InteractableObjects.h"
#include "../game/SceneObjects/Items.h"
#include <string>
#include <vector>

class Room : public GameObject
{
 public:
  enum RoomType
  {
    NORMAL,
    ITEM,
    EXIT
  };

  Room() = default;
  Room(int id,
       RoomType room_type,
       bool n_door,
       bool e_door,
       bool s_door,
       bool w_door);
  ~Room();

  bool setup(ASGE::Renderer* renderer, std::string* filename);

  int getId();
  RoomType getType();
  void setType(RoomType room_type);

  bool getNorth();
  bool getEast();
  bool getSouth();
  bool getWest();

  bool canMove();
  void canMove(bool movement);

  void renderObjectsInRoom(ASGE::Renderer* renderer);
  void updateObjectsInRoom(ASGE::Renderer* renderer,
                           double delta_time,
                           Player* player);

  void addDemonToRoom(ASGE::Renderer* renderer, float x_pos, float y_pos);
  void addGhostToRoom(ASGE::Renderer* renderer, float x_pos, float y_pos);

  std::vector<InteractableObjects*> getObjectsInRoom();
  std::vector<GameObject*> getEnemies(bool inlcude_objects);
  void checkEnemyHealth();

  void addItemToRoom(ASGE::Renderer* renderer, float x_pos, float y_pos);
  void removeItemToRoom(int item_index);

 private:
  int ID = -1;
  RoomType type = NORMAL;
  bool north = false;
  bool east = false;
  bool south = false;
  bool west = false;
  bool movement_enabled = true;

  // zoe remeber to delete the eneimes when you change room/ memory leak other
  // wise
  std::vector<Demon*> demons;
  std::vector<Ghost*> ghosts;

  // objects in the room
  std::vector<InteractableObjects*> interactable_objs;
  std::string texture_dir = "data/Objects/crate2.png";
  std::vector<Items*> items;
};

#endif // PROJECT_ROOM_H
