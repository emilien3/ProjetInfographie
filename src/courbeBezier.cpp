#include "header/courbeBezier.hpp"
#include "header/utils.hpp"

courbeBezier::courbeBezier(const std::vector<glm::vec3> &list)
    : controlPoints(list) //initialisation directe
{
    initBezierPoints(beziersPoints, controlPoints.getListPoint(), courbe, nbpoints);
    // vbo = VBO(courbe, courbe.size());

    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, courbe.size() * sizeof(glm::vec3), courbe.data(), GL_DYNAMIC_DRAW);

    vao.bind();
    vao.linkAttrib(vbo);
    // unbind
    vao.unbind();
    vbo.unbind();
}

courbeBezier::~courbeBezier()
{
    vao.del();
    vbo.del();
}

int courbeBezier::getNbPoint()
{
    return nbpoints;
}

controlPoint& courbeBezier::getControlPoint()
{
    return controlPoints;
    
}

void courbeBezier::renduPointControl(){
    // Rendu des points
    controlPoints.getVAO().bind();
    controlPoints.getVBO().bind(); // test
    // dernier elem est le nombre de points de controle
    glDrawArrays(GL_POINTS, 0, controlPoints.getListPoint().size());
    glDrawArrays(GL_LINE_STRIP, 0, controlPoints.getListPoint().size());    
    controlPoints.getVAO().unbind();
}

void courbeBezier::renduCourbeBezier(){

    // bezierVAO.bind();
    vao.bind();
    if (previousnbpoints != nbpoints)
    {
        //actualisation du nombre de points de la courbe de beziers
        previousnbpoints = nbpoints;
        initBezierPoints(beziersPoints, controlPoints.getListPoint(), courbe, nbpoints);

        // /// TEST
        // vbo.bind();
        // vbo = VBO(courbe, courbe.size());
        // ///

        vbo.bind();// Mettre à jour le VBO avec les nouveaux points
        glBufferData(GL_ARRAY_BUFFER, courbe.size() * sizeof(glm::vec3), courbe.data(), GL_DYNAMIC_DRAW);
        vbo.unbind();
        // vao.linkAttrib(vbo);//test
    }
    glDrawArrays(GL_LINE_STRIP, 0, courbe.size());
    vao.unbind();
}

