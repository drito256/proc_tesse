#include "../include/headers/renderer.h"

void Renderer::addObject(const Object object){
	objects.push_back(object);
}

void Renderer::renderScene(std::vector<Shader> &shaders) {
    	for (int i = 0; i < objects.size(); i++) {
            Shader current_shader = shaders.at(i);
            current_shader.use();
    	    current_shader.setMat4("model", objects[i].getModelMatrix());

        	for (int j = 0; j < objects[i].getMeshes().size(); j++) {
            		Mesh mesh = objects[i].getMeshes()[j];
            		glBindVertexArray(mesh.getVAO());  

            		glDrawElements(GL_TRIANGLES, mesh.getIndices().size(),
				       GL_UNSIGNED_INT, 0);
            		glBindVertexArray(0); // Unbind VAO
        }
    }
        glUseProgram(0); // reset shader
}


Renderer::Renderer(){
}
