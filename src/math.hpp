#ifndef MATH_HPP
#define MATH_HPP

#include "camera.hpp"
#include "pch.h"

class Camera;

namespace Math
{
    constexpr float floatPI = static_cast<float> (M_PI);

    inline constexpr double toDegrees (const double& radians) { return radians * (180.0 / M_PI); }
    inline constexpr float  toDegrees (const float& radians) { return radians * (180.0F / floatPI); }
    inline constexpr double toRadians (const double& degrees) { return degrees * (M_PI / 180.0); }
    inline constexpr float  toRadians (const float& degrees) { return degrees * (floatPI / 180.0F); }

    glm::mat4 createTransformationMatrix (glm::vec2 const& translation, glm::vec2 const& scale);

    glm::mat4 createTransformationMatrix (glm::vec3 const& translation, glm::vec3 const& rotation,
                                          glm::vec3 const& scale);

    float barryCentric (glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec2 const& pos);

    glm::mat4 createViewMatrix (const Camera& camera);

    float determinant (glm::mat4 src);

    float determinant3x3 (float t00, float t01, float t02, float t10, float t11, float t12, float t20, float t21,
                          float t22);

    glm::mat4 invert (glm::mat4 src);

    // glm::vec3 cross Math::cross(glm::vec3 const& vec1, glm::vec3 const& vec2);

    glm::vec4 transform (glm::mat4 left, glm::vec4 right);
    // glm::mat4 setIdentity(glm::mat4 m);

}  // namespace Math
#endif /* MATH_HPP */