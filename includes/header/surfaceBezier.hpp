#include "header/courbeBezier.hpp"
#include "header/utils.hpp"

class surfaceBezier
{
private:

    controlPoint controlPoints;
    std::vector<courbeBezier> listBezierCourbes;

    int tailleList1;
    int tailleList2;


public:
    surfaceBezier(std::vector<glm::vec3>surfacePoints, size_t tailleList1, size_t tailleList2);
    surfaceBezier(std::vector<glm::vec3>list1, std::vector<glm::vec3>list2);

    controlPoint& getControlPoint();
    ~surfaceBezier() = default;
};