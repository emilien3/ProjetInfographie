#include "header/controlPoint.hpp"
#include "header/utils.hpp"
#include "ebo.hpp"

class surfaceBezier
{
private:

    EBO ebo;
    std::vector<unsigned int> indices;

    controlPoint controlPoints;
    int tailleList1;
    int tailleList2;
    
    std::vector<glm::vec3> surface;
    
    int previousN = 4;
    int previousM = 4;
    
    VAO vao;
    VBO vbo;
    
public:
    
    int n = 4;
    int m = 4;

    surfaceBezier(std::vector<glm::vec3>surfacePoints, size_t tailleList1, size_t tailleList2, int n, int m);
    surfaceBezier(std::vector<glm::vec3>list1, std::vector<glm::vec3>list2);

    const controlPoint& getControlPoint();

    void renduSurfaceBezier();

    void updateIndices();

    ~surfaceBezier() = default;
};