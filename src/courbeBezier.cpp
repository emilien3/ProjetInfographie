#include "header/courbeBezier.hpp"
#include "header/utils.hpp"

courbeBezier::courbeBezier(int i)
    : curlVersion(i), controlPoints(i) //initialisation directe
{

    initBezierPoints(beziersPoints, controlPoints.getListPoint(), courbe, nbpoints);
    vbo = VBO(courbe, courbe.size());
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

controlPoint courbeBezier::getControlPoint()
{
    return controlPoints;
}

void courbeBezier::renduPointControl(){
    // Rendu des points


    controlPoints.getVAO().bind();
    // dernier elem est le nombre de points de controle
    glDrawArrays(GL_POINTS, 0, 4);
    glDrawArrays(GL_LINE_STRIP, 0, 4);    
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

        vbo.bind();// Mettre à jour le VBO avec les nouveaux points
        glBufferData(GL_ARRAY_BUFFER, courbe.size() * sizeof(glm::vec3), courbe.data(), GL_DYNAMIC_DRAW);

    }
    glDrawArrays(GL_LINE_STRIP, 0, courbe.size());
    vao.unbind();
}

