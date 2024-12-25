#version 330 core

out vec4 color;
in float height;

void main(){
    color = vec4(height, height/2.f, 0.2, 0.5);
}
