#include "../include/headers/transform.h"

void Transform::setPosition(glm::vec3 pos){
	this->pos = pos;
}

glm::vec3 Transform::getPosition(){
    return this->pos;
}

void Transform::localMove(glm::vec3 trans){
	this->pos += trans.x * right + trans.y * up + trans.z * front;
}

void Transform::globalMove(glm::vec3 trans){
	this->pos += trans;
}

glm::mat4 Transform::getModelMatrix(){
	glm::mat4 mat = glm::mat4(1.0f);

	glm::mat4 rot = glm::mat4(glm::vec4(this->right,0.0f),
			glm::vec4(this->up,0.0f),
			glm::vec4(-this->front,0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	
//	mat = this->myglm.translate3D(pos);
	mat = glm::translate(mat, this->pos);
//	mat = this->myglm.rotate3D();
	mat = mat * rot;
	//mat = this->myglm.scale3D(glm::vec3(scale_factor));
	mat = glm::scale(mat, this->scale_factor);

	return mat;
}

Transform::Transform(){
	this->pos = glm::vec3(0.f);
	this->scale_factor = glm::vec3(1.f);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
}


void Transform::rotate(glm::mat4 mat){
	glm::mat3 rotation = glm::mat3(mat);

	up = glm::normalize(rotation * up);
	right = glm::normalize(rotation * right);
	front = glm::normalize(rotation * front);
}

void Transform::scale(glm::vec3 factor){
	this->scale_factor = factor;

}

void Transform::setOrientation(glm::vec3 new_up, glm::vec3 new_right, glm::vec3 new_front){
	this->up = glm::normalize(new_up);
	this->right = glm::normalize(new_right);
	this->front = glm::normalize(new_front);

	right = glm::normalize(glm::cross(front, up));
	front = glm::normalize(glm::cross(up, right));
}
