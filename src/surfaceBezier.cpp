#include "header/surfaceBezier.hpp"

surfaceBezier::surfaceBezier(std::vector<glm::vec3>surfacePoints, size_t tailleList1, size_t tailleList2, int n, int m)
    : controlPoints(surfacePoints), tailleList1(tailleList1), tailleList2(tailleList2), n(n), m(m)
{
    bezierSurface(controlPoints.getListPoint(), surface, n, m, tailleList1, tailleList2);
    
    updateIndices();
    ebo = EBO(indices.data(), indices.size() * sizeof(unsigned int)); // Modifiez le constructeur EBO si nécessaire
    
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, surface.size() * sizeof(glm::vec3), surface.data(), GL_DYNAMIC_DRAW);
    vao.bind();
    vao.linkAttrib(vbo);
    // unbind
    vao.unbind();
    vbo.unbind();
}

surfaceBezier::surfaceBezier(std::vector<glm::vec3>list1, std::vector<glm::vec3>list2)
    : surfaceBezier(concate2list(list1, list2), list1.size(), list2.size(), 4, 4)
{}

const controlPoint& surfaceBezier::getControlPoint()
{
    return controlPoints;
}

void surfaceBezier::renduSurfaceBezier(){

    // bezierVAO.bind();
    vao.bind();
    ebo.bind();

    if ((previousN != n) || (previousM != m))
    {
        updateIndices();
        ebo.bind();

        // //actualisation du nombre de points de la courbe de beziers
        // previousN = n;
        // previousM = m;
        // bezierSurface(controlPoints.getListPoint(), surface, n, m, tailleList1, tailleList2);
        // vbo.bind();// Mettre à jour le VBO avec les nouveaux points
        // glBufferData(GL_ARRAY_BUFFER, surface.size() * sizeof(glm::vec3), surface.data(), GL_DYNAMIC_DRAW);
        // vbo.unbind();
    // }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // Dessiner la surface avec des lignes
    for(int i = 0; i < n; ++i)
    {
        // glDrawArrays(GL_LINE_STRIP, i * m, m);
        glDrawElements(GL_LINE_STRIP, m, GL_UNSIGNED_INT, (void*)(i * m * sizeof(unsigned int)));

    }
    for(int j = 0; j < m; ++j)
    {
        // glDrawArrays(GL_LINE_STRIP, j, n);
        glDrawElements(GL_LINE_STRIP, n, GL_UNSIGNED_INT, (void*)(n * m * sizeof(unsigned int) + j * n * sizeof(unsigned int)));

    }
    vao.unbind();

    // glDrawArrays(GL_LINE_STRIP, 0, surface.size());
    // vao.unbind();
}


void surfaceBezier::updateIndices()
{
    indices.clear();
    
    // Lignes horizontales (U)
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            indices.push_back(i * m + j);
        }
    }
    
    // Lignes verticales (V)
    for(int j = 0; j < m; ++j) {
        for(int i = 0; i < n; ++i) {
            indices.push_back(i * m + j);
        }
    }
}