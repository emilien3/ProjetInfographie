#pragma once

#include <glm/glm.hpp>

class Bone {

    public:

        int id;
        glm::mat4 offsetMatrix;
        glm::mat4 localTransform;
        
        Bone* parent; 
        
        Bone(int _id, glm::mat4 _offset) : id(_id), offsetMatrix(_offset), localTransform(1.0f), parent(nullptr) {}
        
        glm::mat4 getFinalMatrix() const;

};