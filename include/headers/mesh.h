#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <vector>
#include <string>
#include "../glad/glad.h"

struct Vertex{
	glm::vec4 pos;
	glm::vec3 norm;
};

class Mesh{
	public:
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		Mesh(std::vector<Vertex> vertices, std::vector<int> indices);
		std::vector<int> getIndices();
		unsigned int getVAO();
		std::pair<glm::vec3, glm::vec3> getBoundingBox();
		void applyTransform(glm::mat4 mat);

	private:
		unsigned int vao, vbo, ebo;
		void setupMesh();
};
		     
	
