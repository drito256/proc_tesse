#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "../glad/glad.h"

#include "object.h"
#include "shader.h"

class Renderer{
	public:
		Renderer();
		void addObject(const Object object);
		void renderScene(std::vector<Shader> &shaders);

	private:
		std::vector<Object> objects;
};

#endif
