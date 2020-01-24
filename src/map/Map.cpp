//
// Created by Zoe on 16/12/2019.
//

#include <Engine/FileIO.h>
#include <array>
#include <iostream>
#include <queue>

#include "Map.h"

Map::~Map()
{
  for (auto room : mini_map)
  {
    delete room;
  }
  mini_map.clear();

  for (int i = 0; i < 8; i++)
  {
    delete room_wall_collision[i];
    room_wall_collision[i] = nullptr;
  }

  for (int i = 0; i < 4; i++)
  {
    delete room_door_collision[i];
    room_door_collision[i] = nullptr;
  }
}

void Map::setupRoomCollision()
{
  for (int i = 0; i < 8; i++)
  {
    room_wall_collision[i] = new CollisionComponent();
  }

  for (int i = 0; i < 4; i++)
  {
    room_door_collision[i] = new CollisionComponent();
  }

  setupBoundingBox(room_wall_collision[0], 0, 0, 64, 192);
  setupBoundingBox(room_wall_collision[1], 0, 0, 320, 64);
  setupBoundingBox(room_wall_collision[2], 384, 0, 320, 64);
  setupBoundingBox(room_wall_collision[3], 640, 0, 64, 192);

  setupBoundingBox(room_wall_collision[4], 0, 256, 64, 192);
  setupBoundingBox(room_wall_collision[5], 0, 384, 320, 64);
  setupBoundingBox(room_wall_collision[6], 384, 384, 320, 64);
  setupBoundingBox(room_wall_collision[7], 640, 256, 64, 192);

  setupBoundingBox(room_door_collision[0], 320, 0, 64, 64);
  setupBoundingBox(room_door_collision[1], 640, 192, 64, 64);
  setupBoundingBox(room_door_collision[2], 320, 384, 64, 64);
  setupBoundingBox(room_door_collision[3], 0, 192, 64, 64);
}

void Map::handlePlayerCollision(Player* player)
{
  CollisionComponent* player_collider = player->collisionComponent();
  player->updateCollisionComponent();

  for (auto& wall : room_wall_collision)
  {
    if (player_collider->hasCollided(*wall))
    {
      CollisionComponent::CollisionSide side =
        player_collider->getCollisionSide(*wall);
      fixCollision(player, wall, side);
    }
  }

  std::vector<InteractableObjects*> objects =
    getCurrentRoom()->getObjectsInRoom();
  for (auto& obj : objects)
  {
    bool collision = player_collider->hasCollided(*obj->collisionComponent());
    if (collision)
    {
      CollisionComponent::CollisionSide side =
        player_collider->getCollisionSide(*obj->collisionComponent());
      fixCollision(player, obj->collisionComponent(), side);
    }
  }

  checkNorthDoorCollision(player);
  checkEastDoorCollision(player);
  checkSouthDoorCollision(player);
  checkWestDoorCollision(player);
}

void Map::handleObjectCollision(std::vector<GameObject*> colliders)
{
  std::vector<InteractableObjects*> scene_objects =
    getCurrentRoom()->getObjectsInRoom();
  for (auto& col : colliders)
  {
    col->updateCollisionComponent();

    for (auto& wall : room_wall_collision)
    {
      if (col->collisionComponent()->hasCollided(*wall))
      {
        CollisionComponent::CollisionSide side =
          col->collisionComponent()->getCollisionSide(*wall);
        fixCollision(col, wall, side);
      }
    }

    for (auto& door : room_door_collision)
    {
      if (col->collisionComponent()->hasCollided(*door))
      {
        CollisionComponent::CollisionSide side =
          col->collisionComponent()->getCollisionSide(*door);
        fixCollision(col, door, side);
      }
    }

    for (auto& obj : scene_objects)
    {
      if (col->collisionComponent()->hasCollided(*obj->collisionComponent()))
      {
        CollisionComponent::CollisionSide side =
          col->collisionComponent()->getCollisionSide(
            *obj->collisionComponent());
        fixCollision(col, obj->collisionComponent(), side);
      }
    }
  }
}

void Map::fixCollision(GameObject* object,
                       CollisionComponent* collided,
                       CollisionComponent::CollisionSide side)
{
  float col_bounding_box[4];
  collided->getBoundingBox(col_bounding_box);

  ASGE::Sprite* object_sprite = object->spriteComponent()->getSprite();

  if (side == CollisionComponent::CollisionSide::SIDE_LEFT)
  {
    float x_pos = col_bounding_box[0] - object_sprite->width();
    object_sprite->xPos(x_pos);
  }

  else if (side == CollisionComponent::CollisionSide::SIDE_RIGHT)
  {
    float x_pos = col_bounding_box[0] + col_bounding_box[2];
    object_sprite->xPos(x_pos);
  }

  else if (side == CollisionComponent::CollisionSide::SIDE_TOP)
  {
    float y_pos = col_bounding_box[1] - object_sprite->height();
    object_sprite->yPos(y_pos);
  }

  else if (side == CollisionComponent::CollisionSide::SIDE_BOTTOM)
  {
    float y_pos = col_bounding_box[1] + col_bounding_box[3];
    object_sprite->yPos(y_pos);
  }
}

bool Map::moveNorth()
{
  if (getCurrentRoom()->getNorth() && getCurrentRoom()->canMove())
  {
    current_room -= map_size;
    updateMiniMap();
    return true;
  }

  return false;
}

bool Map::moveEast()
{
  if (getCurrentRoom()->getEast() && getCurrentRoom()->canMove())
  {
    current_room += 1;
    updateMiniMap();
    return true;
  }

  return false;
}

bool Map::moveSouth()
{
  if (getCurrentRoom()->getSouth() && getCurrentRoom()->canMove())
  {
    current_room += map_size;
    updateMiniMap();
    return true;
  }

  return false;
}

bool Map::moveWest()
{
  if (getCurrentRoom()->getWest() && getCurrentRoom()->canMove())
  {
    current_room -= 1;
    updateMiniMap();
    return true;
  }

  return false;
}

Room* Map::getRoom(int id)
{
  int x_index = id % map_size;
  return &rooms[x_index][id - (x_index * map_size)];
}

Room* Map::getCurrentRoom()
{
  return getRoom(current_room);
}

void Map::renderCurrentRoom(ASGE::Renderer* renderer)
{
  renderer->renderSprite(*getCurrentRoom()->spriteComponent()->getSprite());
  getCurrentRoom()->renderObjectsInRoom(renderer);
}

void Map::updateCurrentRoom(ASGE::Renderer* renderer,
                            double delta_time,
                            Player* player)
{
  getCurrentRoom()->updateObjectsInRoom(renderer, delta_time, player);

  if (getCurrentRoom()->getEnemies(false).empty())
  {
    getCurrentRoom()->canMove(true);
  }
}

void Map::renderMiniMap(ASGE::Renderer* renderer)
{
  for (int i = 0; i < mini_map.size(); i++)
  {
    renderer->renderSprite(*mini_map[i]->spriteComponent()->getSprite());
  }
}

void Map::generateStartingRoom(ASGE::Renderer* renderer)
{
  // Set All Rooms To Empty
  current_room = STARTING_ROOM;
  for (int i = 0; i < map_size; i++)
  {
    for (int j = 0; j < map_size; j++)
    {
      rooms[i][j] = Room();
    }
  }

  // Generate Starting Room
  std::string file = "data/Rooms/NESW.png";
  rooms[map_size / 2][map_size / 2] =
    Room(STARTING_ROOM, Room::NORMAL, true, true, true, true);
  rooms[map_size / 2][map_size / 2].setup(renderer, &file);
  rooms[map_size / 2][map_size / 2].canMove(true);
}

void Map::generateNewRoom(ASGE::Renderer* renderer, int x_index, int y_index)
{
  // Calculate Required Doors (NESW)
  std::string doors_needed =
    needNorthDoor(x_index, y_index) + needEastDoor(x_index, y_index) +
    needSouthDoor(x_index, y_index) + needWestDoor(x_index, y_index);

  // Randomly Select Room with Correct Doors
  // Directory: data/Rooms/NESW.png
  // _ = No Door
  std::vector<std::string> possible_rooms;

  // For Each File
  std::vector<std::string> files = ASGE::FILEIO::enumerateFiles("data/"
                                                                "Rooms");
  for (auto name : files)
  {
    // Check If File is Valid
    if (checkRoomName(name, doors_needed))
    {
      // Add File to Possible Rooms
      possible_rooms.push_back(name);
    }
  }

  // Generate Random Room
  int index = static_cast<int>(rand() % possible_rooms.size());

  // Setup Room
  std::string file = "data/Rooms/" + possible_rooms[index];
  rooms[x_index][y_index] = Room(x_index * map_size + y_index,
                                 Room::NORMAL,
                                 possible_rooms[index][0] == 'N',
                                 possible_rooms[index][1] == 'E',
                                 possible_rooms[index][2] == 'S',
                                 possible_rooms[index][3] == 'W');
  rooms[x_index][y_index].setup(renderer, &file);
}

void Map::generateRooms(ASGE::Renderer* renderer,
                        int game_width,
                        int game_height)
{
  generateStartingRoom(renderer);

  // Create Queue of Rooms From Open Doors
  std::queue<std::array<int, 2>> rooms_to_generate;
  rooms_to_generate.push({ 2, 1 });
  rooms_to_generate.push({ 1, 2 });
  rooms_to_generate.push({ 2, 3 });
  rooms_to_generate.push({ 3, 2 });

  Room* last_room = &rooms[map_size / 2][map_size / 2];

  // For Each Room in the Queue
  while (!rooms_to_generate.empty())
  {
    int x_index = rooms_to_generate.front()[0];
    int y_index = rooms_to_generate.front()[1];

    if (rooms[x_index][y_index].getId() == -1)
    {
      generateNewRoom(renderer, x_index, y_index);
      last_room = &rooms[x_index][y_index];

      // Add New Rooms To Generate To Queue
      if (rooms[x_index][y_index].getNorth() &&
          rooms[x_index - 1][y_index].getId() == -1)
      {
        rooms_to_generate.push({ x_index - 1, y_index });
      }
      if (rooms[x_index][y_index].getEast() &&
          rooms[x_index][y_index + 1].getId() == -1)
      {
        rooms_to_generate.push({ x_index, y_index + 1 });
      }
      if (rooms[x_index][y_index].getSouth() &&
          rooms[x_index + 1][y_index].getId() == -1)
      {
        rooms_to_generate.push({ x_index + 1, y_index });
      }
      if (rooms[x_index][y_index].getWest() &&
          rooms[x_index][y_index - 1].getId() == -1)
      {
        rooms_to_generate.push({ x_index, y_index - 1 });
      }

      // Remove This Room From Queue
      rooms_to_generate.pop();
    }
    else
    {
      // Remove This Room From Queue
      rooms_to_generate.pop();
    }
  }

  last_room->setType(Room::EXIT);
  last_room->canMove(false);
  generateItemRooms(renderer, game_width / 2, game_height / 2);
  generateEnemies(renderer, game_width, game_height);

  setupMinimap(renderer, game_width, game_height);
}

void Map::setupMinimap(ASGE::Renderer* renderer,
                       int game_width,
                       int game_height)
{
  mini_map.clear();
  mini_map_ids.clear();

  int count = 0;
  // For Each Valid Room, Setup Mini Map Room With a Sprite Component
  for (int i = 0; i < map_size * map_size; i++)
  {
    if (getRoom(i)->getId() != -1)
    {
      std::string file = "data/MiniMap/";
      file += getRoom(i)->getNorth() ? "N" : "_";
      file += getRoom(i)->getEast() ? "E" : "_";
      file += getRoom(i)->getSouth() ? "S" : "_";
      file += getRoom(i)->getWest() ? "W" : "_";
      file += ".png";

      GameObject* new_room = new GameObject();
      mini_map.push_back(new_room);
      mini_map_ids.push_back(getRoom(i)->getId());

      if (mini_map.at(count)->addSpriteComponent(renderer, file))
      {
        int column = i % map_size;
        int row = (i - column) / map_size;

        mini_map.at(count)->spriteComponent()->getSprite()->xPos(
          game_width - (20 * map_size) + (column * 20));
        mini_map.at(count)->spriteComponent()->getSprite()->yPos(
          game_height - (20 * map_size) + (row * 20));
        count += 1;
      }
    }
  }

  updateMiniMap();
}

void Map::setupBoundingBox(CollisionComponent* component,
                           float x_pos,
                           float y_pos,
                           float width,
                           float height)
{
  float bounding_box[4] = { x_pos, y_pos, width, height };
  component->updateBoundingBox(bounding_box);
}

void Map::checkNorthDoorCollision(Player* player)
{
  CollisionComponent* player_collider = player->collisionComponent();
  ASGE::Sprite* player_sprite = player->spriteComponent()->getSprite();

  if (player_collider->hasCollided(*room_door_collision[0]))
  {
    if (getCurrentRoom()->getNorth())
    {
      if (player_sprite->yPos() < 10)
      {
        if (moveNorth())
        {
          player_sprite->yPos(384);
        }
        else
        {
          player_sprite->yPos(10);
        }
      }
    }
    else
    {
      CollisionComponent::CollisionSide side =
        player_collider->getCollisionSide(*room_door_collision[0]);
      fixCollision(player, room_door_collision[0], side);
    }
  }
}

void Map::checkEastDoorCollision(Player* player)
{
  CollisionComponent* player_collider = player->collisionComponent();
  ASGE::Sprite* player_sprite = player->spriteComponent()->getSprite();

  if (player_collider->hasCollided(*room_door_collision[1]))
  {
    if (getCurrentRoom()->getEast())
    {
      if (player_sprite->xPos() > 694 - player_sprite->width())
      {
        if (moveEast())
        {
          player_sprite->xPos(15);
        }
        else
        {
          player_sprite->xPos(694 - player_sprite->width());
        }
      }
    }
    else
    {
      CollisionComponent::CollisionSide side =
        player_collider->getCollisionSide(*room_door_collision[1]);
      fixCollision(player, room_door_collision[1], side);
    }
  }
}

void Map::checkSouthDoorCollision(Player* player)
{
  CollisionComponent* player_collider = player->collisionComponent();
  ASGE::Sprite* player_sprite = player->spriteComponent()->getSprite();

  if (player_collider->hasCollided(*room_door_collision[2]))
  {
    if (getCurrentRoom()->getSouth())
    {
      if (player_sprite->yPos() > 438 - player_sprite->height())
      {
        if (moveSouth())
        {
          player_sprite->yPos(15);
        }
        else
        {
          player_sprite->yPos(438 - player_sprite->height());
        }
      }
    }
    else
    {
      CollisionComponent::CollisionSide side =
        player_collider->getCollisionSide(*room_door_collision[2]);
      fixCollision(player, room_door_collision[2], side);
    }
  }
}

void Map::checkWestDoorCollision(Player* player)
{
  CollisionComponent* player_collider = player->collisionComponent();
  ASGE::Sprite* player_sprite = player->spriteComponent()->getSprite();

  if (player_collider->hasCollided(*room_door_collision[3]))
  {
    if (getCurrentRoom()->getWest())
    {
      if (player_sprite->xPos() < 10)
      {
        if (moveWest())
        {
          player_sprite->xPos(640);
        }
        else
        {
          player_sprite->xPos(10);
        }
      }
    }
    else
    {
      CollisionComponent::CollisionSide side =
        player_collider->getCollisionSide(*room_door_collision[3]);
      fixCollision(player, room_door_collision[3], side);
    }
  }
}

void Map::generateItemRooms(ASGE::Renderer* renderer,
                            int game_width,
                            int game_height)
{
  int item_room_num = rand() % 3 + 2;

  for (int i = 0; i < item_room_num; i++)
  {
    int id = STARTING_ROOM;
    while (id == STARTING_ROOM || getRoom(id)->getId() == -1 ||
           getRoom(id)->getType() != Room::NORMAL)
    {
      id = rand() % (map_size * map_size);
    }

    getRoom(id)->setType(Room::ITEM);
    getRoom(id)->addItemToRoom(renderer, game_width / 2, game_height / 2);
  }
}

void Map::generateEnemies(ASGE::Renderer* renderer,
                          int game_width,
                          int game_height)
{
  for (int i = 0; i < map_size * map_size; i++)
  {
    if (getRoom(i)->getId() != -1 && getRoom(i)->getId() != STARTING_ROOM &&
        getRoom(i)->getType() == Room::NORMAL)
    {
      // Generate Demons
      int demon_num = rand() % 2 + 1;
      for (int x = 0; x < demon_num; x++)
      {
        float rand_x = rand() % (game_width - 204) + 102;
        float rand_y = rand() % (game_height - 236) + 118;

        getRoom(i)->addDemonToRoom(renderer, rand_x, rand_y);
      }

      // Generate Ghosts
      int ghost_num = rand() % 2;
      for (int x = 0; x < ghost_num; x++)
      {
        float rand_x = rand() % (game_width - 204) + 102;
        float rand_y = rand() % (game_height - 236) + 118;

        getRoom(i)->addGhostToRoom(renderer, rand_x, rand_y);
      }
    }
  }
}

void Map::updateMiniMap()
{
  for (int i = 0; i < mini_map.size(); i++)
  {
    // Update The Current Room To Red and The Rest to Black
    if (mini_map_ids.at(i) == current_room)
    {
      mini_map.at(i)->spriteComponent()->getSprite()->colour(
        ASGE::COLOURS::RED);
    }
    else if (getRoom(mini_map_ids.at(i))->getType() == Room::EXIT)
    {
      mini_map.at(i)->spriteComponent()->getSprite()->colour(
        ASGE::COLOURS::BLUE);
    }
    else if (getRoom(mini_map_ids.at(i))->getType() == Room::ITEM)
    {
      mini_map.at(i)->spriteComponent()->getSprite()->colour(
        ASGE::COLOURS::GREEN);
    }
    else
    {
      mini_map.at(i)->spriteComponent()->getSprite()->colour(
        ASGE::COLOURS::BLACK);
    }
  }
}

std::string Map::needNorthDoor(int x_pos, int y_pos)
{
  if (x_pos - 1 >= 0)
  {
    if (rooms[x_pos - 1][y_pos].getId() == -1)
    {
      return "*";
    }

    if (rooms[x_pos - 1][y_pos].getSouth())
    {
      return "N";
    }

    return "_";
  }

  return "_";
}

std::string Map::needEastDoor(int x_pos, int y_pos)
{
  if (y_pos + 1 < map_size)
  {
    if (rooms[x_pos][y_pos + 1].getId() == -1)
    {
      return "*";
    }

    if (rooms[x_pos][y_pos + 1].getWest())
    {
      return "E";
    }

    return "_";
  }

  return "_";
}

std::string Map::needSouthDoor(int x_pos, int y_pos)
{
  if (x_pos + 1 < map_size)
  {
    if (rooms[x_pos + 1][y_pos].getId() == -1)
    {
      return "*";
    }

    if (rooms[x_pos + 1][y_pos].getNorth())
    {
      return "S";
    }

    return "_";
  }

  return "_";
}

std::string Map::needWestDoor(int x_pos, int y_pos)
{
  if (y_pos - 1 >= 0)
  {
    if (rooms[x_pos][y_pos - 1].getId() == -1)
    {
      return "*";
    }

    if (rooms[x_pos][y_pos - 1].getEast())
    {
      return "W";
    }

    return "_";
  }

  return "_";
}

bool Map::checkRoomName(std::string name, std::string required_doors)
{
  bool valid = true;
  for (int i = 0; i < 4; i++)
  {
    if (required_doors[i] != '*' && name[i] != required_doors[i])
    {
      valid = false;
    }
  }

  return valid;
}

std::vector<GameObject*> Map::getEnemies(bool include_objects = false)
{
  return getCurrentRoom()->getEnemies(include_objects);
}
