#pragma once

// #pragma once
// #include "header/object.hpp"
// #include <cmath>

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"

class Cylinder {
private:
    float radius;
    float length;
    int sectorCount;
    int stackCount; 

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normales;
    std::vector<unsigned int> indices;

    VAO vao;
    VBO vbo;
    VBO normalVBO;
    EBO ebo;

    void buildCylinder();
    void updateBuffers();

public:
    Cylinder(float radius = 0.5f, float length = 3.0f, int sectorCount = 36, int stackCount = 10);
    ~Cylinder();

    void renduCylinder();
    
    std::vector<glm::vec3>& getVertices() { return vertices; }
    std::vector<unsigned int>& getIndices() { return indices; }
};