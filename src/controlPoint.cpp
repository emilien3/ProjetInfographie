#include "header/controlPoint.hpp"
#include "header/utils.hpp"

controlPoint::controlPoint(int i)
{   
    if (i==1)
    {
        initControlPoints1(listPoint);
    }
    else if (i == 2)
    {
        initControlPoints2(listPoint);
    }

    vbo = VBO(listPoint, listPoint.size());
    vao.bind();
    vao.linkAttrib(vbo);
    vao.unbind();
    vbo.unbind();  
}

controlPoint::~controlPoint()
{
    vao.del();
    vbo.del();
}

std::vector<glm::vec3> controlPoint::getListPoint()
{
    return listPoint;
}

VAO controlPoint::getVAO()
{
    return vao; 
}

VBO controlPoint::getVBO()
{
    return vbo;
}