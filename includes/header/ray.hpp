#include <glm/vec3.hpp>
#include "glad/glad.h"

#include <iostream>
#include "header/vao.hpp"
#include "header/vbo.hpp"
#include "header/utils.hpp"


class ray
{
private:

    std::vector<glm::vec3> rayStored;
    VAO vao;
    VBO vbo;
    
public:
    ray();
    void add_ray(glm::vec3 pos, glm::vec3 front);
    void add_ray(glm::vec3 pos, glm::vec3 front, int position);
    void renduRay();
    void updateGLObject();
    void verifCollisions(glm::vec3 sphereCenter, float radius);


    std::vector<glm::vec3>& getRayStored(){ return rayStored; }
    void clear_ray(){rayStored.clear();}
    ~ray(){};
};

