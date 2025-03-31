#include <iostream>
#include <vector>
#include <glm/glm.hpp>

float binomialCoeff(int n, int k)
{
    int res = 1;
    if (k > n - k)
        k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

void printPascalTriangles(int n)
{    
    for (int i = 0; i <= n; i++)
    {
        std::cout<< binomialCoeff(n, i) <<std::endl;
    }
}

float bernsteinPolynome(float u, int n, int i)
{
    float C_ni = binomialCoeff(n, i);
    float u_i = std::pow(u, i);
    float sous = std::pow((1- u), (n-i)) ;

    return C_ni * u_i * sous;
}
float bernsteinPolynome2D(float u, int n, int i, float v, int m, int j)
{
    return bernsteinPolynome(u, n, i) * bernsteinPolynome(v, m, j);
}

// n la taille du vecteur attendu
// u entre 0 et 1
void bezierCourbes(const std::vector<float> &bezierPoints, const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &courbe)
{
    int n = controlPoints.size();
    // pour l'ensemble de mes u
    for (int k = 0; k < bezierPoints.size(); k++)
    {
        float u = bezierPoints[k];
        courbe[k] = glm::vec3(0.f);
        for (int i = 0; i < controlPoints.size(); i++)
        {
            float B_n_i = bernsteinPolynome(u, n-1, i);
            courbe[k] += B_n_i*controlPoints[i];
        }
    }    
}


void initControlPoints1(std::vector<glm::vec3> &points)
{
    points.clear();
    glm::vec3 v1(0.f, 0.f, 0.f);
    glm::vec3 v2(2.f, 0.f, 2.f);
    glm::vec3 v3(3.f, 0.f, 1.f);
    
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
}

void initControlPoints2(std::vector<glm::vec3> &points)
{
    points.clear();
    glm::vec3 v1(0.f, 0.f, 0.f);
    glm::vec3 v2(0.f, 1.f, 0.f);
    glm::vec3 v3(0.f, 2.f, 1.f);

    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
}

void initBezierPoints(std::vector<float> &bezierPoints, const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &courbe, int n)
{
    //on fixe la taille à n
    bezierPoints.clear();
    
    for (int i = 0 ; i < n; i++)
    {
        bezierPoints.push_back(i/float(n-1));
    }
    courbe.resize(bezierPoints.size());
    
    bezierCourbes(bezierPoints, controlPoints, courbe);
}

std::vector<glm::vec3> concate2list(std::vector<glm::vec3> list1, std::vector<glm::vec3> list2)
{
    std::vector<glm::vec3> surface = list1;
    
    for(int j = 1; j < list2.size(); j++)
    {
        surface.push_back(list2[j]);

        for(int i = 1; i < list1.size(); i++)
        {
            glm::vec3 v1(list1[i].x, list2[j].y, floorf((list1[i].z+list2[j].z)/2));
            surface.push_back(v1);
        } 
    }
    return surface;
}