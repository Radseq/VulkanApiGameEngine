#include "TinyObjModelLoader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void TinyObjModelLoader::loadFromFile (Model* const model, const std::string& filename, ModelCreateInfo* createInfo)
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;

    std::unordered_map<Vertex, uint32_t> uniqueVertices = { };

    glm::vec3 scale (1.0F);
    glm::vec2 uvscale (1.0F);
    glm::vec3 center (0.0F);
    if (createInfo != nullptr)
    {
        scale   = createInfo->getScale( );
        uvscale = createInfo->getUvScale( );
        center  = createInfo->getCenter( );
    }

    char const* charFileName = filename.data( );

    if (!tinyobj::LoadObj (&attrib, &shapes, &materials, &warn, &err, charFileName))
    { throw std::runtime_error (warn + err); }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            glm::vec3 position = {attrib.vertices [index.vertex_index * 3],
                                  attrib.vertices [index.vertex_index * 3 + 1],
                                  attrib.vertices [index.vertex_index * 3 + 2]};

            position *= scale;
            position += center;

            glm::vec3 normal = {attrib.normals [index.normal_index * 3], attrib.normals [index.normal_index * 3 + 1],
                                attrib.normals [index.normal_index * 3 + 2]};

            glm::vec2 texCoord = {
                attrib.texcoords [index.texcoord_index * 2],
                attrib.texcoords [index.texcoord_index * 2 + 1],
            };

            Vertex vertex = {position, texCoord, normal};

            if (uniqueVertices.count (vertex) == 0)
            {
                uniqueVertices [vertex] = static_cast<uint32_t> (vertices.size( ));
                vertices.push_back (vertex);
            }
            indices.push_back (uniqueVertices [vertex]);
        }
    }

    model->indexCount  = static_cast<uint32_t> (indices.size( ));
    model->vertexCount = static_cast<uint32_t> (vertices.size( ));

    createVertexBuffer (model, vertices);
    createIndexBuffer (model, indices);
}

void TinyObjModelLoader::createVertexBuffer (Model* const model, const std::vector<Vertex>& vertices)
{
    m_BufferManager.stageToDeviceBuffer<Vertex> (model->vertexCoreBuffer, vk::BufferUsageFlagBits::eVertexBuffer,
                                                 vertices);
}

void TinyObjModelLoader::createIndexBuffer (Model* const model, const std::vector<uint32_t>& indices)
{
    m_BufferManager.stageToDeviceBuffer<uint32_t> (model->indexCoreBuffer, vk::BufferUsageFlagBits::eIndexBuffer,
                                                   indices);
}

TinyObjModelLoader::TinyObjModelLoader (const GraphicCore::VulkanDevice& Context)
    : context (Context)
{
}
