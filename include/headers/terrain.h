#ifndef TERRAIN_H
#define TERRAIN_H

#include "../glm/glm.hpp"
#include "../glad/glad.h"
#include "noise.h"

#include <vector>
#include <iostream>
#include <random>

struct Vertex{
    glm::vec3 pos;
    glm::vec3 copy_pos;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

class Terrain{
    public:
        Terrain();
        void apply_noise();
        void render();
        void update();
        void change_res(int terrain_res);

    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;
};

#endif
