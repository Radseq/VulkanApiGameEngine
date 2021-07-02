#include "pch.h"
#include "Frustum.hpp"

namespace GraphicCore
{
    void Frustum::update (const glm::mat4& matrix)
    {
        planes [LEFT].x = matrix [0].w + matrix [0].x;
        planes [LEFT].y = matrix [1].w + matrix [1].x;
        planes [LEFT].z = matrix [2].w + matrix [2].x;
        planes [LEFT].w = matrix [3].w + matrix [3].x;

        planes [RIGHT].x = matrix [0].w - matrix [0].x;
        planes [RIGHT].y = matrix [1].w - matrix [1].x;
        planes [RIGHT].z = matrix [2].w - matrix [2].x;
        planes [RIGHT].w = matrix [3].w - matrix [3].x;

        planes [TOP].x = matrix [0].w - matrix [0].y;
        planes [TOP].y = matrix [1].w - matrix [1].y;
        planes [TOP].z = matrix [2].w - matrix [2].y;
        planes [TOP].w = matrix [3].w - matrix [3].y;

        planes [BOTTOM].x = matrix [0].w + matrix [0].y;
        planes [BOTTOM].y = matrix [1].w + matrix [1].y;
        planes [BOTTOM].z = matrix [2].w + matrix [2].y;
        planes [BOTTOM].w = matrix [3].w + matrix [3].y;

        planes [BACK].x = matrix [0].w + matrix [0].z;
        planes [BACK].y = matrix [1].w + matrix [1].z;
        planes [BACK].z = matrix [2].w + matrix [2].z;
        planes [BACK].w = matrix [3].w + matrix [3].z;

        planes [FRONT].x = matrix [0].w - matrix [0].z;
        planes [FRONT].y = matrix [1].w - matrix [1].z;
        planes [FRONT].z = matrix [2].w - matrix [2].z;
        planes [FRONT].w = matrix [3].w - matrix [3].z;

        for (int i = 0; i < planesSize; ++i)
        {
            const float length =
                sqrtf (planes [i].x * planes [i].x + planes [i].y * planes [i].y + planes [i].z * planes [i].z);
            planes [i] /= length;
            // planes[i] /= glm::normalize(planes[i]);
        }
    }

    bool Frustum::checkSphere (const glm::vec3& pos, const float& radius) const
    {
        for (const auto& plane : planes)
        {
            if ((plane.x * pos.x) + (plane.y * pos.y) + (plane.z * pos.z) + plane.w <= -radius) { return false; }
        }

        /* for (int i = 0; i < planesSize; ++i) {
             if ((planes [i].x * pos.x) + (planes [i].y * pos.y) + (planes [i].z * pos.z) + planes [i].w <= -radius) {
                 return false;
             }

             // Plane-sphere intersection test. If p*n + d + r < 0 then we're outside the plane.
             // const double result = glm::dot(glm::vec3(planes[i]), pos) + planes[i].w;

             // if (result <= -radius)
             // return false;
         }*/

        // If none of the planes had the entity lying on its "negative" side then it must be
        // on the "positive" side for all of them. Thus the entity is inside or touching the frustum.
        return true;
    }
}  // namespace GraphicCore
