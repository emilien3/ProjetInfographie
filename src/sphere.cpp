#include "header/sphere.hpp"

void Sphere::buildSphere()
{
    float x, y, z, xy;
    float nx, ny, nz, lengthNorm = 1.0f / radius;
    float s, t; // Coordonnées UV
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        
        stackAngle = M_PI / 2.0f - i * stackStep; // de pi/2 à -pi/2
        z = radius * sinf(stackAngle);
        nz = z * lengthNorm;
        xy = radius * cosf(stackAngle);
        
        for (int j = 0; j <= sectorCount; ++j) {

            sectorAngle = j * sectorStep;

            // Position cartésienne
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            
            // Normale normalisée
            nx = x * lengthNorm;
            ny = y * lengthNorm;
            
            // Coordonnées de texture UV
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            Vertex vertex;
            vertex.Position = glm::vec3(x, y, z);
            vertex.Normal = glm::vec3(nx, ny, nz);
            vertex.TexCoords = glm::vec2(s, t);
            m_vertices.push_back(vertex);
        }
    }

    unsigned int k1, k2;

    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) 
            {
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
        }
    }

    // Appel à la fonction de la classe parente pour initialiser VAO/VBO/EBO 
    setupMesh();
}