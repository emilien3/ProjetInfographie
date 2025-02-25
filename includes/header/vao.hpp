#pragma once
#include "header/vbo.hpp"

class VAO {

    private:
        unsigned int id;

    public :

        VAO();
        void linkAttrib(VBO& vbo);
        void bind();
        void unbind();
        void del();
};