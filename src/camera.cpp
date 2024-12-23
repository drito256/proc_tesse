#include "../include/headers/camera.h"
#include <iostream>

Camera::Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f), 
	       float yaw = -90.f, float pitch = 0.f){
	this->pos = pos;
	this->yaw = yaw;
	this->pitch = pitch;
	this->speed = 0.1f;
	this->zoom = 45.f;
	updateVectors();
}


glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(this->pos, this->pos + this->front, this->up);
	//return this->myglm.lookAtMatrix(this->pos, 
	//				  this->pos + this->front,
	//				  this->up);
}

glm::mat4 Camera::getProjectionMatrix(){
	return glm::perspective(glm::radians(this->zoom),
		       	1280.f/800.f , 0.1f, 100.f);
	//return this->myglm.frustum();
}

void Camera::updatePosition(glm::vec3 pos){
	this->pos += (pos * this->speed);
	std::cout << this->pos.x << " " <<this->pos.y << " " << this->pos.z << std::endl;
}

void Camera::updateRotation(float pitch, float yaw){
	this->yaw += (yaw * this->speed * 3);
	this->pitch += (pitch * this->speed * 3);

	updateVectors();
}

void Camera::updateZoom(float zoom){
	this->zoom = zoom;
}

float Camera::getYaw(){
	return this->yaw;
}
float Camera::getPitch(){
	return this->pitch;
}

glm::vec3 Camera::getFront(){
	return this->front;
}
glm::vec3 Camera::getUp(){
	return this->up;
}

void Camera::updateVectors(){
	this->front.x =cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front.y =sin(glm::radians(this->pitch));
        this->front.z =sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front =glm::normalize(this->front);

        this->right =glm::normalize(glm::cross(this->front, glm::vec3(0.f,1.f,0.f)));
        this->up = glm::normalize(glm::cross(this->right, this->front));
	
}
