#include "header/cylindre.hpp"
#include <math.h>

Cylinder::Cylinder(float radius, float length, int sectorCount, int stackCount)
    : radius(radius), length(length), sectorCount(sectorCount), stackCount(stackCount)
{
    buildCylinder();
    updateBuffers();
}

Cylinder::~Cylinder() {}

void Cylinder::buildCylinder()
{
    vertices.clear();
    normales.clear();
    indices.clear();

    for (int i = 0; i <= stackCount; ++i)
    {
        // On calcule la position sur la hauteur (axe Y) : de -length/2 à +length/2
        float y = -length / 2.0f + (float)i / stackCount * length;

        for (int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * 2.0f * M_PI / sectorCount;

            // Calcul X et Z avec la trigonométrie classique
            float x = radius * cosf(sectorAngle);
            float z = radius * sinf(sectorAngle);
            
            vertices.push_back(glm::vec3(x, y, z));

            // La normale d'un cylindre pointe vers l'extérieur sur le plan XZ (Y = 0)
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));
            normales.push_back(normal);
        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }
    }
}

void Cylinder::updateBuffers()
{
    vao.bind();

    // pos
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    vao.linkAttrib(vbo, 0, 3);

    // normales
    normalVBO.bind();
    glBufferData(GL_ARRAY_BUFFER, normales.size() * sizeof(glm::vec3), normales.data(), GL_STATIC_DRAW);
    vao.linkAttrib(normalVBO, 1, 3);

    // EBO
    ebo = EBO(indices.data(), indices.size() * sizeof(unsigned int));
    // ebo.bind();
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    vao.unbind();
    vbo.unbind();
    normalVBO.unbind();
    ebo.unbind();
}

void Cylinder::renduCylinder()
{
    vao.bind();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.unbind();
}