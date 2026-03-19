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

// #include <glm/gtx/string_cast.hpp>

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

#include "header/ray.hpp"
#include "header/cylindre.hpp"
#include "header/cubemap.hpp"

#include "header/utils.hpp"

// 3D Objects
#include "header/cube.hpp"

#include "header/controlPoint.hpp"
#include "header/courbeBezier.hpp"
#include "header/surfaceBezier.hpp"
#include "header/sphere.hpp"

#include "header/model.hpp"

// #include "header/glfwWindow.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void shift_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void space_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void clear_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window, ray& rayTraced);
unsigned int loadTexture(char const * path);

// screen settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool shiftMode = false;
bool spacePressedLastFrame = false;
bool showNormalsMode = false;
bool N_KeyPressedLastFrame = false;
bool W_KeyPressedLastFrame = false;

bool showWireFrame = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting

// std::vector<glm::vec3> lightPositions;
// std::vector<glm::vec3> lightColors;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightCol(150.0f, 150.0f, 150.0f);

//surface
glm::vec3 surfacePos(-1.2f, 1.0f, -2.0f);
glm::vec3 cylindrePos(-7.2f, 1.0f, -3.0f);
glm::vec3 spherePos(-7.2f, 1.0f, -3.0f);
glm::vec3 object1Pos(-7.2f, 1.0f, -3.0f);
glm::vec3 object2Pos(-7.2f, 1.0f, -3.0f);

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
    
    // courbes
    courbeBezier courbe1(liste3);
    courbeBezier courbe2(liste4);
    
    // surface
    std::vector<glm::vec3> surface = concate2list(liste1, liste2);
    surfaceBezier maSurface(surface, liste1.size(), liste2.size(), 20, 20);

    // sphere
    Sphere maSphere;

    // ray
    ray rayTraced;

    // cylinder
    Cylinder monCylindre;

    // cubes
    
    Cube lightCube;

    // /// 3D object
    Model backpack(FileSystem::getPath("object/backpack/backpack.obj"));
    // Model samurai(FileSystem::getPath("object/pbr_kabuto_samurai_helmet/scene.gltf"));
    
    
    ////////////////////// SHADERS /////////////////////////////
    
    Shader colorShader("../shaders/1.colors.vs", "../shaders/1.colors.fs");
    Shader lightCubeShader("../shaders/1.light_cube.vs", "../shaders/1.light_cube.fs");
    Shader newShader("../shaders/lighting.vs", "../shaders/lighting.fs");
    Shader normalsShader("../shaders/lighting.vs", "../shaders/normal.fs");
    
    Shader skyboxShader("../shaders/skybox.vs", "../shaders/skybox.fs");
    
    Shader reflectionShader("../shaders/lighting.vs", "../shaders/reflection.fs");
    Shader refractionShader("../shaders/lighting.vs", "../shaders/refraction.fs");
    Shader skinningShader("../shaders/skinning.vs", "../shaders/refraction.fs");
    Shader w_skinningShader("../shaders/skinning.vs", "../shaders/weight_shader.fs");

    Shader pbrShader("../shaders/pbr.vs", "../shaders/pbr.fs");
    pbrShader.use();
    pbrShader.setInt("nbLights", 1);

    pbrShader.setInt("albedoMap", 0);
    pbrShader.setInt("normalMap", 1);
    pbrShader.setInt("metallicMap", 2);
    pbrShader.setInt("roughnessMap", 3);
    pbrShader.setInt("aoMap", 4);

    // load PBR material textures
    // --------------------------
    unsigned int albedo    = loadTexture(FileSystem::getPath("textures/pbr/-ornatebrass3-bl/albedo.png").c_str());
    unsigned int normal    = loadTexture(FileSystem::getPath("textures/pbr/-ornatebrass3-bl/normal.png").c_str());
    unsigned int metallic  = loadTexture(FileSystem::getPath("textures/pbr/-ornatebrass3-bl/metallic.png").c_str());
    unsigned int roughness = loadTexture(FileSystem::getPath("textures/pbr/-ornatebrass3-bl/roughness.png").c_str());
    unsigned int ao        = loadTexture(FileSystem::getPath("textures/pbr/-ornatebrass3-bl/ao.png").c_str());

    maSphere.m_material.albedoMap = albedo;
    maSphere.m_material.normalMap = normal;
    maSphere.m_material.metallicMap = metallic;
    maSphere.m_material.roughnessMap = roughness;
    maSphere.m_material.aoMap = ao;


    //////////////////////// DATA ////////////////////////////

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

    //////////////// CUBE 

    VAO cubeVAO;
    VBO vbo(vertices, sizeof(vertices)); // utilisation du 1er constructeur
    EBO ebo(indices, sizeof(indices));

    cubeVAO.bind();
    ebo.bind();
    cubeVAO.linkAttrib(vbo);
    
    // unbind
    cubeVAO.unbind();
    vbo.unbind();
    ebo.unbind();

    
    // CubeMap - SkyBox

    std::vector<std::string> faces
    {
        FileSystem::getPath("textures/skybox/right.jpg"),
        FileSystem::getPath("textures/skybox/left.jpg"),
        FileSystem::getPath("textures/skybox/top.jpg"),
        FileSystem::getPath("textures/skybox/bottom.jpg"),
        FileSystem::getPath("textures/skybox/front.jpg"),
        FileSystem::getPath("textures/skybox/back.jpg")
    };

    Cubemap my_sky_box;
    unsigned int cubemapTexture = my_sky_box.loadCubemap(faces);

    float currentRefractionRatio = 1.00f / 1.52f;
    glm::vec3 colorRefractionRatio = glm::vec3(1.0f);

    ////////////////////// IMGUI /////////////////////////////
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    
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
        GL_CHECK(glClearColor(0.2f, 0.4f, 0.8f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        //////////////////////////////////////////////////////////
        
        // MATRICES EN COMMUN
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        GL_CHECK(model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)));

        ////////////////
        // Per shader //
        ////////////////

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glm::mat4 modelLight = glm::mat4(1.0f);
        modelLight = glm::translate(modelLight, lightPos);
        modelLight = glm::scale(modelLight, glm::vec3(0.2f));
        lightCube.setModelMatrix(modelLight);

        lightCube.Draw(lightCubeShader);


        pbrShader.use();
        pbrShader.setMat4("projection", projection);
        pbrShader.setMat4("view", view);
        pbrShader.setVec3("camPos", camera.Position);
        pbrShader.setVec3("lightPositions[0]", lightPos);
        pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
        pbrShader.setVec3("lightColors[0]", lightCol);
        
        maSphere.setModelMatrix(model);
        maSphere.Draw(pbrShader);

        glm::mat4 my_modelMatrix = glm::mat4(1.0f);
        my_modelMatrix = glm::translate(my_modelMatrix, glm::vec3(0.0f, -1.0f, 10.0f)); 
        my_modelMatrix = glm::scale(my_modelMatrix, glm::vec3(0.5f)); 

        backpack.setModelMatrix(my_modelMatrix);
        backpack.Draw(pbrShader);
        // samurai.Draw(pbrShader);

        ///////////////////// dessin de la sphère /////////////////////////
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
        // Envoyer les uniforms SPÉCIFIQUES au shader Lambertien (si actif)
        if (!showNormalsMode) {
            GL_CHECK(currentSphereShader->setVec3("objectColor", 0.8f, 0.8f, 0.8f));
            GL_CHECK(currentSphereShader->setVec3("lightColor",  1.0f, 1.0f, 1.0f));
            GL_CHECK(currentSphereShader->setVec3("lightPos", lightPos));
        }

        //////////////////// sphere rendering ///////////////////////
        
        // Envoyer les uniforms COMMUNES aux deux shaders
        GL_CHECK(currentSphereShader->setMat4("projection", projection));
        GL_CHECK(currentSphereShader->setMat4("view", view));
        GL_CHECK(currentSphereShader->setMat4("model", model));
        
        // Envoyer les uniforms SPÉCIFIQUES au shader Lambertien
        if (!showNormalsMode) {
            GL_CHECK(currentSphereShader->setVec3("objectColor", 0.8f, 0.8f, 0.8f));
            GL_CHECK(currentSphereShader->setVec3("lightColor",  1.0f, 1.0f, 1.0f));
            GL_CHECK(currentSphereShader->setVec3("lightPos", lightPos));
        }

        //décalage de la surface
        glm::mat4 modelSurface = glm::mat4(1.0f);
        modelSurface = glm::translate(modelSurface, surfacePos);
        currentSphereShader->setMat4("model", modelSurface);
        
        maSurface.renduSurfaceBezier();

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        currentSphereShader->setMat4("model", modelMatrix); 
        
        // CYLINDRE
        Shader* currentShader = &reflectionShader;

        // currentShader = &refractionShader;
        currentShader = &w_skinningShader;
        currentShader = &skinningShader;
        
        currentShader->use();
        GL_CHECK(currentShader->setVec3("cameraPos", camera.Position));
        GL_CHECK(currentShader->setFloat("refractionRatio", currentRefractionRatio));
        GL_CHECK(currentShader->setVec3("color", colorRefractionRatio));
        
        // décalage du cylindre
        modelSurface = glm::mat4(1.0f);
        modelSurface = glm::translate(modelSurface, -cylindrePos);
        
        monCylindre.setModelMatrix(modelSurface);
        GL_CHECK(currentShader->setMat4("projection", projection));
        GL_CHECK(currentShader->setMat4("view", view));

        float time = static_cast<float>(glfwGetTime());

        glm::mat4 bone0 = glm::mat4(1.0f);
        glm::mat4 bone1 = glm::mat4(1.0f);

        float articulationY = 0.0f;

        bone1 = glm::translate(bone1, glm::vec3(0.0f, articulationY, 0.0f));

        bone1 = glm::rotate(bone1, sin(time) * 1.5f, glm::vec3(1.0f, 0.0f, 0.0f)); 
        bone1 = glm::translate(bone1, glm::vec3(0.0f, -articulationY, 0.0f));

        currentShader->setMat4("finalBonesMatrices[0]", bone0);
        currentShader->setMat4("finalBonesMatrices[1]", bone1);

        glActiveTexture(GL_TEXTURE0);
        my_sky_box.bind();
        currentShader->setInt("skybox", 0);

        monCylindre.Draw(*currentShader);


        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////Courbes, points de controles et rayons /////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        // Color Shader param
        GL_CHECK(glPointSize(10.f));
        GL_CHECK(colorShader.use());
        // view/projection transformations
        GL_CHECK(colorShader.setMat4("projection", projection));
        GL_CHECK(colorShader.setMat4("view", view));
        // world transformation
        GL_CHECK(colorShader.setMat4("model", model));
        GL_CHECK(colorShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f));

        GL_CHECK(colorShader.setVec3(" ", 1.0f, 1.0f, 0.0f));
        courbe1.renduPointControl();
        courbe2.renduPointControl();

        GL_CHECK(colorShader.setVec3("objectColor", 1.0f, 0.0f, 1.0f));
        courbe1.renduCourbeBezier();
        courbe2.renduCourbeBezier();
        
        GL_CHECK(colorShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f));
        GL_CHECK(rayTraced.renduRay());

        /////////////////////
        // Rendu de la skybox 
        /////////////////////

        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();

        glm::mat4 view1 = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
        
        skyboxShader.setMat4("view", view1);
        skyboxShader.setMat4("projection", projection);
        
        cubeVAO.bind();

        my_sky_box.bind();
        glActiveTexture(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        cubeVAO.unbind();
        // GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
        glDepthFunc(GL_LESS);

        //Fin du rendu de la skybox

        ///////////////////////////////////////////////////
        //////////////////// IMGUI  ///////////////////////
        ///////////////////////////////////////////////////

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

        ImGui::Begin("Propriétés des Matériaux");
        ImGui::SliderFloat("Ratio Réfraction", &currentRefractionRatio, 0.42f, 1.0f);
        ImGui::ColorEdit3("Couleur refraction",glm::value_ptr(colorRefractionRatio));
        ImGui::End();

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

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // === Nettoyage ===
    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}

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
        rayTraced.verifCollisions(glm::vec3(0.0f), 1.0f);
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
    
    // partie wireframe
    bool W_CurrentlyPressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    if (W_CurrentlyPressed && !W_KeyPressedLastFrame) {
        showWireFrame = !showWireFrame; // Basculer le mode
        std::cout << "Show WireFrame Mode: " << (showWireFrame ? "ON" : "OFF") << std::endl;
    }
    W_KeyPressedLastFrame = W_CurrentlyPressed;
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
