#ifndef ASSIMP_LOADER_HPP
#define ASSIMP_LOADER_HPP

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <stdexcept>
#include <vector>

class AssimpLoader {
    static constexpr int defaultFlags = aiProcess_FlipWindingOrder | aiProcess_Triangulate |
                                        aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace |
                                        aiProcess_GenSmoothNormals;

    Assimp::Importer importer;
    const aiScene*   pScene;

    void throwOnNoLoaded (const std::string& filename);

   public:
    void loadFromFile (const std::string& filename, int flags = defaultFlags);

    const aiScene* GetLoaded( );
    //assimp automatic delete scene
    //~AssimpLoader( ) { importer.FreeScene( ); };
};

#endif  // ASSIMP_LOADER_HPP