#include "header/ray.hpp"

ray::ray()
{
    GL_CHECK(vbo.bind());
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW)); // Buffer vide valide
    GL_CHECK(vao.bind());
    vao.linkAttrib(vbo, 3); // Configuration des attributs
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