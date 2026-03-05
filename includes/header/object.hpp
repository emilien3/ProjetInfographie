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

    // Pour le pbr (gltf) :
    // glm::vec3 Tangent;
    // glm::vec3 Bitangent;
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
            
            m_vao->unbind();
            m_vbo->unbind();
            m_ebo->unbind();
        }


    public:

        Objet3D() : m_modelMatrix(1.0f) {}
        
        virtual ~Objet3D() = default;

        virtual void Draw(Shader& shader) const {
            shader.setMat4("model", m_modelMatrix);
            
            m_vao->bind();
            if (m_indices.empty()) {
                glDrawArrays(m_drawMode, 0, static_cast<unsigned int>(m_vertices.size()));
            } else {
                glDrawElements(m_drawMode, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
            }            
            m_vao->unbind();
        }

        void setModelMatrix(glm::mat4 modelMatrix)
        {
            m_modelMatrix = modelMatrix;
        }
};