#pragma once


class EBO {

    private : 
        unsigned int id ;

    public :
        EBO(const int *indices, size_t size);
        void bind();
        void unbind();
        void del();

};