#include "../include/headers/mesh.h"

void Mesh::setupMesh(){
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],			  GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), 
	             &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, norm));

	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices){
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
	
	glm::mat4 mat(1.0f);
	std::pair<glm::vec3, glm::vec3> mm = getBoundingBox();
	float xavg = (mm.first.x + mm.second.x) / 2.f;
	float yavg = (mm.first.y + mm.second.y) / 2.f;
	float zavg = (mm.first.z + mm.second.z) / 2.f;
	glm::vec3 move = glm::vec3(-xavg, -yavg, -zavg);

	float x_dif = mm.second.x - mm.first.x;
	float y_dif = mm.second.y - mm.first.y;
	float z_dif = mm.second.z - mm.first.z;

	float max_dif = x_dif;
	if(y_dif > max_dif)
		max_dif = y_dif;
	if(z_dif > max_dif)
		max_dif = z_dif;

	glm::vec3 enlarge = glm::vec3(2.f / max_dif, 2.f / max_dif,2.f /  max_dif);
	mat = glm::scale(mat, enlarge);
	mat = glm::translate(mat, move);
	//mat = glm::scale(mat, enlarge);
	applyTransform(mat);	
}

std::vector<int> Mesh::getIndices(){
	return this->indices;
}

std::pair<glm::vec3, glm::vec3> Mesh::getBoundingBox(){
	float xmin = vertices[0].pos.x, xmax = vertices[0].pos.x;
	float ymin = vertices[0].pos.y, ymax = vertices[0].pos.y;
	float zmin = vertices[0].pos.z, zmax = vertices[0].pos.z;
	
	std::pair<glm::vec3, glm::vec3> res;

	for(int i=0;i<vertices.size();i++){
		if(vertices[i].pos.x < xmin){
			xmin = vertices[i].pos.x;
		}
		if(vertices[i].pos.x > xmax){
			xmax = vertices[i].pos.x;
		}
		
		if(vertices[i].pos.y < ymin){
			ymin = vertices[i].pos.y;
		}
		if(vertices[i].pos.y > ymax){
			ymax = vertices[i].pos.y;
		}
		
		if(vertices[i].pos.z < zmin){
			zmin = vertices[i].pos.z;
		}
		if(vertices[i].pos.z > zmax){
			zmax = vertices[i].pos.z;
		}
	}

	res.first = glm::vec3(xmin, ymin, zmin);
	res.second = glm::vec3(xmax, ymax, zmax);

	return res;
}

unsigned int Mesh::getVAO(){
	return this->vao;
}


void Mesh::applyTransform(glm::mat4 mat){
	for(int i =0;i<vertices.size();i++){
		vertices[i].pos = mat * vertices[i].pos;
	}
	setupMesh();

}
