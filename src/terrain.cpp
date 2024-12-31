#include "../include/headers/terrain.h"
#include <linux/limits.h>


Terrain::Terrain(){
    
    // init grid
    for(int i = -100; i < 100; i+=1){
        for(int j = -100 ; j < 100 ; j+=1){
            Vertex vertex;
            vertex.pos = glm::vec3(2 * j / 100.f, 0.0f, 2 * (100 - i) / 100.f);
            vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
            vertex.copy_pos = vertex.pos;
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.tex_coords = glm::vec2(j / 100.f, i / 100.f);
            
            vertices.push_back(vertex);
        }
    }
     

    for(int i = 0; i < 200/1 - 1; i++){
        for(int j = 0 ; j < 200/1 - 1; j++){
            unsigned int top_left = (i + 1) * 200/1 + j;
            unsigned int top_right = top_left + 1;
            unsigned int bottom_left = (i * 200/1) + j;
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

void Terrain::render(){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::change_res(int terrain_res){

    Vertex temp = vertices.front();
    std::cout << temp.copy_pos.x << std::endl;
    std::cout << temp.copy_pos.z << std::endl;
    // init grid
    vertices.clear();
    indices.clear();
    for(float i = 0; i < terrain_res; i++){
        for(float j = 0 ; j < terrain_res ; j++){
            Vertex vertex;
            vertex.pos = glm::vec3(4 * j / terrain_res, 0.0f, 4 *- i / terrain_res);
            vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
            //vertex.copy_pos = vertex.pos + glm::vec3(counter*0.02f,0,-counter*0.02f);
            vertex.copy_pos = vertex.pos + glm::vec3(temp.copy_pos.x,0,temp.copy_pos.z);
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.tex_coords = glm::vec2(j / 100.f, i / 100.f);
            
            vertices.push_back(vertex);
        }
    }
     

    for(int i = 0; i < terrain_res - 1; i++){
        for(int j = 0 ; j < terrain_res - 1; j++){
            unsigned int top_left = (i + 1) * terrain_res + j;
            unsigned int top_right = top_left + 1;
            unsigned int bottom_left = (i * terrain_res) + j;
            unsigned int bottom_right = bottom_left + 1;

            indices.push_back(top_left);
            indices.push_back(bottom_left);
            indices.push_back(top_right);


            indices.push_back(top_right);
            indices.push_back(bottom_left);
            indices.push_back(bottom_right);
        }
    }
     
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

}

void Terrain::update(){
    for(auto& vertex : vertices){
        vertex.copy_pos.x += 0.01f;
        vertex.copy_pos.z -= 0.01f;
        float dist = worley_noise(vertex.copy_pos.x, vertex.copy_pos.z);
        vertex.pos.y = pow(dist, 0.8f);
        vertex.pos.y = std::floor(vertex.pos.y * 8) / 5;
    }
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}


