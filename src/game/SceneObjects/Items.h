//
// Created by Ryan on 07/01/2020.
//

#ifndef THE_SHINING_GAME_GROUP_3_ITEMS_H
#define THE_SHINING_GAME_GROUP_3_ITEMS_H

#include "GameObject.h"
#include "string"
#include <Engine/OGLGame.h>

class Items : public GameObject
{
 public:
  Items() = default;
  ~Items() = default;

  bool hasCollidedWithPlayer();
  void hasPickedUpItem();

  bool setUpItems(ASGE::Renderer* renderer, float x_pos, float y_pos);
  void renderItem(ASGE::Renderer* renderer);

 private:
  bool visibility = true;
  std::string item_Name = "";
};

#endif // THE_SHINING_GAME_GROUP_3_ITEMS_H
