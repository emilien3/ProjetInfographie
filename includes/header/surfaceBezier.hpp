#include "header/controlPoint.hpp"
#include "header/utils.hpp"
#include "ebo.hpp"

class surfaceBezier
{
private:

    EBO ebo;
    EBO triangleEBO;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> triangleIndices;

    controlPoint controlPoints;
    int tailleList1;
    int tailleList2;
    
    std::vector<glm::vec3> surface;
    std::vector<glm::vec3> surfaceNormals;
    
    int previousN = 4;
    int previousM = 4;
    
    VAO vao;
    VBO vbo;
    VBO normalVBO;
    
public:
    
    int n = 4;
    int m = 4;

    surfaceBezier(std::vector<glm::vec3>surfacePoints, size_t tailleList1, size_t tailleList2, int n, int m);
    surfaceBezier(std::vector<glm::vec3>list1, std::vector<glm::vec3>list2);

    const controlPoint& getControlPoint();

    void renduSurfaceBezier();

    void updateIndices();

    std::vector<glm::vec3>& getSurface();

    void updateTriangleIndices();
    void calculateSurfaceNormals();


    EBO& getTriangleEBO(){return triangleEBO;}
    VAO& getVAO(){return vao;}
    VBO& getVBO(){return vbo;}


    const std::vector<unsigned int>& getTriangleIndices() const { return triangleIndices; } 

    ~surfaceBezier() = default;
};