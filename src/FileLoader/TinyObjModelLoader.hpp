#ifndef TINY_OBJ_MODEL_LOADER_HPP
#define TINY_OBJ_MODEL_LOADER_HPP

#include <Buffer\CoreBuffeManager.hpp>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "../Model/Model.hpp"
#include "../Model/ModelCreateInfo.hpp"
#include "IModelLoad.hpp"

// MUST CORRECT PIPELINE appendVertexLayout ORDER
struct Vertex
{
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;

    bool operator== (const Vertex& other) const
    {
        return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
    }
};

namespace std
{
    template <> struct hash<Vertex>
    {
        size_t operator( ) (Vertex const& vertex) const
        {
            return ((hash<glm::vec3>( ) (vertex.pos) ^ (hash<glm::vec3>( ) (vertex.normal) << 1)) >> 1) ^
                   (hash<glm::vec2>( ) (vertex.texCoord) << 1);
        }
    };
}  // namespace std

class TinyObjModelLoader : public IModelLoad
{
    const GraphicCore::VulkanDevice& context;

    void createVertexBuffer (Model* const model, const std::vector<Vertex>& vertices);
    void createIndexBuffer (Model* const model, const std::vector<uint32_t>& indices);

    GraphicCore::CoreBufferManager m_BufferManager {context};

   public:
    explicit TinyObjModelLoader (const GraphicCore::VulkanDevice& Context);

    void loadFromFile (Model* const model, const std::string_view& filename)
    {
        ModelCreateInfo modelCreateInfo {1.0F, 1.0F, 0.0F};
        loadFromFile (model, filename, &modelCreateInfo);
    }
    void loadFromFile (Model* const model, const std::string_view& filename, const float& scale)
    {
        ModelCreateInfo modelCreateInfo {scale, 1.0F, 0.0F};
        loadFromFile (model, filename, &modelCreateInfo);
    }

    void loadFromFile (Model* const model, const std::string_view& filename,
                       ModelCreateInfo* const createInfo) override;
};

#endif  // TINY_OBJ_MODEL_LOADER_HPP