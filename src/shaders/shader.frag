#version 330 core

out vec4 color;
in float height;

void main(){
    color = vec4(pow(height,2), pow(height,2), height, 1);
}
