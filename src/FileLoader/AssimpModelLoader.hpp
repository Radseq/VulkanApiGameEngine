#ifndef ASSIMP_MODEL_LOADER_HPP
#define ASSIMP_MODEL_LOADER_HPP

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdlib.h>

#include <Image\ImageManager.hpp>
#include <Model\VertexLayout.hpp>
#include <assimp/Importer.hpp>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include "../Model/Model.hpp"
#include "../Model/ModelCreateInfo.hpp"
#include "AssimpLoader.hpp"
#include "IModelLoad.hpp"
#include "vulkan/vulkan.h"
// thx to Sascha Willems - www.saschawillems.de

class AssimpModelLoader : public IModelLoad
{
   public:
    static const int defaultFlags = aiProcess_FlipWindingOrder | aiProcess_Triangulate |
                                    aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace |
                                    aiProcess_GenSmoothNormals;

    AssimpLoader loader { };

    /**
     * Loads a 3D model from a file into Vulkan buffers
     *
     * @param device Pointer to the Vulkan device used to generated the vertex and index buffers on
     * @param filename File to load (must be a model format supported by ASSIMP)
     * @param layout Vertex layout components (position, normals, tangents, etc.)
     * @param createInfo MeshCreateInfo structure for load time settings like scale, center, etc.
     * @param copyQueue Queue used for the memory staging copy commands (must support transfer)
     */
    void loadFromFile (Model* const model, const std::string_view& filename,
                       ModelCreateInfo* const createInfo) override;
    /**
     * Loads a 3D model from a file into Vulkan buffers
     *
     * @param device Pointer to the Vulkan device used to generated the vertex and index buffers on
     * @param filename File to load (must be a model format supported by ASSIMP)
     * @param layout Vertex layout components (position, normals, tangents, etc.)
     * @param scale Load time scene scale
     * @param copyQueue Queue used for the memory staging copy commands (must support transfer)
     */
    void loadFromFile (Model* const model, const std::string_view& filename, const float& scale = 1.0F)
    {
        ModelCreateInfo modelCreateInfo {scale, 1.0F, 0.0f};
        return loadFromFile (model, filename, &modelCreateInfo);
    }
};

#endif  // ASSIMP_MODEL_LOADER_HPP