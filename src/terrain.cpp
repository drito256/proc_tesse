#include "../include/headers/terrain.h"
#include <linux/limits.h>


Terrain::Terrain(int terrain_res){
    
    // init grid
   /* for(int i = 0; i < terrain_res; i++){
        for(int j = 0 ; j < terrain_res ; j++){
            Vertex vertex;
            vertex.pos = glm::vec3(4.f * j / terrain_res, 0.0f,
                                   4.f * (terrain_res - i) / terrain_res);
            vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
            vertex.copy_pos = vertex.pos;
            
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
    }*/
    for(int i = 0; i < terrain_res; i++){
        for(int j = 0 ; j < terrain_res ; j++){
            Vertex vertex;
            vertex.pos = glm::vec3(4 * static_cast<float>(j) / terrain_res, 0.0f,
                                   4 * static_cast<float>(-i) / terrain_res);
            vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
            vertex.copy_pos = vertex.pos;
            
            vertices.push_back(vertex);

            if(i != terrain_res-1 && j!=terrain_res-1){
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

    glBindVertexArray(0);
}

void Terrain::render(){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::change_res(int terrain_res){

    Vertex temp = vertices.front();
    // init grid
    vertices.clear();
    indices.clear();
    for(int i = 0; i < terrain_res; i++){
        for(int j = 0 ; j < terrain_res ; j++){
            Vertex vertex;
            vertex.pos = glm::vec3(4 * static_cast<float>(j) / terrain_res, 0.0f,
                                   4 * static_cast<float>(-i) / terrain_res);
            vertex.pos.y = worley_noise(vertex.pos.x, vertex.pos.z);
            vertex.copy_pos = vertex.pos + glm::vec3(temp.copy_pos.x,0,temp.copy_pos.z);
            
            vertices.push_back(vertex);

            if(i != terrain_res-1 && j!=terrain_res-1){
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
        vertex.copy_pos.x += 0.02f;
//        vertex.copy_pos.z -= 0.01f;
        float dist = worley_noise(vertex.copy_pos.x, vertex.copy_pos.z);
        vertex.pos.y = pow(dist, 0.8f);
        vertex.pos.y = floor(vertex.pos.y * 8) / 5.f;
    }
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}


