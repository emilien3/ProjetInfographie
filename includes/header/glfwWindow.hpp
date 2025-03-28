#pragma once

//OPENGL libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class glfwWindow
{
private:
    GLFWwindow *window;
public:
    glfwWindow(const unsigned int SCR_WIDTH,const unsigned int SCR_HEIGHT);
    
    ~glfwWindow();
};
