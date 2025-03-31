#pragma once


class EBO {

    private : 
        unsigned int id ;

    public :

        EBO()= default;
        EBO(const int *indices, size_t size);

        EBO(const unsigned int* indices, size_t size);
        void bind();
        void unbind();
        void del();

};
