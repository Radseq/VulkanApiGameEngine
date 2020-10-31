#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include <array>
#include <glm/glm.hpp>

namespace GameCore {
    class Frustum {
        const int planesSize = 6;

        const enum side { LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3, BACK = 4, FRONT = 5 };

       public:
        void update (const glm::mat4& matrix);
        bool checkSphere (const glm::vec3& pos, const float& radius) const;

        std::array<glm::vec4, 6> planes;
    };
}  // namespace GameCore

#endif FRUSTUM_HPP
