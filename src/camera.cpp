#include "pch.h"
#include "camera.hpp"

// void Camera::move( ) {
/*sf::Vector2i mousePositionDelta = DisplayManager::getMousePositionDelta( );
mouse_y_delta                   = mousePositionDelta.y;
mouse_x_delta                   = mousePositionDelta.x;
calculate_zoom( );
calculate_pitch( );
calculate_angle_around_player( );
float horizontal_distance = calculate_horizontal_distance( );
float vertical_distance   = calculate_vertical_distance( );
calculate_camera_position (horizontal_distance, vertical_distance);
yaw = 180 - (player_.getRotation( ).y + angle_around_player);*/
/*
if (KeyCodes::Instance( ).keyBoard.w) {
    position_.z -= 0.02F;
} else {
    // current_speed = 0;
}

if (KeyCodes::Instance( ).keyBoard.d) {
    position_.x += 0.02F;
} else if (KeyCodes::Instance( ).keyBoard.a) {
    position_.x -= 0.02F;
} else {
    // current_turn_speed = 0;
}
}*/

/*
Camera::Camera (Player const& player_)
    : player_ {player_} {
    position_ += glm::vec3 (0.0F, 6.0F, 0.0F);
}*/

float            Camera::getPitch( ) const { return pitch; }
float            Camera::getYaw( ) const { return yaw; }
float            Camera::getRoll( ) const { return roll; }
const glm::vec3& Camera::getPosition( ) const { return position; }

float Camera::calculateHorizontalDistance( ) const { return distanceFromPlayer * std::cos (Math::toRadians (pitch)); }
float Camera::calculateVerticalDistance( ) const { return distanceFromPlayer * std::sin (Math::toRadians (pitch)); }

void Camera::calculateCameraPosition (float horiz_distance, float vert_distance)
{
    /* float theta    = player_.getRotation( ).y + angle_around_player;
     float offset_x = horiz_distance * std::sin (to_radians (theta));
     float offset_z = horiz_distance * std::cos (to_radians (theta));
     position_.x    = player_.getPosition( ).x - offset_x;
     position_.y    = player_.getPosition( ).y + vert_distance;
     position_.z    = player_.getPosition( ).z - offset_z;*/
}

void Camera::calculateZoom( )
{
    float zoom_level = static_cast<float> (mouseWheelDelta);
    distanceFromPlayer -= zoom_level;
    mouseWheelDelta = 0;
}

bool Camera::isMoving( )
{
    return KeyCodes::Instance( ).keyBoard.w || KeyCodes::Instance( ).keyBoard.a || KeyCodes::Instance( ).keyBoard.s ||
           KeyCodes::Instance( ).keyBoard.d;
}

void Camera::update( )
{
    if (KeyCodes::Instance( ).keyBoard.w) { position.z -= 0.02F; }

    if (KeyCodes::Instance( ).keyBoard.s) { position.z += 0.02F; }

    if (KeyCodes::Instance( ).keyBoard.d) { position.x += 0.02F; }
    if (KeyCodes::Instance( ).keyBoard.a) { position.x -= 0.02F; }
}

void Camera::mouseWheel (int delta) { mouseWheelDelta = delta; }

void Camera::calculatePitch( )
{
    if (KeyCodes::Instance( ).mouse.right)
    {
        float pitch_change = static_cast<float> (mouse_y_delta);
        pitch += pitch_change;
        mouse_y_delta = 0;
    }
}

void Camera::calculateAngleAroundPlayer( )
{
    if (KeyCodes::Instance( ).mouse.left)
    {
        float angle_change = static_cast<float> (mouse_x_delta);
        angleAroundPlayer -= angle_change;
        mouse_x_delta = 0;
    }
}

void Camera::mouseMove (const int& dx, const int& dy)
{
    mouse_x_delta = static_cast<int> (0.3) * dx;
    mouse_y_delta = static_cast<int> (0.1) * dy;
}
