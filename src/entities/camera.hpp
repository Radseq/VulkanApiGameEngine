#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "../Util/KeyCodes.hpp"
//#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../Util/math.hpp"

class Player;

namespace
{
    constexpr float EYE_LEVEL {6.0F};
    constexpr float WALK_SPEED {20.0F};
    constexpr float RUN_SPEED {100.0F};
    constexpr float SENSITIVITY {0.005F};
}  // namespace

class Camera
{
    void  calculateZoom( );
    void  calculatePitch( );
    void  calculateAngleAroundPlayer( );
    float calculateHorizontalDistance( );
    float calculateVerticalDistance( );

    // const Player& player_;

    glm::vec3 position {0, 0, 0};

    float pitch {25.0F};
    float yaw {0.0F};
    float roll {0.0F};
    float distanceFromPlayer {50.0F};
    float angleAroundPlayer {0};

    int mouseWheelDelta {0};
    int mouse_x_delta {0};
    int mouse_y_delta {0};

    void calculateCameraPosition (float horiz_distance, float vert_distance);

   public:
    // explicit Camera (Player const& player);
    Camera( ) { };

    void             setPosition (const glm::vec3& pos) { position = pos; }
    const glm::vec3& getPosition( ) const;

    float getPitch( ) const;
    float getYaw( ) const;
    float getRoll( ) const;
    // void  move( );
    // temp
    bool isMoving( );

    void update( );

    void mouseWheel (int delta);
    void mouseMove (const int& dx, const int& dy);

    void invertPitch( ) { pitch = -pitch; }
};

#endif /* CAMERA_HPP */