//#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include <string>
//#include "ProgramData.h"
#include "Asset.h"
#include "../../utils/shader_util.h"

//extern ProgramData ProgramContext;

HillPlane::HillPlane(float w, float h, float l, unsigned int seed, shader_prog* shader)
	:HeightMap(256, 180, 7, 20, 10, 30, 150, seed),
	position(0.0, -9.0, 0.0),
	rotationAxis(0.0, 1.0, 0.0),
	rotationDegrees(0.0),
	scaling(w, h, l)
{
	HeightMap.GenerateHeightmap();
	model = HeightMap.exportMesh(1.0, 1.0);
	this->shader = shader;

	//std::string grassFilePath = std::string(DATA_PATH) + "Twisted Hills2.jpg";
	//std::string sandFilePath = std::string(DATA_PATH) + "sand.jpg";

	//grassTexture = new Texture(grassFilePath.c_str());
	//sandTexture = new Texture(sandFilePath.c_str());
}

void HillPlane::Draw(std::stack<glm::mat4> modelMatrix)
{
	shader->activate();
	modelMatrix.push(glm::mat4(1.0));

	modelMatrix.push(modelMatrix.top());
	modelMatrix.top() = glm::translate(modelMatrix.top(), position);
	modelMatrix.top() = glm::rotate(modelMatrix.top(), rotationDegrees, rotationAxis);
	modelMatrix.top() = glm::scale(modelMatrix.top(), scaling);

	//ProgramContext.SetShader(0);
	//ProgramContext.UpdateModToCamBlock(modelMatrix.Top());

	shader->uniformMatrix4fv("modelMatrix", modelMatrix.top());

	//TODO glm::mat3 normalMatrix(glm::transpose(glm::inverse(glm::mat3(modelMatrix.top()))));
	//TODO glUniformMatrix3fv(ProgramContext.programList[0]->uniforms[UniformEnums::NORMAL_MOD_TO_CAM], 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//glUniform1i(ProgramContext.programList[0]->uniforms[UniformEnums::GRASS_TEX_TARGET], 0);
	//grassTexture->BindTexture(0);

	//glUniform1i(ProgramContext.programList[0]->uniforms[UniformEnums::SAND_TEX_TARGET], 1);
	//sandTexture->BindTexture(1);

	model->Render();
}

//void HillPlane::DrawReflection(glutil::MatrixStack modelMatrix, float clipHeight)
//{
//	glutil::MatrixStack Push(modelMatrix);
//
//	ProgramContext.SetShader(2);
//
//	glutil::MatrixStack modelToWorldMatrix;
//	modelToWorldMatrix.SetIdentity();
//	modelToWorldMatrix.Scale(1.0, -1.0, 1.0);
//	modelToWorldMatrix.Translate(position);
//	modelToWorldMatrix.Rotate(rotationAxis, rotationDegrees);
//	modelToWorldMatrix.Scale(scaling);
//	modelMatrix.ApplyMatrix(modelToWorldMatrix.Top());
//
//	ProgramContext.UpdateModToCamBlock(modelMatrix.Top());
//
//	glm::mat3 normalMatrix(glm::transpose(glm::inverse(glm::mat3(modelMatrix.Top()))));
//	glUniform1f(ProgramContext.programList[2]->uniforms[UniformEnums::CLIP_HEIGHT], clipHeight);
//
//	glUniformMatrix4fv(ProgramContext.programList[2]->uniforms[UniformEnums::MOD_TO_WORLD], 1, GL_FALSE, glm::value_ptr(modelToWorldMatrix.Top()));
//	glUniformMatrix3fv(ProgramContext.programList[2]->uniforms[UniformEnums::NORMAL_MOD_TO_CAM], 1, GL_FALSE, glm::value_ptr(normalMatrix));
//
//	//glUniform1i(ProgramContext.programList[2]->uniforms[UniformEnums::GRASS_TEX_TARGET], 0);
//	//grassTexture->BindTexture(0);
//
//	//glUniform1i(ProgramContext.programList[2]->uniforms[UniformEnums::SAND_TEX_TARGET], 1);
//	//sandTexture->BindTexture(1);
//
//	glDisable(GL_CULL_FACE);
//	model->Render();
//	glEnable(GL_CULL_FACE);
//}

HillPlane::~HillPlane()
{
	delete model;
	//delete sandTexture;
	//delete grassTexture;
}

//Skybox::Skybox(float rotateOffset)
//{
//	rotationDegrees = rotateOffset;
//
//	std::vector<VertexComponents> format;
//	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));
//
//	unsigned int vertexCount = 8;
//	unsigned int elementCount = 36;
//	model = new Mesh(format, (void*)vertices, vertexCount, elements, elementCount, GL_TRIANGLES);
//
//	std::string frontPath = std::string(DATA_PATH) + "skyrender1.bmp";
//	std::string rightPath = std::string(DATA_PATH) + "skyrender2.bmp";
//	std::string backPath = std::string(DATA_PATH) + "skyrender4.bmp";
//	std::string leftPath = std::string(DATA_PATH) + "skyrender5.bmp";
//	std::string topPath = std::string(DATA_PATH) + "skyrender3.bmp";
//	std::string bottomPath = std::string(DATA_PATH) + "skyrender6.bmp";
//	cubeMap = new Texture(frontPath.c_str(), backPath.c_str(), topPath.c_str(), bottomPath.c_str(), leftPath.c_str(), rightPath.c_str());
//}
//
//void Skybox::Draw(glutil::MatrixStack modelMatrix)
//{
//	glutil::MatrixStack Push(modelMatrix);
//	modelMatrix.RotateY(rotationDegrees);
//	glm::mat4 zeroTranslation(1.0);
//	zeroTranslation[3][3] = 0.0;
//	modelMatrix.ApplyMatrix(zeroTranslation);
//
//	ProgramContext.SetShader(1);
//	ProgramContext.UpdateModToCamBlock(modelMatrix.Top());
//
//	glDisable(GL_CULL_FACE);
//	glDisable(GL_DEPTH_TEST);
//	cubeMap->BindTexture(0);
//	glUniform1i(ProgramContext.programList[1]->uniforms[UniformEnums::SKY_TEX_TARGET], 0);
//
//	model->Render();
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
//
//}
//
//Skybox::~Skybox()
//{
//	delete model;
//	delete cubeMap;
//}
//
//WaterPlane::WaterPlane(float scale, float height)
//	:position(0.0, height, 0.0),
//	rotationAxis(0.0, 1.0, 0.0),
//	rotationDegrees(0.0),
//	scaling(scale, scale, scale),
//	texOffset(true, 20.0),
//	waveOffset(true, 18.0)
//{
//	glutil::MatrixStack texMatrix;
//	texMatrix.SetIdentity();
//	texMatrix.Translate(glm::vec3(0.5, 0.5, 0.0));
//	texMatrix.Scale(glm::vec3(0.5, 0.5, 1.0));
//	clipToTexMat = texMatrix.Top();
//
//	std::vector<VertexComponents> format;
//	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));
//	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));
//
//	unsigned int vertexCount = 4;
//	unsigned int elementCount = 6;
//	model = new Mesh(format, (void*)vertices, vertexCount, elements, elementCount, GL_TRIANGLES);
//
//	texBuffer = new FrameBuffer(ProgramContext.ScreenWidth(), ProgramContext.ScreenHeight());
//	reflection = new Texture(ProgramContext.ScreenWidth(), ProgramContext.ScreenHeight());
//	refractMask = new Texture(ProgramContext.ScreenWidth(), ProgramContext.ScreenHeight());
//	floorDepthTexture = new Texture(ProgramContext.ScreenWidth(), ProgramContext.ScreenHeight(), GL_DEPTH_COMPONENT);
//	surfDepthTexture = new Texture(ProgramContext.ScreenWidth(), ProgramContext.ScreenHeight(), GL_DEPTH_COMPONENT);
//
//	//std::string displacementPath = std::string(DATA_PATH) + "water_dudv.jpg";
//	//std::string normalPath = std::string(DATA_PATH) + "water_normal.jpg";
//	std::string displacementPath = std::string(DATA_PATH) + "Wavy_Water_DuDv_Height2.png";
//	std::string normalPath = std::string(DATA_PATH) + "Wavy_Water - Height (Normal Map 2).png";
//	displacementTex = new Texture(displacementPath.c_str());
//	normalMap = new Texture(normalPath.c_str());
//}
//
//void WaterPlane::CopyDefaultBuffer()
//{
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//
//	glBindTexture(GL_TEXTURE_2D, refractMask->GetTexHandle());
//	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, refractMask->width, refractMask->height);
//	glBindTexture(GL_TEXTURE_2D, floorDepthTexture->GetTexHandle());
//	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, floorDepthTexture->width, floorDepthTexture->height);
//
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, texBuffer->frameBufferHandle);
//
//	glBlitFramebuffer(0, 0, texBuffer->width, texBuffer->height,
//		0, 0, texBuffer->width, texBuffer->height,
//		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
//	texBuffer->BindTexture(refractMask);
//}
//
//void WaterPlane::GenerateRefractMask()
//{
//	CopyDefaultBuffer();
//	glBindFramebuffer(GL_FRAMEBUFFER, texBuffer->frameBufferHandle);
//
//	texBuffer->BindBuffer();
//
//	glColorMask(FALSE, FALSE, FALSE, TRUE);
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	ProgramContext.SetShader(4);
//	model->Render();
//
//	glColorMask(TRUE, TRUE, TRUE, TRUE);
//	glBindTexture(GL_TEXTURE_2D, surfDepthTexture->GetTexHandle());
//	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, surfDepthTexture->width, surfDepthTexture->height);
//}
//
//void WaterPlane::Draw(glutil::MatrixStack modelMatrix)
//{
//	glutil::MatrixStack Push(modelMatrix);
//	texBuffer->BindBuffer();
//	texBuffer->BindTexture(reflection);
//
//	ProgramContext.currentScene->RenderSceneReflection(position.y);
//
//	texBuffer->BindTexture(0);
//
//	glutil::MatrixStack modelToWorld;
//	modelToWorld.SetIdentity();
//	modelToWorld.Translate(position);
//	modelToWorld.Rotate(rotationAxis, rotationDegrees);
//	modelToWorld.Scale(scaling);
//
//	modelMatrix.ApplyMatrix(modelToWorld.Top());
//	ProgramContext.UpdateModToCamBlock(modelMatrix.Top());
//
//	GenerateRefractMask();
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	ProgramContext.SetShader(3);
//
//	glUniformMatrix4fv(ProgramContext.programList[3]->uniforms[UniformEnums::CLIP_TO_TEX], 1, GL_FALSE, glm::value_ptr(clipToTexMat));
//	glUniform1f(ProgramContext.programList[3]->uniforms[UniformEnums::TIME_1], texOffset.GetAlpha());
//	glUniform1f(ProgramContext.programList[3]->uniforms[UniformEnums::TIME_2], waveOffset.GetAlpha());
//	UpdateLightingUniforms(modelToWorld.Top());
//
//	glUniform1i(ProgramContext.programList[3]->uniforms[UniformEnums::REFLECT_TEX_TARGET], 0);
//	glUniform1i(ProgramContext.programList[3]->uniforms[UniformEnums::REFRACT_TEX_TARGET], 1);
//	glUniform1i(ProgramContext.programList[3]->uniforms[UniformEnums::DISPLACE_TEX_TARGET], 2);
//	glUniform1i(ProgramContext.programList[3]->uniforms[UniformEnums::NORMAL_TEX_TARGET], 3);
//	glUniform1i(ProgramContext.programList[3]->uniforms[UniformEnums::FLOOR_DEPTH_TARGET], 4);
//	glUniform1i(ProgramContext.programList[3]->uniforms[UniformEnums::SURF_DEPTH_TARGET], 5);
//
//	reflection->BindTexture(0);
//	refractMask->BindTexture(1);
//	displacementTex->BindTexture(2);
//	normalMap->BindTexture(3);
//	floorDepthTexture->BindTexture(4);
//	surfDepthTexture->BindTexture(5);
//
//	glDisable(GL_CULL_FACE);
//	model->Render();
//	glEnable(GL_CULL_FACE);
//}
//
//void WaterPlane::UpdateLightingUniforms(glm::mat4 tanToWorld)
//{
//	glm::vec4 cameraPos(ProgramContext.currentScene->mainCamera.GetEyePosition(), 1.0);
//	glm::vec3 lightDirection(ProgramContext.currentScene->lightDirection);
//
//	glm::mat4 worldToTan = glm::inverse(tanToWorld);
//
//	cameraPos = worldToTan * cameraPos;
//	lightDirection = glm::normalize(glm::mat3(worldToTan) * lightDirection);
//
//	glUniform3fv(ProgramContext.programList[3]->uniforms[UniformEnums::EYE_TAN_SPACE], 1, glm::value_ptr(glm::vec3(cameraPos)));
//	glUniform3fv(ProgramContext.programList[3]->uniforms[UniformEnums::LIGHT_TAN_SPACE], 1, glm::value_ptr(lightDirection));
//}
//
//void WaterPlane::Reshape(int screenW, int screenH)
//{
//	delete texBuffer;
//	delete reflection;
//	delete refractMask;
//	delete floorDepthTexture;
//	delete surfDepthTexture;
//
//	texBuffer = new FrameBuffer(screenW, screenH);
//	reflection = new Texture(screenW, screenH);
//	refractMask = new Texture(screenW, screenH);
//	floorDepthTexture = new Texture(screenW, screenH, GL_DEPTH_COMPONENT);
//	surfDepthTexture = new Texture(screenW, screenH, GL_DEPTH_COMPONENT);
//}