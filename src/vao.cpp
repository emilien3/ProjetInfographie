#include <glad/glad.h>
#include "header/vao.hpp"

VAO::VAO(){
    glGenVertexArrays(1, &id);
}

void VAO::linkAttrib(VBO& vbo)
{
    vbo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
}

void VAO::linkAttrib(VBO& vbo, int layout)
{
    vbo.bind();
    glVertexAttribPointer((GLuint)layout, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray((GLuint)layout);
}
void VAO::linkAttrib(VBO& vbo, int layout, int components)
{
    vbo.bind();
    glVertexAttribPointer((GLuint)layout, (GLuint)components, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray((GLuint)layout);
}

void VAO::linkAttrib(VBO& vbo, GLuint layoutLocation, GLint components, GLenum type, GLsizeiptr stride, const void* offset)
{
    vbo.bind();
    glVertexAttribPointer(layoutLocation, components, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layoutLocation);
}


void VAO::bind(){
    glBindVertexArray(id);
}

void VAO::unbind()
{
    glBindVertexArray(0);
}

void VAO::del()
{
    glDeleteVertexArrays(1, &id);
}
