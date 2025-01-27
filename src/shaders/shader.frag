#version 330 core

out vec4 color;
in float height;

uniform vec3 bottom_color;
uniform vec3 top_color;
void main(){
    //color = vec4(pow(height,2), pow(height,2), height, 1);
    color = vec4(mix(bottom_color, top_color, pow(height,2)),1);
}
