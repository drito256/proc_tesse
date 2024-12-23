#include "../include/headers/object.h"


std::vector<Mesh> Object::getMeshes(){
	return this->meshes;
}

Mesh Object::processMesh(aiMesh *mesh, const aiScene *scene){
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	for(int i=0;i<mesh->mNumVertices;i++){
		Vertex vertex;
		glm::vec4 vec;
		
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vec.w = 1.f;
		vertex.pos = vec;

		if(mesh->HasNormals()){
			vec.x = mesh->mNormals[i].x;
			vec.y = mesh->mNormals[i].y;
			vec.z = mesh->mNormals[i].z;
			vertex.norm = vec;
		}

		vertices.push_back(vertex);
	}

	for(int i=0;i<mesh->mNumFaces;i++){
		aiFace face = mesh->mFaces[i];

		for(int j=0;j<face.mNumIndices;j++)
			indices.push_back(face.mIndices[j]);
	}
	std::cout << indices.size() << std::endl;
	return Mesh(vertices, indices);
}

void Object::processNode(aiNode *node, const aiScene *scene){
	for(int i=0;i<node->mNumMeshes;i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for(int i =0;i<node->mNumChildren;i++){
		processNode(node->mChildren[i], scene);
	}

}


void Object::loadObject(std::string path){
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cout << "ERR Assimp" << import.GetErrorString() << std::endl;
		return;
	}

	dir = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);

}



