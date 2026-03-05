#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_m.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "header/object.hpp"
class Model {
public:
    void Draw(Shader &shader); // Bouclera sur m_meshes pour appeler m_meshes[i].Draw(shader)

private:
    std::vector<Objet3D> m_meshes; // Ou std::vector<Mesh> selon votre nommage
    std::string directory;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Objet3D processMesh(aiMesh *mesh, const aiScene *scene);


};