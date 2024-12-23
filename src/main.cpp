#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

//nekima mozda ne radi primjerASSIMP zbog ponovnih definicija stbi funkcija.
//Jedno od mogucih rjesenja je da se zakomentira linija #define STB_IMAGE_IMPLEMENTATION.
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#include "../include/headers/shader.h"
#include "../include/headers/renderer.h"
#include "../include/headers/camera.h"


// Standard Headers
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

	glClearColor(0.15, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);
    
    std::vector<Shader> shaders;
	Shader shader1("shaders/myshader.vs", "shaders/myshader.fs", "shaders/myshader.gs");
	Shader shader2("shaders/myshader2.vs", "shaders/myshader2.fs", "shaders/myshader2.gs");
    shaders.push_back(shader1);
    shaders.push_back(shader2);

	Camera camera(glm::vec3(0.f,1.f,5.f), -90.f, 0.f);

	//kocka 1
	Object o("../resources/other/frog.obj");
	o.scale(glm::vec3(0.55f));	

	Object o2("../resources/other/frog.obj");
	o2.scale(glm::vec3(0.25f));
	o2.globalMove(glm::vec3(0.8f,0.8f,0.f));

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(60.f),
		       	        	       glm::vec3(1.0f,0.0f,0.0f));
	o2.rotate(rotationMatrix);
	Renderer r;

	r.addObject(o);
	r.addObject(o2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		processInput(window, camera);
		camera.updateZoom(fov);
        
        shader1.use();
		shader1.setMat4("view", camera.getViewMatrix());
		shader1.setMat4("projection", camera.getProjectionMatrix());
        shader1.setVec3("eye", camera.getPosition());
        shader2.use();
        shader2.setMat4("view", camera.getViewMatrix());
		shader2.setMat4("projection", camera.getProjectionMatrix());

        std::vector<Shader> s{shader1,shader2};
		r.renderScene(s);


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
