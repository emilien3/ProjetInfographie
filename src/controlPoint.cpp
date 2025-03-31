#include "header/controlPoint.hpp"
#include "header/utils.hpp"

controlPoint::controlPoint(const std::vector<glm::vec3> &list) :
listPoint(list), vbo(listPoint, listPoint.size())
{   
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

const std::vector<glm::vec3>& controlPoint::getListPoint() const
{
    return listPoint;
}

VAO& controlPoint::getVAO()
{
    return vao; 
}

VBO& controlPoint::getVBO()
{
    return vbo;
}