#pragma once
#include "header/vbo.hpp"

class VAO {

    private:
        unsigned int id;

    public :

        //suppression de la copie pour eviter 
        // erreur opengl -> voir doc
        VAO(const VAO&) = delete;
        VAO& operator=(const VAO&) = delete;

        //Autorisation des déplacements
        VAO(VAO&& other) noexcept {
            id = other.id;
            other.id = 0; // Empêche la suppression de l'ancien VAO
        }
        
        VAO& operator=(VAO&& other) noexcept {
            if (this != &other) {
                glDeleteVertexArrays(1, &id); // Supprime l'ancien VAO
                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        VAO();
        void linkAttrib(VBO& vbo);
        void bind();
        void unbind();
        void del();
};