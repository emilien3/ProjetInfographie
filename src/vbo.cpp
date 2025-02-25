#include <glad/glad.h>
#include "header/vbo.hpp"


VBO::VBO(const float *vertices, size_t size)
{
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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