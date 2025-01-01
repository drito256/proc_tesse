#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/headers/terrain.h"
#include "../include/headers/shader.h"
#include "../include/headers/camera.h"

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#include <iostream>
#include <cstdlib>

int width = 1280;
int height = 800;
float fov = 45.f;

int terrain_res = 100;

void processInput(GLFWwindow *window, Camera& c);
void framebuffer_size_callback(GLFWwindow* window, int w, int h){
	width = w;
	height = h;
	glViewport(0,0,width,height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	fov -= (float)yoffset;
	if(fov < 1.0f)
		fov = 1.0f;
	if(fov > 45.f)
		fov = 45.f;
}

int main(int argc, char * argv[]) {
	GLFWwindow* window;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "title", nullptr, nullptr);
	if(window == nullptr){
		std::cout << "failed to create opengl context" << std::endl;
		exit(1);
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "failed to init glad" << std::endl;
		return -1;
	}
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");

    srand(0);
    Terrain terrain;
    Shader shader("shaders/shader.vert", "shaders/shader.frag"/*, nullptr,
                  "shaders/shader.tc", "shaders/shader.te"*/);
    Camera camera(glm::vec3(-8.51, 6.45, 6.4f), -24.f, 327.f);


    glClearColor(0.1, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    bool qPressed =false, ePressed=false;
	while(!glfwWindowShouldClose(window)){

         ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (!qPressed) { // Key was just pressed
            terrain_res+=2;
            terrain.change_res(terrain_res);
            qPressed = true; // Set to true to prevent multiple reads
        }
        } else {
            qPressed = false; // Reset when the key is released
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!ePressed) { // Key was just pressed
                terrain_res-=2;
                terrain.change_res(terrain_res);
                ePressed = true; // Set to true to prevent multiple reads
            }
        } else {
            ePressed = false; // Reset when the key is released
        }
        
        processInput(window, camera);
        shader.use();            
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());
       	
        terrain.update();
        terrain.render();

        ImGui::Begin("Slider");
        if(ImGui::SliderInt("Tesselation level", &terrain_res, 0.f, 256.f)){
            terrain.change_res(terrain_res);
        }            
        
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       	glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
      
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	return 0;
}

void processInput(GLFWwindow *window, Camera& c)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        c.updatePosition(c.getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        c.updatePosition(-c.getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        c.updatePosition(-glm::normalize(glm::cross(c.getFront(), c.getUp())));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        c.updatePosition(glm::normalize(glm::cross(c.getFront(), c.getUp())));

    if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	    c.updateRotation(2.f, 0.f);
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	    c.updateRotation(-2.f ,0.f);
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	    c.updateRotation(0.f, 2.f);
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	    c.updateRotation(0.f ,-2.f);

}

