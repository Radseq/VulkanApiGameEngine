#include "AssimpModelLoader.hpp"

#include "AssimpLoader.hpp"

/**
 * Loads a 3D model from a file into Vulkan buffers
 *
 * @param device Pointer to the Vulkan device used to generated the vertex and index buffers on
 * @param filename File to load (must be a model format supported by ASSIMP)
 * @param layout Vertex layout components (position, normals, tangents, etc.)
 * @param createInfo MeshCreateInfo structure for load time settings like scale, center, etc.
 * @param copyQueue Queue used for the memory staging copy commands (must support transfer)
 */
bool AssimpModelLoader::loadFromFile (Model& model, const GraphicCore::VertexLayout& layout,
                                      const std::string& filename, ModelCreateInfo* createInfo) {
    loader.loadFromFile (filename);

    const auto *pScene = loader.GetLoaded( );

    model.parts.clear( );
    model.parts.resize (pScene->mNumMeshes);

    glm::vec3 scale (1.0F);
    glm::vec2 uvscale (1.0F);
    glm::vec3 center (0.0F);
    if (createInfo != nullptr) {
        scale   = createInfo->getScale( );
        uvscale = createInfo->getUvScale( );
        center  = createInfo->getCenter( );
    }

    std::vector<float>    vertexBuffer;
    std::vector<uint32_t> indexBuffer;

    model.vertexCount = 0;
    model.indexCount  = 0;

    // Load meshes
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
        const aiMesh* paiMesh = pScene->mMeshes [i];

        model.parts [i]            = { };
        model.parts [i].vertexBase = model.vertexCount;
        model.parts [i].indexBase  = model.indexCount;

        model.vertexCount += paiMesh->mNumVertices;

        aiColor3D pColor (0.F, 0.F, 0.F);
        pScene->mMaterials [paiMesh->mMaterialIndex]->Get (AI_MATKEY_COLOR_DIFFUSE, pColor);

        const aiVector3D Zero3D (0.0F, 0.0F, 0.0F);

        for (unsigned int j = 0; j < paiMesh->mNumVertices; j++) {
            const aiVector3D* pPos      = &(paiMesh->mVertices [j]);
            const aiVector3D* pNormal   = &(paiMesh->mNormals [j]);
            const aiVector3D* pTexCoord = (paiMesh->HasTextureCoords (0)) ? &(paiMesh->mTextureCoords [0][j]) : &Zero3D;
            const aiVector3D* pTangent  = (paiMesh->HasTangentsAndBitangents( )) ? &(paiMesh->mTangents [j]) : &Zero3D;
            const aiVector3D* pBiTangent =
                (paiMesh->HasTangentsAndBitangents( )) ? &(paiMesh->mBitangents [j]) : &Zero3D;

            glm::vec3 scaledPos {pPos->x, -pPos->y, pPos->z};
            scaledPos *= scale;
            scaledPos += center;

            for (const auto& component : layout.components) {
                switch (component) {
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION:
                        vertexBuffer.push_back (scaledPos.x);
                        vertexBuffer.push_back (scaledPos.y);
                        vertexBuffer.push_back (scaledPos.z);
                        break;
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL:
                        vertexBuffer.push_back (pNormal->x);
                        vertexBuffer.push_back (-pNormal->y);
                        vertexBuffer.push_back (pNormal->z);
                        break;
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_UV:
                        vertexBuffer.push_back (pTexCoord->x * uvscale.s);
                        vertexBuffer.push_back (pTexCoord->y * uvscale.t);
                        break;
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_COLOR:
                        vertexBuffer.push_back (pColor.r);
                        vertexBuffer.push_back (pColor.g);
                        vertexBuffer.push_back (pColor.b);
                        break;
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_TANGENT:
                        vertexBuffer.push_back (pTangent->x);
                        vertexBuffer.push_back (pTangent->y);
                        vertexBuffer.push_back (pTangent->z);
                        break;
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_BITANGENT:
                        vertexBuffer.push_back (pBiTangent->x);
                        vertexBuffer.push_back (pBiTangent->y);
                        vertexBuffer.push_back (pBiTangent->z);
                        break;
                        // Dummy components for padding
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_DUMMY_FLOAT:
                        vertexBuffer.push_back (0.0F);
                        break;
                    case GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_DUMMY_VEC4:
                        vertexBuffer.push_back (0.0F);
                        vertexBuffer.push_back (0.0F);
                        vertexBuffer.push_back (0.0F);
                        vertexBuffer.push_back (0.0F);
                        break;
                };
            }

            model.dim.max = glm::max (scaledPos, model.dim.max);
            model.dim.min = glm::min (scaledPos, model.dim.min);
        }

        model.dim.size = model.dim.max - model.dim.min;

        model.parts [i].vertexCount = paiMesh->mNumVertices;

        uint32_t indexBase = GraphicCore::util::to_uint_32_t (indexBuffer.size( ));
        for (unsigned int j = 0; j < paiMesh->mNumFaces; j++) {
            const aiFace& Face = paiMesh->mFaces [j];
            if (Face.mNumIndices != 3) continue;
            indexBuffer.push_back (indexBase + Face.mIndices [0]);
            indexBuffer.push_back (indexBase + Face.mIndices [1]);
            indexBuffer.push_back (indexBase + Face.mIndices [2]);
            model.parts [i].indexCount += 3;
            model.indexCount += 3;
        }
    }

    GraphicCore::CoreBufferManager vertexBufferManager {model.GetDevice( )};

    vertexBufferManager.stageToDeviceBuffer (model.vertexCoreBuffer, vk::BufferUsageFlagBits::eVertexBuffer,
                                             vertexBuffer);
    vertexBufferManager.stageToDeviceBuffer (model.indexCoreBuffer, vk::BufferUsageFlagBits::eIndexBuffer, 
                                             indexBuffer);

    return true;
}