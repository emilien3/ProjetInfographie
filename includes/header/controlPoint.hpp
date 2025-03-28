#pragma once
#include <iostream>

//OPENGL libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/vao.hpp"
#include "header/vbo.hpp"


class controlPoint
{

private:
    std::vector<glm::vec3> listPoint;
    VAO vao;
    VBO vbo;

public:

    controlPoint() = delete;
    controlPoint(int i);
    ~controlPoint();

    std::vector<glm::vec3> getListPoint();

    VAO getVAO();
    VBO getVBO();
};
