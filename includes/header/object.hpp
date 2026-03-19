#pragma once

// c++
#include <vector>
#include <memory>
#include <glm/glm.hpp>

//learnopengl
#include <learnopengl/shader.h>

// my classes
#include "header/vao.hpp"
#include "header/vbo.hpp"
#include "header/ebo.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::ivec2 BoneIDs; 
    glm::vec2 Weights;

    // tests
    // glm::vec3 Tangent;
    // glm::vec3 Bitangent;
};

struct PBRMaterial {
    unsigned int albedoMap = 0;
    unsigned int normalMap = 0;
    unsigned int metallicMap = 0;
    unsigned int roughnessMap = 0;
    unsigned int aoMap = 0;
};

class Objet3D
{
    protected:

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        std::unique_ptr<VAO> m_vao;
        std::unique_ptr<VBO> m_vbo;
        std::unique_ptr<EBO> m_ebo;

        glm::mat4 m_modelMatrix;

        GLenum m_drawMode = GL_TRIANGLES;

        void setupMesh()
        {
            m_vao = std::make_unique<VAO>();
            
            size_t sizeOfVertices = m_vertices.size() * sizeof(Vertex);
            size_t sizeOfIndices = m_indices.size() * sizeof(unsigned int);

            m_vbo = std::make_unique<VBO>(m_vertices.data(), sizeOfVertices);
            m_ebo = std::make_unique<EBO>(m_indices.data(), sizeOfIndices);

            m_vao->bind();
            m_ebo->bind();

            m_vao->linkAttrib(*m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
            m_vao->linkAttrib(*m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            m_vao->linkAttrib(*m_vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

            m_vao->linkAttribInt(*m_vbo, 3, 2, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));
            m_vao->linkAttrib(*m_vbo, 4, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Weights));
            
            m_vao->unbind();
            m_vbo->unbind();
            m_ebo->unbind();
        }


    public:
        PBRMaterial m_material;

        Objet3D() : m_modelMatrix(1.0f) {}
        
        virtual ~Objet3D() = default;

        virtual void Draw(Shader& shader) const {
            shader.setMat4("model", m_modelMatrix);

            // Binding de l'Albedo
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_material.albedoMap);
            shader.setInt("albedoMap", 0);
            

            // Binding de la Normal Map
            
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_material.normalMap);
            shader.setInt("normalMap", 1);
            

            //Binding de la carte Metallic/Roughness glTF
            
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_material.metallicMap);
            shader.setInt("metallicMap", 2);
            
            
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_material.roughnessMap);
            shader.setInt("roughnessMap", 3);
            
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, m_material.aoMap);
            shader.setInt("aoMap", 4);
            
            m_vao->bind();
            if (m_indices.empty()) {
                glDrawArrays(m_drawMode, 0, static_cast<unsigned int>(m_vertices.size()));
            } else {
                glDrawElements(m_drawMode, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
            }            
            m_vao->unbind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        virtual void Draw(Shader& shader, glm::mat4 parentMatrix) const {
            
            glm::mat4 finalMatrix = parentMatrix * m_modelMatrix;
            shader.setMat4("model", finalMatrix);

            // Binding de l'Albedo
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_material.albedoMap);
            shader.setInt("albedoMap", 0);
            

            // Binding de la Normal Map
            
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_material.normalMap);
            shader.setInt("normalMap", 1);
            

            //Binding de la carte Metallic/Roughness glTF
            
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_material.metallicMap);
            shader.setInt("metallicMap", 2);
            
            
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_material.roughnessMap);
            shader.setInt("roughnessMap", 3);
            
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, m_material.aoMap);
            shader.setInt("aoMap", 4);
            
            m_vao->bind();
            if (m_indices.empty()) {
                glDrawArrays(m_drawMode, 0, static_cast<unsigned int>(m_vertices.size()));
            } else {
                glDrawElements(m_drawMode, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
            }            
            m_vao->unbind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void setModelMatrix(glm::mat4 modelMatrix)
        {
            m_modelMatrix = modelMatrix;
        }

        void initFromAssimp(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
        {
            m_vertices = vertices;
            m_indices = indices;
            setupMesh(); 
        }
};