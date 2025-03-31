#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

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