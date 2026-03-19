#include "header/bone.hpp"

glm::mat4 Bone::getFinalMatrix() const 
{
    if (parent) {
        return parent->getFinalMatrix() * localTransform * offsetMatrix;
    }
    return localTransform * offsetMatrix;
}