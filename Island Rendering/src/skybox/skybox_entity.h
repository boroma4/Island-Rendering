#pragma once

#include "../utils/shader_util.h"
#include "../utils/texture_util.h"
//#include <GLFW/glfw3.h>

class skybox_entity
{
public:

    ~skybox_entity();
    void init(shader_prog* shader);
    void draw(glm::mat4 perspective, glm::mat4 view);
    void changeSkybox(int change);

private:

    std::vector<GLuint> textureHandles;
    unsigned int skyboxId;
    shader_prog* shader;
    unsigned int skybox_vao;
};