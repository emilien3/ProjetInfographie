#include "header/cylindre.hpp"

void Cylinder::buildCylinder()
{

    for (int i = 0; i <= stackCount; ++i)
    {
        float y = -length / 2.0f + (float)i / stackCount * length;
        float v = (float)i / stackCount;

        for (int j = 0; j <= sectorCount; ++j)
        {
            float u = (float)j / sectorCount;

            float sectorAngle = j * 2.0f * M_PI / sectorCount;
            float x = radius * cosf(sectorAngle);
            float z = radius * sinf(sectorAngle);
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));

            Vertex vertices;

            vertices.Normal = normal;
            vertices.Position = glm::vec3(x, y, z);
            vertices.TexCoords = glm::vec2(u, v);
            m_vertices.push_back(vertices);
        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            m_indices.push_back(k1);
            m_indices.push_back(k1 + 1);
            m_indices.push_back(k2);

            m_indices.push_back(k2);
            m_indices.push_back(k1 + 1);
            m_indices.push_back(k2 + 1);
        }
    }

    float yMin = -length/2.0f;
    float yMax = length/2.0f;

    for(auto& vertex : m_vertices) {
        float y = vertex.Position.y;
        
        // On normalise la hauteur (0.0 à la base, 1.0 au sommet)
        float factor = (y - yMin) / (yMax - yMin);
        
        // On assigne les IDs de nos deux os
        vertex.BoneIDs = glm::ivec2(0, 1);
        
        // L'os 0 (Base) influence surtout le bas
        // L'os 1 (Milieu) influence surtout le haut
        // La somme des deux poids DOIT être égale à 1.0
        vertex.Weights[0] = 1.0f - factor;
        vertex.Weights[1] = factor;
    }

    // Seulement APRÈS avoir rempli ces données, tu appelles :
    // setupMesh();

    setupMesh();
}