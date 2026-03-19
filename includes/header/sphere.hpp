#pragma once


#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <iostream>

#include "header/object.hpp"
#include <cmath>

class Sphere : public Objet3D
{
    private:

        float radius;
        int sectorCount;
        int stackCount;

    public:
        // sphere(int mySectorCount, int myStackCount, float myRadius);

        Sphere(int sectorCount = 36, int stackCount = 16, float radius = 1.0f) 
            : radius(radius), sectorCount(sectorCount), stackCount(stackCount)
        {
            buildSphere();
        }

        ~Sphere() override = default;

        void buildSphere();

};

