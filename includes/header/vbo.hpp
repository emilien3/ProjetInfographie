#pragma once


#include <iostream>
#include <glm/glm.hpp>



class VBO {

    private :
        unsigned int id;

    public :
        
        VBO(const VBO&) = delete;
        VBO& operator=(const VBO&) = delete;

        VBO();
        VBO(const float *vertices, size_t size);
        VBO(std::vector<glm::vec3> &points, size_t size);
        void bind();
        void unbind();
        void del();
};