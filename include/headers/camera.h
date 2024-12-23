#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "../include/GLFW/glfw3.h"

class Camera : public Transform{
	public:
		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();
		void updateZoom(float zoom);
		void updatePosition(glm::vec3 pos);
		void updateRotation(float pitch, float yaw);
		glm::vec3 getFront();
		glm::vec3 getUp();
		float getPitch();
		float getYaw();
		Camera(glm::vec3 pos, float yaw,
		       float pitch);

	private:
		float zoom;
		float pitch, yaw;
		float speed;
		void updateVectors();

};
#endif
