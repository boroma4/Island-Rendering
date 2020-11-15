#pragma once

#include "../utils/shader_util.h"
#include "../utils/texture_util.h"

class skybox_entity
{
public:

    ~skybox_entity();
    void init(shader_prog* shader);
    void draw(glm::mat4 perspective, glm::mat4 view);

private:


    shader_prog* shader;
    unsigned int texture;
    unsigned int skybox_vao;
};