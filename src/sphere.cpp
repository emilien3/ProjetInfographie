#include "header/sphere.hpp"
#include <math.h>

sphere::sphere(int mySectorCount, int myStackCount, float myRadius)
{
    radius = myRadius;
    sectorCount = mySectorCount;
    stackCount = myStackCount;

    renduSphere();

}

sphere::~sphere()
{
}

void sphere::renduSphere()
{
    vertices.clear();
    normales.clear();
    
    indices.clear();
    lineIndices.clear();

    float z, xy, x, y;
    float nx, ny, nz, lengthNorm = 1.0f/radius;

    sectorStep = 2* M_PI /sectorCount;
    stackStep = M_PI /stackCount;

    for (int i = 0; i < stackCount; ++i)
    {
        
        stackAngle = M_PI / 2 - i * stackStep;  // starting from pi/2 to -pi/2
        z = radius * sinf(stackAngle);
        nz = z * lengthNorm;
        xy = radius * cosf(stackAngle);
        
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            // Vertex position
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(glm::vec3(x, y, z));

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthNorm;
            ny = y * lengthNorm;
            normales.push_back(glm::vec3(nx, ny, nz));

        }
    }
    updateIndices();
}


void sphere::updateIndices()
{
    unsigned int k1, k2;

    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i!=0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1+1);
            }
            if (i != (stackCount-1)) 
            {
                indices.push_back(k1+1);
                indices.push_back(k2);
                indices.push_back(k2+1);
            }
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i!=0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1+1);
            }
        }
    }
}


std::vector<glm::vec3>& sphere::getVertices()
{
    return vertices;
}

std::vector<glm::vec3>& sphere::getNormales()
{
    return normales;
}

std::vector<unsigned int>& sphere::getIndices()
{
    return indices;
}

std::vector<int>& sphere::getLineIndices()
{
    return lineIndices;
}

int sphere::getIndicesSize()
{
    return indices.size();
}