#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <iostream>

class sphere
{
private:
    float radius;

    float sectorStep;
    float stackStep;

    int sectorCount;
    int stackCount;

    float sectorAngle;
    float stackAngle;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normales;


    std::vector<unsigned int> indices;
    std::vector<int> lineIndices;

    // VAO vao;
    // VBO vbo;
    // EBO ebo;


public:
    sphere(int mySectorCount, int myStackCount, float myRadius);
    ~sphere();

    void renduSphere();
    void updateIndices();

    std::vector<glm::vec3>& getVertices();
    std::vector<glm::vec3>& getNormales();

    std::vector<unsigned int>& getIndices();
    std::vector<int>& getLineIndices();

    int getIndicesSize();
};

