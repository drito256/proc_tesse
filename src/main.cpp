#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/headers/terrain.h"
#include "../include/headers/shader.h"
#include "../include/headers/camera.h"

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#include <iostream>
#include <cstdlib>

int width = 1280;
int height = 800;
float fov = 45.f;

int terrain_res = 100;

void processInput(GLFWwindow *window, Camera& c);
void imGuiInit();
void imGuiDisplay(Terrain *terrain, float *func_mod, glm::vec3 *bottom_color, glm::vec3 *top_color);

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
    Terrain terrain(100);

    float func_mod = 5.f;
    glm::vec3 bottom_color{0}, top_color{0};

    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    Camera camera(glm::vec3(-4.51, 5.45, 0.4f), -24.f, 327.f);


    glClearColor(0.1, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    bool qPressed =false, ePressed=false;
//    auto start = std::chrono::high_resolution_clock::now();

	while(!glfwWindowShouldClose(window)){

        imGuiInit();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
/*        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count();
        terrain_res = 102 + sin(elapsed/2000.f)*100; //ovo maknuti
        terrain.change_res(terrain_res); // ovo maknuti*/
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (!qPressed) {
            terrain_res+=2;
            terrain.change_res(terrain_res);
            qPressed = true;
        }
        } else {
            qPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!ePressed) {
                terrain_res-=2;
                terrain.change_res(terrain_res);
                ePressed = true;
            }
        } else {
            ePressed = false;
        }
        
        processInput(window, camera);
        shader.use();            
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());
        
        shader.setVec3("bottom_color", bottom_color);
        shader.setVec3("top_color", top_color);
        terrain.update(func_mod);
        terrain.render();

        imGuiDisplay(&terrain, &func_mod, &bottom_color, &top_color);
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

void imGuiInit(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imGuiDisplay(Terrain *terrain, float *func_mod, glm::vec3 *bottom_color, glm::vec3 *top_color){
        static bool button_status = true;

        float* bottom_ptr = glm::value_ptr(*bottom_color);
        float* top_ptr = glm::value_ptr(*top_color);
        ImGui::Begin("  ");
        if(ImGui::SmallButton("Wireframe Mode")){
            button_status = !button_status;
            if(button_status){
	            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else{
	            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
        ImGui::NewLine();
        ImGui::SliderFloat("Modify", func_mod, 2, 20);
        ImGui::ColorEdit3("Bottom color", bottom_ptr);
        ImGui::ColorEdit3("Top color", top_ptr);

        ImGui::End();

        ImGui::Begin(" ");
        if(ImGui::VSliderInt(" ",ImVec2(20, 500), &terrain_res, 0, 202)){
            terrain->change_res(terrain_res);
        }            
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

