//
// Created by Ryan on 07/01/2020.
//

#include "Items.h"
#include "../Components/CollisionComponent.h"
#include <Engine/OGLGame.h>

bool Items::hasCollidedWithPlayer()
{
  return false;
}
void Items::hasPickedUpItem()
{
  visibility = false;
}
bool Items::setUpItems(ASGE::Renderer* renderer, float x_pos, float y_pos)
{
  // set up all the items
  addSpriteComponent(renderer, "data/Items/Item1.PNG");
  spriteComponent()->getSprite()->width(32);
  spriteComponent()->getSprite()->height(32);
  spriteComponent()->getSprite()->xPos(x_pos);
  spriteComponent()->getSprite()->yPos(y_pos);

  return true;
}
void Items::renderItem(ASGE::Renderer* renderer)
{
  renderer->renderSprite(*spriteComponent()->getSprite());
}