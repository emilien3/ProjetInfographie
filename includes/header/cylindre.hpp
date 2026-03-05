#pragma once
#include "header/object.hpp"
#include <cmath>

class Cylinder : public Objet3D
{

    private:
        float radius;
        float length;
        int sectorCount;
        int stackCount; 
        void buildCylinder();

        
    public:
        Cylinder(float radius = 1.0f, float length = 5.0f, int sectorCount = 36, int stackCount = 10)
            : radius(radius), length(length), sectorCount(sectorCount), stackCount(stackCount)
        {
            buildCylinder();
        }
        ~Cylinder() override = default;
    
};