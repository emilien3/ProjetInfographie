#pragma once
#include <iostream>

//OPENGL libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/controlPoint.hpp"
#include "header/vao.hpp"
#include "header/vbo.hpp"

class courbeBezier
{
private:

    std::vector<float> beziersPoints;
    std::vector<glm::vec3> courbe;

    controlPoint controlPoints;

    int previousnbpoints = 4;
    VAO vao;
    VBO vbo;

public:

    courbeBezier() = delete;
        
    courbeBezier(const std::vector<glm::vec3> &list);
    ~courbeBezier();

    int nbpoints = 4;
    
    void renduPointControl();
    void renduCourbeBezier();
    
    controlPoint& getControlPoint();
    int getNbPoint();
};