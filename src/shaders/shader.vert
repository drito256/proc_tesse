#version 330 core

layout (location = 0) in vec3 pos;
    
uniform mat4 projection;
uniform mat4 view;
out float height;

void main(){
    gl_Position = projection * view * vec4(pos, 1.0);
    height = pos.y;
}
