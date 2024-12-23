#ifndef MYGLM_H
#define MYGLM_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

class MyGLM{
	public:
		glm::mat4 translate3D(glm::vec3 translate_vector);
		glm::mat4 scale3D(glm::vec3 scale_vector);
		glm::mat4 rotate3D(glm::vec3 axis, float angle);
		glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center,
					glm::vec3 viewUp);
		glm::mat4 frustum(float l, float r, float b, 
				  float t, float n, float f);
};

#endif

