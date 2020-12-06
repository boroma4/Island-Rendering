#include "skybox_entity.h"


skybox_entity::~skybox_entity()
{
	this->shader->free();
}

//right now I am doing something wrongly and the method doesn't give me the expected results. 

//Maybe storing skybox textures would be better done as uniform variables in the fragment shader. 
//This would also maybe eliminate the need to call glActiveTexture in the changeSkybox method to not break the water.

std::vector<GLuint> loadCubemaps()              //the method is supposed to return a vector of unsigned integers that can be used to change the skybox texture.
{
    std::vector<std::string> faces = std::vector<std::string>();
    std::vector<GLuint> textures = std::vector<GLuint>();               

    faces.push_back("res/default/front.jpg");               //sunny cloudy
    faces.push_back("res/default/back.jpg");
    faces.push_back("res/default/top.jpg");
    faces.push_back("res/default/bottom.jpg");
    faces.push_back("res/default/left.jpg");
    faces.push_back("res/default/right.jpg");

/*    faces.push_back("res/sunset/1/front.jpg");              //sunset 1
    faces.push_back("res/sunset/1/back.jpg");
    faces.push_back("res/sunset/1/top.jpg");
    faces.push_back("res/sunset/1/bottom.jpg");
    faces.push_back("res/sunset/1/left.jpg");
    faces.push_back("res/sunset/1/right.jpg");

    faces.push_back("res/sunset/2/front.jpg");              //sunset 2
    faces.push_back("res/sunset/2/back.jpg");
    faces.push_back("res/sunset/2/top.jpg");
    faces.push_back("res/sunset/2/bottom.jpg");
    faces.push_back("res/sunset/2/left.jpg");
    faces.push_back("res/sunset/2/right.jpg");
    
    faces.push_back("res/clear sky/front.jpg");             //clear
    faces.push_back("res/clear sky/back.jpg");
    faces.push_back("res/clear sky/top.jpg");
    faces.push_back("res/clear sky/bottom.jpg");
    faces.push_back("res/clear sky/left.jpg");
    faces.push_back("res/clear sky/right.jpg");
  */

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int minus = 0;

    for (unsigned int i = 0; i < faces.size(); i++)                     // right now the textures get overwritten on each iteration.
    {
       // printf(faces[i].c_str());
        load_texture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i - minus, GL_RGBA, faces[i].c_str());
        if ((i+1) % 6 == 0 && i != 0) {
            textures.push_back(textureID);
            textureID = 0;
            glGenTextures(1, &textureID);
            minus += 6;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textures;
}



void skybox_entity::init(shader_prog* shader)
{
    this->textureHandles = loadCubemaps();
    this->skyboxId = 1;
    this->skybox_vao = 0;
    unsigned int skyboxVBO = 0;
	this->shader = shader;

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &skybox_vao);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skybox_vao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    this->shader->use();
    this->shader->uniform1i("cubeMap", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandles[0]);
}

void skybox_entity::changeSkybox(int change)
{
    this->skyboxId += change;

    if (skyboxId > textureHandles.size()) {
        this->skyboxId = 1;
    }
    else if (1 > skyboxId){
        this->skyboxId = textureHandles.size();
    }
   // glActiveTexture(GL_TEXTURE0);   //right now this is needed to change the skybox texture, but it also breaks the water texture. 
    this->shader->use();
    this->shader->uniform1i("cubeMap", 0);
    printf("%d\n", textureHandles[skyboxId - 1]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureHandles[skyboxId - 1]);
}


void skybox_entity::draw(glm::mat4 perspective, glm::mat4 view) {

    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    this->shader->activate();
    this->shader->uniformMatrix4fv("projectionMatrix", perspective);
    this->shader->uniformMatrix4fv("viewMatrix", glm::mat4(glm::mat3(view)));

    glBindVertexArray(skybox_vao);
   // glActiveTexture(GL_TEXTURE0);
  //  glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureHandles[skyboxId - 1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}