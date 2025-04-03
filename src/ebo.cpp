#include <glad/glad.h>
#include "header/ebo.hpp"

EBO::EBO(const int *indices, size_t size){
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

EBO::EBO(const unsigned int* indices, size_t size) {
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}


EBO::EBO(std::vector<int> * indices, size_t size) {
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::del()
{
    glDeleteBuffers(1, &id);
}