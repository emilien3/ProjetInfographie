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
        // C'est ici que nous allons convertir les données SoA (Struct of Arrays) d'Assimp
        // vers votre format AoS (Array of Structs) pour remplir vos VAO/VBO.
}

void Draw(Shader &shader)
{

}