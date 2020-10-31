#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

#include "camera.hpp"
#include "entity.hpp"

#include "player.hpp"

#include "../Util/KeyCodes.hpp"

class Player : public Entity {
    //using Entity::Entity;  // this must be without this give error in game engine player object thats need 1 argument 4
                           // provides

    void check_inputs( );

    float current_speed      = 0;
    float current_turn_speed = 0;
    float upwards_speed      = 0;
    bool  is_in_air          = false;

   public:
    void move (/*Terrain const& terrain*/);
    void jump( );
    void turn_left( );
    void turn_right( );
    void move_forward( );
    void move_back( );
};

#endif /* PLAYER_HPP */
