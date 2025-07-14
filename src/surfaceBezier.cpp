#include "header/surfaceBezier.hpp"

surfaceBezier::surfaceBezier(std::vector<glm::vec3>surfacePoints, size_t tailleList1, size_t tailleList2, int n, int m)
    : controlPoints(surfacePoints), tailleList1(tailleList1), tailleList2(tailleList2), n(n), m(m)
{
    bezierSurface(controlPoints.getListPoint(), surface, n, m, tailleList1, tailleList2);
    updateTriangleIndices();
    calculateSurfaceNormals();
    // updateIndices();

    // ebo = EBO(indices.data(), indices.size() * sizeof(unsigned int));
    triangleEBO = EBO(triangleIndices.data(), triangleIndices.size() * sizeof(unsigned int));

    vao.bind();
    
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, surface.size() * sizeof(glm::vec3), surface.data(), GL_DYNAMIC_DRAW);
    vao.linkAttrib(vbo, 0);
    
    normalVBO.bind();
    glBufferData(GL_ARRAY_BUFFER, surface.size() * sizeof(glm::vec3), surfaceNormals.data(), GL_DYNAMIC_DRAW);
    vao.linkAttrib(normalVBO, 1);

    triangleEBO.bind();
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndices.size() * sizeof(unsigned int), triangleIndices.data(), GL_DYNAMIC_DRAW);

    // unbind
    vao.unbind();
    vbo.unbind();
    triangleEBO.unbind();
}

surfaceBezier::surfaceBezier(std::vector<glm::vec3>list1, std::vector<glm::vec3>list2)
    : surfaceBezier(concate2list(list1, list2), list1.size(), list2.size(), 4, 4)
{}

void surfaceBezier::updateTriangleIndices() {
    triangleIndices.clear();
    if (n < 2 || m < 2) return;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m - 1; ++j) {

            unsigned int topLeft = i * m + j;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (i + 1) * m + j;
            unsigned int bottomRight = bottomLeft + 1;
            triangleIndices.push_back(topLeft);
            triangleIndices.push_back(topRight);
            triangleIndices.push_back(bottomLeft);

            triangleIndices.push_back(topRight);
            triangleIndices.push_back(bottomRight);
            triangleIndices.push_back(bottomLeft);
        }
    }
}

void surfaceBezier::calculateSurfaceNormals() {
    surfaceNormals.clear();
    surfaceNormals.resize(surface.size(), glm::vec3(0.0f));

    if (n < 2 || m < 2 || triangleIndices.empty()) return;

    for (int idx = 0; idx < triangleIndices.size(); idx += 3) {
        // les indices des trian
        unsigned int i1 = triangleIndices[idx];
        unsigned int i2 = triangleIndices[idx + 1];
        unsigned int i3 = triangleIndices[idx + 2];
        const glm::vec3& v1 = surface[i1];
        const glm::vec3& v2 = surface[i2];
        const glm::vec3& v3 = surface[i3];

        //  On doit prendre les cote des triangles pour faire
        // un produit vectoriel qui nous la normale de la face

        glm::vec3 cote1 = v2 - v1;
        glm::vec3 cote2 = v3 - v1;
        glm::vec3 faceNormal = glm::cross(cote1, cote2);
        // on ajout la normal au vector les contenans
        surfaceNormals[i1] += faceNormal;
        surfaceNormals[i2] += faceNormal;
        surfaceNormals[i3] += faceNormal;
    }

    //on notrmalise les normals
    for (glm::vec3& normal : surfaceNormals) {
        normal = glm::normalize(normal);
    }
}



const controlPoint& surfaceBezier::getControlPoint()
{
    return controlPoints;
}

void surfaceBezier::renduSurfaceBezier(){

    // bezierVAO.bind();
    // ebo.bind();
    
    if ((previousN != n) || (previousM != m))
    {
        //actualisation du nombre de points de la courbe de beziers
        previousN = n;
        previousM = m;
        bezierSurface(controlPoints.getListPoint(), surface, n, m, tailleList1, tailleList2);
        updateTriangleIndices();
        calculateSurfaceNormals();
        
        vbo.bind();
        glBufferData(GL_ARRAY_BUFFER, surface.size() * sizeof(glm::vec3), surface.data(), GL_DYNAMIC_DRAW);
        
        normalVBO.bind();
        glBufferData(GL_ARRAY_BUFFER, surfaceNormals.size() * sizeof(glm::vec3), surfaceNormals.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        triangleEBO.bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndices.size() * sizeof(unsigned int), triangleIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    vao.bind();
    triangleEBO.bind();
    glDrawElements(GL_TRIANGLES, triangleIndices.size(), GL_UNSIGNED_INT, (void*)0);
    
    // // Dessiner la surface avec des lignes
    // for(int i = 0; i < n; ++i)
    // {
    //     // glDrawArrays(GL_LINE_STRIP, i * m, m);
    //     glDrawElements(GL_LINE_STRIP, m, GL_UNSIGNED_INT, (void*)(i * m * sizeof(unsigned int)));

    // }
    // for(int j = 0; j < m; ++j)
    // {
    //     // glDrawArrays(GL_LINE_STRIP, j, n);
    //     glDrawElements(GL_LINE_STRIP, n, GL_UNSIGNED_INT, (void*)(n * m * sizeof(unsigned int) + j * n * sizeof(unsigned int)));

    // }
    vao.unbind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

std::vector<glm::vec3>& surfaceBezier::getSurface()
{
    return surface;
}