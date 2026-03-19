#pragma once

#include <vector>

class Cubemap
{
private:
    unsigned int textureID;

public:
    Cubemap();

    unsigned int loadCubemap(std::vector<std::string> faces);

    void bind();
    void unbind();
    void del();
};
