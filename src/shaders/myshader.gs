#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;

void main(){
    
    vec3 side1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 side2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    vec3 normal = normalize(cross(side1, side2));

    vec3 viewDirection = -gl_in[0].gl_Position.xyz;
//    vec4 eyePosition = vi;

    float dotProduct = dot(normal, viewDirection);

    if (dotProduct <= 0.0) {
        return;
    }

    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = projection * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
