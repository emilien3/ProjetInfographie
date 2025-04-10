#include "header/ray.hpp"

ray::ray()
{
    GL_CHECK(vbo.bind());
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW)); // Buffer vide valide
    GL_CHECK(vao.bind());
    vao.linkAttrib(vbo, 0, 3); // Configuration des attributs
    //unbind
    GL_CHECK(vao.unbind());
    GL_CHECK(vbo.unbind());
};

void ray::add_ray(glm::vec3 pos, glm::vec3 front){
    rayStored.push_back(pos);
    rayStored.push_back(glm::vec3(
                                front.x * 10 + pos.x, 
                                front.y * 10 + pos.y,
                                front.z * 10 + pos.z
                            )
    );

    std::cout << pos.x << pos.y << pos.z << std::endl;
    std::cout << front.x << front.y << front.z << std::endl;
    updateGLObject();
    GL_CHECK(checkOpenGLError("After updateGLObject in add_ray", __FILE__, __LINE__));
}

void ray::updateGLObject(){
    if (rayStored.empty())
    {
        GL_CHECK(vbo.bind());
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW));
        GL_CHECK(vbo.unbind());
        return;
    }
    // vao.bind();

    GL_CHECK(vbo.bind());
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER,
                rayStored.size() * sizeof(glm::vec3),
                rayStored.data(),
                GL_DYNAMIC_DRAW));
    // vao.unbind();
    GL_CHECK(vbo.unbind());
}

void ray::renduRay(){
    // glDrawElements(GL_LINE_STRIP, m, GL_UNSIGNED_INT, (void*)(i * m * sizeof(unsigned int)));
    if (rayStored.size() < 2 || rayStored.size() % 2 != 0) return;
    // updateGLObject();
    GL_CHECK(vao.bind());
    GL_CHECK(vbo.bind());
    // vbo.bind();
    GL_CHECK(glDrawArrays(GL_LINES, 0, rayStored.size()));
    GL_CHECK(vbo.unbind());
    GL_CHECK(vao.unbind());
}


void ray::verifCollisions(glm::vec3 sphereCenter, float radius)
{
    for (int i = 0; i < rayStored.size(); i+=2)
    {
        glm::vec3 rStart = rayStored[i];
        glm::vec3 rEnd = rayStored[i+1];
        glm::vec3 v = rEnd - rStart;
        glm::vec3 oc = rStart - sphereCenter;
        glm::vec3 d = glm::normalize(rEnd - rStart);
        float r_2 = radius * radius;
        
        //  b = 2 * (o-c) d  //  a = dot d d  //  c = (o+c)^2 - (oc)^2 - r^2

        float B = 2.0f * glm::dot(oc, d) ;
        float A = glm::dot(d, d);
        float C =  glm::dot(oc, oc) - r_2 ;
        
        //\delta = b^2 - 4*a*c
        float delta = B*B - 4 * A * C;

        if(delta >= 0)
        {
            float t = -B - glm::sqrt(delta) / 2 * A ;
            if ((t >=0 )&& (t*t <= glm::dot(rEnd - rStart, rEnd - rStart)))
            {
                glm::vec3 coordContact = rStart + d * t ;
                
                // la normal au pt de contact
                glm::vec3 normal = glm::normalize(coordContact-sphereCenter);

                //calcul de la reflexion
                float proj = glm::dot(normal, d);
                glm::vec3 dReflect = d - 2.0f * proj * normal;

                // on modifie et ajoute les rayons
                rayStored[i + 1] = coordContact; 
                add_ray(coordContact, dReflect); 

            }
        }
    }
}