#include "../include/headers/terrain.h"



Terrain::Terrain(){
    
    // init grid
    for(int i = -50; i < 50; i++){
        for(int j = -50 ; j < 50 ; j++){
            Vertex vertex;
            vertex.pos = glm::vec3(5 * j / 100.f, 0.0f, 5 * (100 - i) / 100.f);
            vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.tex_coords = glm::vec2(j / 100.f, i / 100.f);
            vertices.push_back(vertex);
        }
    }

    for(int i = 0; i < 100 - 1; i++){
        for(int j = 0 ; j < 100 - 1; j++){
            unsigned int top_left = (i + 1) * 100 + j;
            unsigned int top_right = top_left + 1;
            unsigned int bottom_left = (i * 100) + j;
            unsigned int bottom_right = bottom_left + 1;

            indices.push_back(top_left);
            indices.push_back(bottom_left);
            indices.push_back(top_right);


            indices.push_back(top_right);
            indices.push_back(bottom_left);
            indices.push_back(bottom_right);
        }
    }
     
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
}

void Terrain::apply_noise(){
    for (auto& vertex : vertices){
        vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
        std::cout << vertex.pos.x << std::endl;
    }
}
void Terrain::render(){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
