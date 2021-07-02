#include "pch.h"
#include "player.hpp"

constexpr float run_speed  = 20;
constexpr float turn_speed = 160;
constexpr float gravity    = -50;
constexpr float jump_power = 30;

void Player::move(/*Terrain const& terrain*/)
{
    check_inputs( );
    // float dt = DisplayManager::getFrameTime().asSeconds();
    /*increase_rotation (glm::vec3 (0, current_turn_speed * DisplayManager::getFrameTime( ).asSeconds( ), 0));
    const auto distance = current_speed * DisplayManager::getFrameTime( ).asSeconds( );
    const auto dx       = distance * std::sin (to_radians (getRotation( ).y));
    const auto dz       = distance * std::cos (to_radians (getRotation( ).y));
    increase_position (glm::vec3 (dx, 0, dz));

    upwards_speed += gravity * DisplayManager::getFrameTime( ).asSeconds( );
    increase_position (glm::vec3 (0, upwards_speed * DisplayManager::getFrameTime( ).asSeconds( ), 0));
    float terrain_height = terrain.getHeightOfTerrain (getPosition( ).x, getPosition( ).z);

    if (position_.y < terrain_height) {
        upwards_speed = 0;
        is_in_air     = false;
        position_.y   = terrain_height;
    } else if (position_.y > terrain_height) {
        is_in_air = true;
    }

    current_speed      = 0;
    current_turn_speed = 0;*/
    // std::cout << "player x:" << position_.x << " player y:" << position_.y << " player z:" << position_.z <<
    // std::endl;
}
/*
void Player::check_inputs( ) {
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Key::W)) {
        current_speed = run_speed;
    } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Key::S)) {
        current_speed = -run_speed;
    } else {
        // current_speed = 0;
    }

    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Key::D)) {
        current_turn_speed = -turn_speed;
    } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Key::A)) {
        current_turn_speed = turn_speed;
    } else {
        // current_turn_speed = 0;
    }

    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Key::Space)) { jump( ); }
}*/

void Player::check_inputs( )
{
    if (KeyCodes::Instance( ).keyBoard.w) { current_speed = run_speed; }
    else if (KeyCodes::Instance( ).keyBoard.s)
    {
        current_speed = -run_speed;
    }
    else
    {
        // current_speed = 0;
    }

    if (KeyCodes::Instance( ).keyBoard.d) { current_turn_speed = -turn_speed; }
    else if (KeyCodes::Instance( ).keyBoard.a)
    {
        current_turn_speed = turn_speed;
    }
    else
    {
        // current_turn_speed = 0;
    }

    if (KeyCodes::Instance( ).keyBoard.space) { jump( ); }
}

void Player::jump( )
{
    if (!is_in_air) { upwards_speed += jump_power; }
}

void Player::turn_left( ) { current_turn_speed = -turn_speed; }
void Player::turn_right( ) { current_turn_speed = turn_speed; }
void Player::move_forward( ) { current_speed = run_speed; }
void Player::move_back( ) { current_speed = -run_speed; }
