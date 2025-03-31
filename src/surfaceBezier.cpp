#include "header/surfaceBezier.hpp"

surfaceBezier::surfaceBezier(std::vector<glm::vec3>surfacePoints, size_t tailleList1, size_t tailleList2)
    : controlPoints(surfacePoints), tailleList1(tailleList1), tailleList2(tailleList2)
{
    int tailleMin = std::min(tailleList1, tailleList2);
    std::vector<glm::vec3> listBezier;

    for(int i = tailleMin ; i <= surfacePoints.size(); i++) // i est une taille
    {
        if (((i%tailleList1)==0))
        {
            for (int k = i - tailleList1 - 1; k < i; k++) //k est indice
            {
                listBezier.push_back(surfacePoints[k]);
            }
            // courbeBezier maCourbe(listBezier);
            // listBezierCourbes.push_back(maCourbe);
            listBezier.clear();
        }
        else if ((i%tailleList2)==0){
            for (int k = i - tailleList2 - 1; k < i; k++) //k est indice
            {
                listBezier.push_back(surfacePoints[k]);
            }
            // courbeBezier maCourbe(listBezier);


            // listBezierCourbes.push_back(maCourbe);
            listBezier.clear();
        }
    }
}

surfaceBezier::surfaceBezier(std::vector<glm::vec3>list1, std::vector<glm::vec3>list2)
    : surfaceBezier(concate2list(list1, list2), list1.size(), list2.size())
{}


controlPoint& surfaceBezier::getControlPoint()
{
    return controlPoints;
}

