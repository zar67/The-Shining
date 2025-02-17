//
// Created by Owner on 17/12/2019.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include <Engine/InputEvents.h>
#include <vector>

#include "../Components/AudioManager.h"
#include "../Components/ShootingComponent.h"
#include "GameObject.h"

class Player : public GameObject
{
 public:
  Player() = default;
  ~Player();
  void init(ASGE::Renderer* renderer,
            std::string& tex_directory,
            float x_pos,
            float y_pos);
  void reset(float game_width, float game_height);
  bool update(AudioManager* audio,
              double delta_time,
              std::vector<GameObject*> enemies);
  void takeDamage(int hit_damage); // maybe pass enemie as different demons do
                                   // different damage

  void moveVertical(float move);
  void moveHorizontal(float move);
  void setMovementVec(const float vec[2]);

  bool addWeaponComponent(ASGE::Renderer* renderer, bool use_arrow = false);
  ShootingComponent* weaponComponent();

  bool addDamagePowerup();
  bool addHealthPowerup();
  bool addMoveSpeedPowerup();
  bool addShotSizePowerup();
  bool addShotSpeedPowerup();
  bool* getPowerups();

  void addCoins(int amount);
  int getCoins();
  void addHealth(int amount);
  int getHealth();

 private:
  bool addPowerup(int index);

  const int starting_health = 100;
  const float starting_speed = 250;
  const int starting_damage = 20;
  const float starting_shot_speed = 400;
  const float starting_shot_size = 8;

  int coins = 0;
  int health = starting_health;
  float speed = starting_speed;
  int damage = starting_damage;
  float input_vector[2] = { 0.0f, 0.0f };
  float vector_movement[2] = { 0.0f, 0.0f };

  bool powerups[5] = { false, false, false, false, false };
  ShootingComponent* weapon_component = nullptr;

  const int damage_powerup_index = 0;
  const int health_powerup_index = 1;
  const int move_speed_powerup_index = 2;
  const int shot_size_powerup_index = 3;
  const int shot_speed_powerup_index = 4;
};

#endif // PROJECT_PLAYER_H
