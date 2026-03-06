#include "header/model.hpp"
#include <iostream>

void Model::loadModel(std::string const &path) 
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERREUR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    // Début du parcours du graphe de scène
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) 
{
    // 1. Traiter tous les maillages du nœud courant
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        // Le nœud ne contient que des indices, on va chercher l'objet réel dans la scène
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // 2. Traiter récursivement chaque enfant
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Objet3D Model::processMesh(aiMesh *mesh, const aiScene *scene) 
{
    std::vector<Vertex> vertices ;
    
    std::vector<unsigned int> m_indices;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
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
            m_indices.push_back(face.mIndices[j]);
        }
    }

    Objet3D nouvelObjet;
    
    nouvelObjet.m_vertices = vertices;
    nouvelObjet.m_indices = m_indices;
    nouvelObjet.setupMesh(); 

    return nouvelObjet;
}

void Draw(Shader &shader)
{

}