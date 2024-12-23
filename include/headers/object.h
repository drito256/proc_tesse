#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "transform.h"
#include "shader.h"

class Object : public Transform{
	public:
        Object(std::string path){
            loadObject(path);
        }
		std::vector<Mesh> getMeshes();
	private:
		std::vector<Mesh> meshes;
		std::string dir;

		void loadObject(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};

#endif
