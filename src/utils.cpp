#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "header/utils.hpp"

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

void bezierCourbes(const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &courbe, int n)
{
    courbe.resize(n);
    int nbControlPoints = controlPoints.size();
    // pour l'ensemble de mes u
    for (int k = 0; k < n; k++)
    {
        float u = k/float(n-1);
        courbe[k] = glm::vec3(0.f);
        for (int i = 0; i < controlPoints.size(); i++)
        {
            float B_n_i = bernsteinPolynome(u, nbControlPoints-1, i);
            courbe[k] += B_n_i*controlPoints[i];
        }
    }    
}

// (float u, int n, int i, float v, int m, int j)

void bezierSurface(const std::vector<glm::vec3> &controlPoints,
                std::vector<glm::vec3> &surface,
                int n, int m, int nbControlPoints1, int nbControlPoints2)
{
    surface.clear();
    surface.resize(n*m);
    for (int k = 0; k < n; k++)
    {
        float u = k/float(n-1);
        for (int l = 0; l < m; l++)
        {
            float v = l/float(m-1);
            glm::vec3 vec(0.f);

            for (int i = 0; i < nbControlPoints1; i++)
            {
                for (int j= 0; j < nbControlPoints2; j++ )
                {
                    float B_n_i_m_j = bernsteinPolynome2D(
                        u, nbControlPoints1-1, i, 
                        v, nbControlPoints2-1, j
                    );
                    vec += B_n_i_m_j*controlPoints[ i * nbControlPoints2 + j];
                }
            }
            surface[k * m +l] = vec;
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


void initControlPoints3(std::vector<glm::vec3> &points)
{
    points.clear();
    points = {
        glm::vec3(-2.0f, 0.0f, -2.0f),
        glm::vec3(-1.0f, 1.0f, -2.0f),
        glm::vec3(0.0f, 0.0f, -2.0f),
        glm::vec3(1.0f, -1.0f, -2.0f),
        glm::vec3(2.0f, 0.0f, -2.0f)
    };
}

void initControlPoints4(std::vector<glm::vec3> &points)
{
    points.clear();
    points = {
        glm::vec3(-2.0f, 0.0f, 2.0f),
        glm::vec3(-1.0f, 1.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(1.0f, -1.0f, 2.0f),
        glm::vec3(2.0f, 0.0f, 2.0f)
    };
}

std::vector<glm::vec3> concate2list(std::vector<glm::vec3> list1, std::vector<glm::vec3> list2)
{
    std::vector<glm::vec3> surface = list1;
    
    for(int j = 1; j < list2.size(); j++)
    {
        surface.push_back(list2[j]);

        for(int i = 1; i < list1.size(); i++)
        {
            glm::vec3 point(list1[i].x,
                        list2[j].y, 
                        (list1[i].z+list2[j].z)/2.0f
            );
            surface.push_back(point);
        } 
    }
    return surface;
}