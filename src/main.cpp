//Imgui includes
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

//OPENGL libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

//learnopengl libs
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

//C lib
#include <iostream>
#include <stdbool.h>

//My files
#include "header/vao.hpp"
#include "header/vbo.hpp"
#include "header/ebo.hpp"

#include "header/controlPoint.hpp"
#include "header/courbeBezier.hpp"
#include "header/surfaceBezier.hpp"
#include "header/sphere.hpp"
#include "header/ray.hpp"

#include "header/utils.hpp"
// #include "header/glfwWindow.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void shift_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void space_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void clear_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window, ray& rayTraced);

// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool shiftMode = false;
bool spacePressedLastFrame = false;
bool showNormalsMode = false;
bool N_KeyPressedLastFrame = false;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
    /////////////////////// CONFIG ////////////////////////////
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    // glfwWindow window(SCR_WIDTH, SCR_HEIGHT);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, shift_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // -> pas utile

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    ////////////////////////////////////////////////////////////

    std::vector<glm::vec3> liste1;
    std::vector<glm::vec3> liste2;
    std::vector<glm::vec3> liste3;
    std::vector<glm::vec3> liste4;
    
    initControlPoints1(liste1);
    initControlPoints2(liste2);

    initControlPoints3(liste3);
    initControlPoints4(liste4);
    
    courbeBezier courbe1(liste3);
    courbeBezier courbe2(liste4);
    
    std::vector<glm::vec3> surface = concate2list(liste1, liste2);
    surfaceBezier maSurface(surface, liste1.size(), liste2.size(), 20, 20);

    sphere maSphere(36, 18, 1.f);

    // ray
    ray rayTraced;

    // std::cout << "Type de surface : " << typeid(surface).name() << std::endl;
    // for (auto &&i : surface)
    // {
    //     std::cout << i.x << i.y << i.z << std::endl; 
    // }

    // std::vector<glm::vec3> testing = maSurface.getControlPoint().getListPoint();
    // std::cout << "Type de surface from object ma surface : " << typeid(surface).name() << std::endl;
    // for (auto &&i : surface)
    // {
    //     std::cout << i.x << i.y << i.z << std::endl; 
    // }

    ////////////////////// SHADERS /////////////////////////////
    // build and compile our shader zprogram
    // ------------------------------------
    Shader colorShader("../shaders/1.colors.vs", "../shaders/1.colors.fs");
    Shader lightCubeShader("../shaders/1.light_cube.vs", "../shaders/1.light_cube.fs");
    Shader newShader("../shaders/lighting.vs", "../shaders/lighting.fs");
    Shader normalsShader("../shaders/lighting.vs", "../shaders/normal.fs");
    ////////////////////////////////////////////////////////////

    //////////////////////// DATA ////////////////////////////
    // set up vertex data (and buffer(s)) and indices
    // ------------------------------------------------------------------

    // points dans l'espace représentant un cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
    };

    //indices permettant de réutiliser les coord pour faire un cube et éviter les doublons de points
    int indices[]{
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        7, 3, 0,
        0, 4, 7,
        6, 2, 1,
        1, 5, 6,
        0, 1, 5,
        5, 4, 0,
        3, 2, 6,
        6, 7, 3
    };
    ////////////////////////////////////////////////////////////

    ////////////////////// VAO, VBO, EBO ////////////////////////////

    ////////////////////////////////////////////
    //////////////// CUBE //////////////////////
    ////////////////////////////////////////////

    VAO cubeVAO;
    VBO vbo(vertices, sizeof(vertices)); // utilisation du 1er constructeur
    EBO ebo(indices, sizeof(indices));
    cubeVAO.bind();
    ebo.bind();
    cubeVAO.linkAttrib(vbo);
    cubeVAO.unbind();
    // unbind
    vbo.unbind();
    ebo.unbind();

    //////////////////////////////////////////////
    //////////////// SPHERE //////////////////////
    //////////////////////////////////////////////

    //////////// VERTICES ///////////////////
    maSphere.renduSphere();

    std::vector<glm::vec3>& sphereVertices = maSphere.getVertices();
    std::vector<unsigned int>& sphereIndices = maSphere.getIndices();
    std::vector<glm::vec3>& sphereNormals = maSphere.getNormales();
    
    VAO sphereVAO;
    VBO sphereVBO(sphereVertices, sphereVertices.size());
    VBO sphereNormalVBO(sphereNormals, sphereNormals.size());
    EBO sphereEBO(sphereIndices.data(), sphereIndices.size() * sizeof(unsigned int));
    
    sphereVAO.bind();
    sphereEBO.bind();
    
    sphereVBO.bind();
    sphereVAO.linkAttrib(sphereVBO, 0);
    sphereNormalVBO.bind();
    sphereVAO.linkAttrib(sphereNormalVBO, 1);

    sphereVAO.unbind();
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    sphereEBO.unbind();

    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////

    ////////////////////// IMGUI /////////////////////////////
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
	///////////////////////////////////////////////////////////////

    ////////////////////// RENDER LOOP ////////////////////////////
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window, rayTraced);

        // render
        // ------
        GL_CHECK(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // render imgui windows
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        //////////////////////////////////////////////////////////
        
        // MATRICES EN COMMUN
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        GL_CHECK(model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)));


        // also draw the lamp object -> the smaller cube
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glm::mat4 modelLight = glm::mat4(1.0f);
        modelLight = glm::translate(modelLight, lightPos);
        modelLight = glm::scale(modelLight, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", modelLight);
        // Rendu point de lumière
        cubeVAO.bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        cubeVAO.unbind();
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //////////////////////////////////////////////////////////
        Shader* currentSphereShader; // Pointeur vers le shader à utiliser
        if (showNormalsMode) {
            currentSphereShader = &normalsShader;
        } else {
            currentSphereShader = &newShader;
        }
        GL_CHECK(currentSphereShader->use());

          // Envoyer les uniforms COMMUNES aux deux shaders
        GL_CHECK(currentSphereShader->setMat4("projection", projection));
        GL_CHECK(currentSphereShader->setMat4("view", view));
        GL_CHECK(currentSphereShader->setMat4("model", model));
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GL_CHECK(currentSphereShader->setMat3("normalMatrix", normalMatrix));

        // Envoyer les uniforms SPÉCIFIQUES au shader Lambertien (si actif)
        if (!showNormalsMode) {
            GL_CHECK(currentSphereShader->setVec3("objectColor", 0.8f, 0.8f, 0.8f));
            GL_CHECK(currentSphereShader->setVec3("lightColor",  1.0f, 1.0f, 1.0f));
            GL_CHECK(currentSphereShader->setVec3("lightPos", lightPos));
        }

        //////////////////// Surface and sphere rendering ///////////////////////
        
        maSurface.renduSurfaceBezier();

        std::vector<glm::vec3> pointsSurface = maSurface.getSurface();
        
        // std::cout << " Ma surface contient " << pointsSurface.size() << " points" << std::endl;

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        
        GL_CHECK(sphereVAO.bind());
        std::vector<unsigned int> &ind = maSphere.getIndices();
        GL_CHECK(glDrawElements(GL_TRIANGLES,
            ind.size(),
            GL_UNSIGNED_INT,
            (void*)0)
        );
        GL_CHECK(sphereVAO.unbind());
        //unbind manuel de l'ebo
        GL_CHECK(sphereEBO.unbind());
        // GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        ////////////////////Courbes, points de controles et rayons //////////////////////
        // Color Shader param
        GL_CHECK(glPointSize(10.f));
        GL_CHECK(colorShader.use());
        // view/projection transformations
        GL_CHECK(colorShader.setMat4("projection", projection));
        GL_CHECK(colorShader.setMat4("view", view));
        // world transformation
        GL_CHECK(colorShader.setMat4("model", model));
        GL_CHECK(colorShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f));

        GL_CHECK(colorShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f));
        courbe1.renduPointControl();
        courbe2.renduPointControl();

        GL_CHECK(colorShader.setVec3("objectColor", 1.0f, 0.0f, 1.0f));
        courbe1.renduCourbeBezier();
        courbe2.renduCourbeBezier();
        
        GL_CHECK(colorShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f));
        GL_CHECK(rayTraced.renduRay());

        //////////////////// IMGUI RENDERING ///////////////////////
        ///// rendu de la fenetre
        // Dans la boucle de rendu (main.cpp) :

        // Début du frame ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Créez toutes les fenêtres ImGui ici
        ImGui::Begin("Fenêtre 1");
        ImGui::SliderInt("Points Courbe 1", &courbe1.nbpoints, 2, 30);
        ImGui::SliderInt("Points Courbe 2", &courbe2.nbpoints, 2, 30);
        ImGui::End();

        ImGui::Begin("Surface Settings");
        ImGui::SliderInt("Resolution U", &maSurface.n, 2, 50);
        ImGui::SliderInt("Resolution V", &maSurface.m, 2, 50);
        ImGui::End();

        // Effectuez le rendu ImGui une seule fois
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ///////////////////////////////////////////////////////

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //// DELETE IMGUI //////
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cubeVAO.del();
    vbo.del();
    ebo.del();

    sphereVAO.del();
    sphereVBO.del();
    sphereNormalVBO.del();
    sphereEBO.del();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // === Nettoyage ===
    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, ray& rayTraced)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Partie Rayon 

    bool spaceCurrentlyPressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    if(spaceCurrentlyPressed && !spacePressedLastFrame)
    {
        std::cout << "Space clique" << std::endl;
        rayTraced.add_ray(camera.Position, camera.Front);
    }
    spacePressedLastFrame = spaceCurrentlyPressed;

    if((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)){
        std::cout << "Ray cleaned" << std::endl;
        rayTraced.clear_ray();
        rayTraced.updateGLObject();
    }

    //Partie Normale 


    bool N_CurrentlyPressed = (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS);
    if (N_CurrentlyPressed && !N_KeyPressedLastFrame) {
        showNormalsMode = !showNormalsMode; // Basculer le mode
        std::cout << "Show Normals Mode: " << (showNormalsMode ? "ON" : "OFF") << std::endl;
    }
    N_KeyPressedLastFrame = N_CurrentlyPressed;

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!shiftMode){
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void shift_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // GLFW_MOD_SHIFT; how to use that ? 
    if( (((key == GLFW_KEY_LEFT_SHIFT) || (key == GLFW_KEY_RIGHT_SHIFT)) && (action == GLFW_PRESS)))
    {
        if (!shiftMode){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            shiftMode = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            shiftMode = false;
        }   
    }
}