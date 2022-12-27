#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Model.h"
#include "SkyBox.h"
#include "SoundEngine.h"

Window mainWindow;
Camera camera;
std::vector<Mesh*> meshList;

Shader mainShader;
Shader directionalLightShadowShader;
Shader omniShadowShader;

Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material notShinyMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

SkyBox skybox;

SoundEngine* soundEngine;

Model oldHouse;
Model flashLight;
Model oldWell;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint uniformProjection = 0;
GLuint uniformModel = 0;
GLuint uniformView = 0;
GLuint uniformCameraPosition = 0;
GLuint uniformSpecularIntensity = 0;
GLuint uniformShinePower = 0;
GLuint uniformOmniLightPosition = 0;
GLuint uniformFarPlane = 0;
GLfloat angle = 0.0f;

unsigned int spotLightCount = 1;
unsigned int pointLightCount = 3;

void CreateMeshes()
{
	Mesh* plane= new Mesh();
	plane->CreateBasicPlane();
	meshList.push_back(plane);
}

void CreateShaders()
{
	mainShader = Shader();
	mainShader.CreateFromFiles(MAIN_VERTEX_SHADER.c_str(), MAIN_FRAGMENT_SHADER.c_str());

	directionalLightShadowShader = Shader();
	directionalLightShadowShader.CreateFromFiles(DIRECTIONAL_SHADOW_VERTEX_SHADER.c_str() ,
		DIRECTIONAL_SHADOW_FRAGMENT_SHADER.c_str());

	omniShadowShader =  Shader();
	omniShadowShader.CreateFromFiles( OMNIDIRECTIONAL_SHADOW_VERTEX_SHADER.c_str(),
	OMNIDIRECTIONAL_SHADOW_GEOMETRY_SHADER.c_str(),
	OMNIDIRECTIONAL_SHADOW_FRAGMENT_SHADER.c_str());
}

void CreateCamera()
{
	camera = Camera(glm::vec3(0.0f, 2.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.1f);
}

void CreateTextures()
{
	dirtTexture = Texture(PLANE_TEXTURE.c_str());
	dirtTexture.LoadTexture(true);

	plainTexture = Texture(ERROR_TEXTURE.c_str());
	plainTexture.LoadTexture(true);
}

void CreateMaterials()
{
	shinyMaterial = Material(1.0f, 32);
	notShinyMaterial = Material(0.3, 4);
}

void CreateModels()
{
	oldHouse = Model(false);
	oldHouse.LoadModel(HOUSE_MODEL);

	flashLight = Model(false);
	flashLight.LoadModel(FLASHLIGHT_MODEL);

	oldWell = Model(false);
	oldWell.LoadModel(WELL_MODEL);
}

void CreateLights()
{
	mainLight = DirectionalLight(0.5f, 1.0f, 0.5f, 0.1f, 0.3f,
	                             -5.0f, -7.0f, 5.0f, 2048, 2048);

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
	                            0.3f, 0.8f,
	                            0.0f, 1.0f, 4.0f,
	                            1.0f, 1.0f, 0.3f,
	                            1024, 1024, 0.1f, 100);

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
	                            0.1f, 0.9f,
	                            4.0f, 1.0f, 1.0f,
	                            1.0f, 1.0f, 0.2f,
	                            1024, 1024, 0.1f, 100);

	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
	                            0.3f, 0.8f,
	                            -3.0f, 6.0f, 3.0f,
	                            1.00f, 1.00f, 0.2f,
	                            1024, 1024, 0.1f, 100);
	
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	                          0.2f, 1.0f,
	                          0.0f, 1.0f, 0.0f,
	                          1.8f, -1.0f, 0.0f,
	                          0.05f, 0.1f, 0.1f, 25.0f,
	                          1024, 1024, 0.1f, 100);
}

void CreateSkybox()
{
	std::vector<std::string> skyboxFaces;
	
	skyboxFaces.push_back(SKYBOX_FT);
	skyboxFaces.push_back(SKYBOX_BK);
	skyboxFaces.push_back(SKYBOX_UP);
	skyboxFaces.push_back(SKYBOX_DN);
	skyboxFaces.push_back(SKYBOX_RT);
	skyboxFaces.push_back(SKYBOX_LF);
	skybox = SkyBox(skyboxFaces);
}

void SetFlashLightTransform(glm::mat4& model)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(
		                       camera.GetCameraPosition().x,
		                       camera.GetCameraPosition().y,
		                       camera.GetCameraPosition().z));
	
	model = glm::rotate(model, camera.GetYaw() * 3.14f / 180.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, camera.GetPitch() * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	
	model = glm::translate(model, glm::vec3(
		                       0.4f,
		                       -0.1f,
		                       0.1f));
	model = glm::rotate(model, 180.0f * 3.14f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notShinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShinePower);
	flashLight.RenderModel();
}

void RenderScene()
{
	glm::mat4 model(1.0f);
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShinePower);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notShinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShinePower);
	oldHouse.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notShinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShinePower);
	oldWell.RenderModel();
	
	SetFlashLightTransform(model);
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalLightShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalLightShadowShader.GetModelLocation();
	glm::mat4 lightTransform = light->CalculateLightTransform(); 
	directionalLightShadowShader.SetDirectionalLightTransform(&lightTransform);

	directionalLightShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPosition = omniShadowShader.GetOmniLightPositionLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPosition, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());
	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glViewport(0, 0, 1920, 1080);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkyBox(viewMatrix, projectionMatrix);

	mainShader.UseShader();
	uniformProjection = mainShader.GetProjectionLocation();
	uniformModel = mainShader.GetModelLocation();
	uniformView = mainShader.GetViewLocation();
	uniformCameraPosition = mainShader.GetCameraPositionLocation();
	uniformSpecularIntensity = mainShader.GetSpecularIntensityLocation();
	uniformShinePower = mainShader.GetShinePower();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformCameraPosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);
	
	mainShader.SetDirectionalLight(&mainLight);
	mainShader.SetPointLights(pointLights, pointLightCount, 3, 0);
	mainShader.SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	
	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	mainShader.SetDirectionalLightTransform(&lightTransform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	mainShader.SetTexture(1);
	mainShader.SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.GetCameraPosition();
	lowerLight.y += 0.4f * camera.GetCameraDirection().y;
	lowerLight.z += 0.4f * camera.GetCameraDirection().z;
	lowerLight.x += 0.4f * camera.GetCameraDirection().x;
	spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection());
	mainShader.Validate();
	RenderScene();
}

int main()
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();
	soundEngine = new SoundEngine();
	soundEngine->Play(BACKGROUND_MUSIC, true);
	
	CreateMeshes();
	CreateShaders();
	CreateCamera();
	CreateTextures();
	CreateMaterials();
	CreateModels();
	CreateLights();
	CreateSkybox();

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		glfwPollEvents();

		camera.KeyControl(mainWindow.getKeys(), deltaTime);
		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		
		if(mainWindow.getKeys()[GLFW_KEY_L])
		{
			spotLights[0].Toggle();
			soundEngine->Play(FLASHLIGHT_SOUND, false);
			mainWindow.getKeys()[GLFW_KEY_L] = false;
		}
		
		DirectionalShadowMapPass(&mainLight);
		for(size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for(size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera.CalculateViewMatrix());
		
		glUseProgram(0);

		mainWindow.SwapBuffers();
	}
	return 0;
}
