#pragma once

#include <learnopengl/shader_m.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <iostream>

#include "header/object.hpp"

struct TextureLoaded {
    unsigned int id;
    std::string path;
};

class Model {
public:

    std::vector<TextureLoaded> m_textures_loaded;
    Model(const std::string& path);
    void Draw(Shader &shader); 

private:
    std::vector<std::unique_ptr<Objet3D>> m_meshes;
    std::string directory;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform);
    std::unique_ptr<Objet3D> processMesh(aiMesh *mesh, const aiScene *scene);
    
    unsigned int loadMaterialTexture(aiMaterial *mat, aiTextureType type, const std::string& typeName);

};