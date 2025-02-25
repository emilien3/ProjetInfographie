#pragma once



class VBO {

    private :
        unsigned int id;

    public :

        VBO(const float *vertices, size_t size);
        void bind();
        void unbind();
        void del();

};