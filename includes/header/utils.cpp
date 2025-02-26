#include <iostream>
#include <vector>
#include <glm/glm.hpp>


int binomialCoeff(int n, int k) {
    int res = 1;
    if (k > n - k)
        k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }

    return res;
}


void printPascalTriangles(int n){
    
    for (int i = 0; i <= n; i++)
    {
        std::cout<< binomialCoeff(n, i) <<std::endl;
    }
}



float bernsteinPolynome(float u, int n, int i){

    int C_ni = binomialCoeff(n, i);
    float u_i = std::pow(u, i);
    float sous = std::pow((1- u), (n-i)) ;

    return C_ni * u_i * sous;
}


// u entre 0 et 1
float bezierCourbes(float u, int n,const std::vector<glm::vec3> &points){
    

    for (int i = 0; i < n; i++)
    {
        float B = bernsteinPolynome(u, n, i);

        float x_u = points[i].x;
        float y_u = points[i].y;
        float z_u = points[i].z;
        
    }
    
    return 0.f;
}
