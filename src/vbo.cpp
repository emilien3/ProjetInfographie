#include <glad/glad.h>

#include "header/vbo.hpp"

VBO::VBO(){}

VBO::VBO(const float *vertices, size_t size)
{
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}


VBO::VBO(std::vector<glm::vec3> &points, size_t size)
{
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec3), points.data(), GL_DYNAMIC_DRAW);
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::del()
{
    glDeleteBuffers(1, &id);
}