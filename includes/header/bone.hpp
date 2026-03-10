#pragma once

#include "header/controlPoint.hpp"

class Bone
{
private:
    
    glm::vec3 position;


public:
    Bone() = delete;
    
    Bone(glm::vec3 pos);

    ~Bone(){};


};