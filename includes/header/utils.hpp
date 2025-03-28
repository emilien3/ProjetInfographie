#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

float binomialCoeff(int n, int k);

void printPascalTriangles(int n);

float bernsteinPolynome(float u, int n, int i);
// n la taille du vecteur attendu
// u entre 0 et 1
void bezierCourbes(const std::vector<float> &bezierPoints, const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &courbe);

void initControlPoints1(std::vector<glm::vec3> &points);
void initControlPoints2(std::vector<glm::vec3> &points);

void initBezierPoints(std::vector<float> &bezierPoints, const std::vector<glm::vec3> &controlPoints, std::vector<glm::vec3> &courbe, int n);