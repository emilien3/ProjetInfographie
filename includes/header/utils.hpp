#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
#include <glad/glad.h> // Assurez-vous que GLAD est inclus

inline void checkOpenGLError(const char* stmt, const char* fname, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error 0x" << std::hex << err << std::dec
        << " at " << stmt
        << " in " << fname << ":" << line << std::endl;
    }
}
#define GL_CHECK(stmt) do { \
    stmt; \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)
////////////////////////////////////////////////////////////////////////////////

float binomialCoeff(int n, int k);

void printPascalTriangles(int n);

float bernsteinPolynome(float u, int n, int i);
float bernsteinPolynome2D(float u, int n, int i, float v, int m, int j);

// n la taille du vecteur attendu
// u entre 0 et 1
void bezierCourbes(const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &courbe, int n);
void bezierSurface(const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &surface, int n, int m, int nbControlPoints1, int nbControlPoints2);

void initControlPoints1(std::vector<glm::vec3> &points);
void initControlPoints2(std::vector<glm::vec3> &points);
void initControlPoints3(std::vector<glm::vec3> &points);
void initControlPoints4(std::vector<glm::vec3> &points);

std::vector<glm::vec3> concate2list(std::vector<glm::vec3> list1, std::vector<glm::vec3> list2);