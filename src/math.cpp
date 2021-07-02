#include "pch.h"
#include "math.hpp"

glm::mat4 Math::createTransformationMatrix (glm::vec2 const& translation, glm::vec2 const& scale)
{
    glm::mat4 matrix = glm::mat4( );

    matrix = glm::translate (matrix, glm::vec3 (translation, 0.0F));
    matrix = glm::scale (matrix, glm::vec3 (scale, 1.0F));

    return matrix;
}

glm::mat4 Math::createTransformationMatrix (glm::vec3 const& translation, glm::vec3 const& rotation,
                                            glm::vec3 const& scale)
{
    glm::mat4 matrix = glm::mat4( );

    matrix = glm::translate (matrix, translation);
    matrix = glm::rotate (matrix, rotation.x, glm::vec3 (1.0F, 0.0F, 0.0F));
    matrix = glm::rotate (matrix, rotation.y, glm::vec3 (0.0F, 1.0F, 0.0F));
    matrix = glm::rotate (matrix, rotation.z, glm::vec3 (0.0F, 0.0F, 1.0F));
    matrix = glm::scale (matrix, scale);

    return matrix;
}

glm::mat4 Math::createViewMatrix (const Camera& camera)
{
    glm::mat4 view_matrix { };

    view_matrix = glm::rotate (view_matrix, glm::radians (camera.getPitch( )), {1, 0, 0});
    view_matrix = glm::rotate (view_matrix, glm::radians (camera.getYaw( )), {0, 1, 0});
    view_matrix = glm::translate (view_matrix, -camera.getPosition( ));

    return view_matrix;
}

float Math::barryCentric (glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec2 const& pos)
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1  = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2  = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3  = 1.0F - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

/*
glm::vec3 cross Math::cross(glm::vec3 const& vec1, glm::vec3 const& vec2)
{
        glm::vec3 vec = glm::vec3(0, 0, 0);
        vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
        vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
        vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
        return vec;
}*/

float Math::determinant (glm::mat4 src)
{
    float result = src [0][0] * ((src [1][1] * src [2][2] * src [3][3] + src [1][2] * src [2][3] * src [3][1] +
                                  src [1][3] * src [2][1] * src [3][2]) -
                                 src [1][3] * src [2][2] * src [3][1] - src [1][1] * src [2][3] * src [3][2] -
                                 src [1][2] * src [2][1] * src [3][3]);
    result -= src [0][1] * ((src [1][0] * src [2][2] * src [3][3] + src [1][2] * src [2][3] * src [3][0] +
                             src [1][3] * src [2][0] * src [3][2]) -
                            src [1][3] * src [2][2] * src [3][0] - src [1][0] * src [2][3] * src [3][2] -
                            src [1][2] * src [2][0] * src [3][3]);
    result += src [0][2] * ((src [1][0] * src [2][1] * src [3][3] + src [1][1] * src [2][3] * src [3][0] +
                             src [1][3] * src [2][0] * src [3][1]) -
                            src [1][3] * src [2][1] * src [3][0] - src [1][0] * src [2][3] * src [3][1] -
                            src [1][1] * src [2][0] * src [3][3]);
    result -= src [0][3] * ((src [1][0] * src [2][1] * src [3][2] + src [1][1] * src [2][2] * src [3][0] +
                             src [1][2] * src [2][0] * src [3][1]) -
                            src [1][2] * src [2][1] * src [3][0] - src [1][0] * src [2][2] * src [3][1] -
                            src [1][1] * src [2][0] * src [3][2]);
    return result;
}

/**
 * Calculate the determinant of a 3x3 matrix
 * @return result
 */

float Math::determinant3x3 (float t00, float t01, float t02, float t10, float t11, float t12, float t20, float t21,
                            float t22)
{
    return t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20);
}

glm::mat4 Math::invert (glm::mat4 src)
{
    float d = determinant (src);

    glm::mat4 dest { };

    if (d != 0)
    {
        /*
         * m00 m01 m02 m03
         * m10 m11 m12 m13
         * m20 m21 m22 m23
         * m30 m31 m32 m33
         */
        // if (dest == null)
        //	dest = new Matrix4f();
        float determinant_inv = 1.F / d;

        // first row
        float t00 = determinant3x3 (src [1][1], src [1][2], src [1][3], src [2][1], src [2][2], src [2][3], src [3][1],
                                    src [3][2], src [3][3]);
        float t01 = -determinant3x3 (src [1][0], src [1][2], src [1][3], src [2][0], src [2][2], src [2][3], src [3][0],
                                     src [3][2], src [3][3]);
        float t02 = determinant3x3 (src [1][0], src [1][1], src [1][3], src [2][0], src [2][1], src [2][3], src [3][0],
                                    src [3][1], src [3][3]);
        float t03 = -determinant3x3 (src [1][0], src [1][1], src [1][2], src [2][0], src [2][1], src [2][2], src [3][0],
                                     src [3][1], src [3][2]);
        // second row
        float t10 = -determinant3x3 (src [0][1], src [0][2], src [0][3], src [2][1], src [2][2], src [2][3], src [3][1],
                                     src [3][2], src [3][3]);
        float t11 = determinant3x3 (src [0][0], src [0][2], src [0][3], src [2][0], src [2][2], src [2][3], src [3][0],
                                    src [3][2], src [3][3]);
        float t12 = -determinant3x3 (src [0][0], src [0][1], src [0][3], src [2][0], src [2][1], src [2][3], src [3][0],
                                     src [3][1], src [3][3]);
        float t13 = determinant3x3 (src [0][0], src [0][1], src [0][2], src [2][0], src [2][1], src [2][2], src [3][0],
                                    src [3][1], src [3][2]);
        // third row
        float t20 = determinant3x3 (src [0][1], src [0][2], src [0][3], src [1][1], src [1][2], src [1][3], src [3][1],
                                    src [3][2], src [3][3]);
        float t21 = -determinant3x3 (src [0][0], src [0][2], src [0][3], src [1][0], src [1][2], src [1][3], src [3][0],
                                     src [3][2], src [3][3]);
        float t22 = determinant3x3 (src [0][0], src [0][1], src [0][3], src [1][0], src [1][1], src [1][3], src [3][0],
                                    src [3][1], src [3][3]);
        float t23 = -determinant3x3 (src [0][0], src [0][1], src [0][2], src [1][0], src [1][1], src [1][2], src [3][0],
                                     src [3][1], src [3][2]);
        // fourth row
        float t30 = -determinant3x3 (src [0][1], src [0][2], src [0][3], src [1][1], src [1][2], src [1][3], src [2][1],
                                     src [2][2], src [2][3]);
        float t31 = determinant3x3 (src [0][0], src [0][2], src [0][3], src [1][0], src [1][2], src [1][3], src [2][0],
                                    src [2][2], src [2][3]);
        float t32 = -determinant3x3 (src [0][0], src [0][1], src [0][3], src [1][0], src [1][1], src [1][3], src [2][0],
                                     src [2][1], src [2][3]);
        float t33 = determinant3x3 (src [0][0], src [0][1], src [0][2], src [1][0], src [1][1], src [1][2], src [2][0],
                                    src [2][1], src [2][2]);

        // transpose and divide by the determinant
        dest [0][0] = t00 * determinant_inv;
        dest [1][1] = t11 * determinant_inv;
        dest [2][2] = t22 * determinant_inv;
        dest [3][3] = t33 * determinant_inv;
        dest [0][1] = t10 * determinant_inv;
        dest [1][0] = t01 * determinant_inv;
        dest [2][0] = t02 * determinant_inv;
        dest [0][2] = t20 * determinant_inv;
        dest [1][2] = t21 * determinant_inv;
        dest [2][1] = t12 * determinant_inv;
        dest [0][3] = t30 * determinant_inv;
        dest [3][0] = t03 * determinant_inv;
        dest [1][3] = t31 * determinant_inv;
        dest [3][1] = t13 * determinant_inv;
        dest [3][2] = t23 * determinant_inv;
        dest [2][3] = t32 * determinant_inv;
        return dest;
    }
    else
        return dest;
}

glm::vec4 Math::transform (glm::mat4 left, glm::vec4 right)
{
    glm::vec4 dest { };

    float x = left [0][0] * right.x + left [1][0] * right.y + left [2][0] * right.z + left [3][0] * right.w;
    float y = left [0][1] * right.x + left [1][1] * right.y + left [2][1] * right.z + left [3][1] * right.w;
    float z = left [0][2] * right.x + left [1][2] * right.y + left [2][2] * right.z + left [3][2] * right.w;
    float w = left [0][3] * right.x + left [1][3] * right.y + left [2][3] * right.z + left [3][3] * right.w;

    dest.x = x;
    dest.y = y;
    dest.z = z;
    dest.w = w;

    return dest;
}
/*
glm::mat4 Math::setIdentity(glm::mat4 m)
{
        m[0][0] = 1.0f;
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = 0.0f;
        m[1][0] = 0.0f;
        m[1][1] = 1.0f;
        m[1][2] = 0.0f;
        m[1][3] = 0.0f;
        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = 1.0f;
        m[2][3] = 0.0f;
        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        m[3][3] = 1.0f;

        return m;
}*/