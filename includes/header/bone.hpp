#pragma once

#include "header/controlPoint.hpp"

class bone
{
private:
    
    controlPoint ;


public:
    bone() = delete;
    
    bone(glm::vec3 point1, glm::vec3 point2);

    ~bone();


};