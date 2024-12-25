#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"


class Transform{
	public:
		glm::mat4 getModelMatrix();
		void rotateFPS(float x_off, float y_off, bool constrain_pitch);
		void rotate(glm::mat4 mat);
		void globalMove(glm::vec3 trans);
		void localMove(glm::vec3 trans);
		void setOrientation(glm::vec3 new_up, glm::vec3 new_right, 
				    glm::vec3 new_front);
		void setPosition(glm::vec3 pos);
        glm::vec3 getPosition();
		void scale(glm::vec3 factor);
		void update(float delta);
		Transform();

	protected:
		glm::vec3 pos;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 scale_factor;

};

#endif
