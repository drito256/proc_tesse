#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/headers/terrain.h"
#include "../include/headers/shader.h"
#include "../include/headers/camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#include <iostream>
#include <cstdlib>

int width = 1280;
int height = 800;
float fov = 45.f;

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    srand(0);
    Terrain terrain;
    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    Camera camera(glm::vec3(-8.51, 6.45, 6.4f), -24.f, 327.f);


    glClearColor(0.1, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
        processInput(window, camera);
        shader.use();            
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());
       	
        terrain.update();
        terrain.render();

       	glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

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
