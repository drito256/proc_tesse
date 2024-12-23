#include "../include/headers/MyGLM.h"


glm::mat4 MyGLM::translate3D(glm::vec3 vec){
	glm::mat4 mat;

	mat = {1, 0, 0 , vec.x, 0, 1 ,0, vec.y, 0, 0, 1, vec.z, 0, 0 , 0 ,1};

	return mat;

}

glm::mat4 MyGLM::scale3D(glm::vec3 vec){
	glm::mat4 mat;

	mat = {vec.x ,0, 0, 0, 0, vec.y, 0, 0, 0, 0, vec.x, 0, 0, 0, 0, 1};

	return mat;
}

glm::mat4 MyGLM::rotate3D(glm::vec3 axis, float angle) {
    axis = glm::normalize(axis);
    
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    float oneMinusCosTheta = 1.0f - cosTheta;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;

    glm::mat4 rotationMatrix;
    rotationMatrix[0][0] = cosTheta + xx * oneMinusCosTheta;
    rotationMatrix[0][1] = xy * oneMinusCosTheta - z * sinTheta;
    rotationMatrix[0][2] = xz * oneMinusCosTheta + y * sinTheta;
    rotationMatrix[0][3] = 0.0f;

    rotationMatrix[1][0] = xy * oneMinusCosTheta + z * sinTheta;
    rotationMatrix[1][1] = cosTheta + yy * oneMinusCosTheta;
    rotationMatrix[1][2] = yz * oneMinusCosTheta - x * sinTheta;
    rotationMatrix[1][3] = 0.0f;

    rotationMatrix[2][0] = xz * oneMinusCosTheta - y * sinTheta;
    rotationMatrix[2][1] = yz * oneMinusCosTheta + x * sinTheta;
    rotationMatrix[2][2] = cosTheta + zz * oneMinusCosTheta;
    rotationMatrix[2][3] = 0.0f;

    rotationMatrix[3][0] = 0.0f;
    rotationMatrix[3][1] = 0.0f;
    rotationMatrix[3][2] = 0.0f;
    rotationMatrix[3][3] = 1.0f;

    return rotationMatrix;
}

glm::mat4 MyGLM::lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp) {
    glm::vec3 forward = glm::normalize(center - eye);
    glm::vec3 right = glm::normalize(glm::cross(forward, viewUp));
    glm::vec3 up = glm::cross(right, forward);

    glm::mat4 viewMatrix(1.0f);
    viewMatrix[0][0] = right.x;
    viewMatrix[1][0] = right.y;
    viewMatrix[2][0] = right.z;
    viewMatrix[0][1] = up.x;
    viewMatrix[1][1] = up.y;
    viewMatrix[2][1] = up.z;
    viewMatrix[0][2] = -forward.x;
    viewMatrix[1][2] = -forward.y;
    viewMatrix[2][2] = -forward.z;
    viewMatrix[3][0] = -glm::dot(right, eye);
    viewMatrix[3][1] = -glm::dot(up, eye);
    viewMatrix[3][2] = glm::dot(forward, eye);

    return viewMatrix;
}

glm::mat4 frustum(float l, float r, float b, float t, float n, float f) {
    glm::mat4 projectionMatrix(0.0f); 

    projectionMatrix[0][0] = 2.0f * n / (r - l);
    projectionMatrix[1][1] = 2.0f * n / (t - b);
    projectionMatrix[2][0] = (r + l) / (r - l);
    projectionMatrix[2][1] = (t + b) / (t - b);
    projectionMatrix[2][2] = -(f + n) / (f - n);
    projectionMatrix[2][3] = -1.0f;
    projectionMatrix[3][2] = -(2.0f * f * n) / (f - n);

    return projectionMatrix;
}
