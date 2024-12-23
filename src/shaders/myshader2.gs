#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main(){

    vec2 point1 = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
    vec2 point2 = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
    vec2 point3 = gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;

    float orientation = (point2.x - point1.x) * (point3.y - point1.y) - (point3.x - point1.x) * (point2.y - point1.y);
    
    
   if(orientation > 0.0) {
        for (int i = 0; i < gl_in.length(); ++i) {
            gl_Position = gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
   }
}
