#include "header/model.hpp"
#include "header/utils.hpp"
#include <iostream>

Model::Model(const std::string& path)
{
        loadModel(path);
}

void Model::loadModel(std::string const &path) 
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERREUR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void Model::processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform) 
{
    aiMatrix4x4 aiMat = node->mTransformation;
    glm::mat4 localTransform(
        aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
        aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
        aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
        aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    );
    glm::mat4 globalTransform = parentTransform * localTransform;

    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        std::unique_ptr<Objet3D> objet = processMesh(mesh, scene);
        objet->setModelMatrix(globalTransform);

        m_meshes.push_back(std::move(objet));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, globalTransform);
    }
}

std::unique_ptr<Objet3D> Model::processMesh(aiMesh *mesh, const aiScene *scene) 
{
    std::vector<Vertex> vertices ;
    std::vector<unsigned int> indices;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 pos;
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;
        vertex.Position = pos;

        if (mesh->HasNormals()) {
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.Normal = normal;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    auto nouvelObjet = std::make_unique<Objet3D>();
    nouvelObjet->initFromAssimp(vertices, indices);

    /////////////////////////////////////////////
    // --- LECTURE GÉNÉRIQUE DES MATÉRIAUX --- //
    /////////////////////////////////////////////

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Albedo (BASE_COLOR pour le glTF)
        nouvelObjet->m_material.albedoMap = loadMaterialTexture(material, aiTextureType_BASE_COLOR, "albedo");
        // Fallback si l'exportateur utilise l'ancien standard diffuse
        if (nouvelObjet->m_material.albedoMap == 0) {
            nouvelObjet->m_material.albedoMap = loadMaterialTexture(material, aiTextureType_DIFFUSE, "albedo");
        }

        // 2. Normales
        nouvelObjet->m_material.normalMap = loadMaterialTexture(material, aiTextureType_NORMALS, "normal");

        // 3. Metallic-Roughness (Souvent dans UNKNOWN pour le glTF)
        nouvelObjet->m_material.metallicMap = loadMaterialTexture(material, aiTextureType_UNKNOWN, "metallicRoughness");
        nouvelObjet->m_material.roughnessMap = loadMaterialTexture(material, aiTextureType_UNKNOWN, "metallicRoughness");

        // 4. Occlusion Ambiante (Souvent dans LIGHTMAP ou AMBIENT)
        nouvelObjet->m_material.aoMap = loadMaterialTexture(material, aiTextureType_LIGHTMAP, "ao");
        if (nouvelObjet->m_material.aoMap == 0) {
            nouvelObjet->m_material.aoMap = loadMaterialTexture(material, aiTextureType_AMBIENT, "ao");
        }
    }

    return nouvelObjet;
}

void Model::Draw(Shader &shader)
{
    for(unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i]->Draw(shader);
}

unsigned int Model::loadMaterialTexture(aiMaterial *mat, aiTextureType type, const std::string& typeName)
{
    aiString str;
    if(mat->GetTexture(type, 0, &str) != AI_SUCCESS) {
        return 0; 
    }

    bool skip = false;
    unsigned int textureID = 0;

    for(unsigned int j = 0; j < m_textures_loaded.size(); j++) {
        if(std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0) {
            textureID = m_textures_loaded[j].id;
            skip = true; 
            break;
        }
    }

    if(!skip)
    {
        TextureLoaded texture;
        std::string fullPath = this->directory + "/" + str.C_Str();
        textureID = loadTexture(fullPath.c_str());
        texture.id = textureID;
        texture.path = str.C_Str();
        m_textures_loaded.push_back(texture);
    }
    
    return textureID;
}
