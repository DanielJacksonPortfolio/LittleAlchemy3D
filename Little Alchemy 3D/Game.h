#pragma once

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <irrKlang.h>

// Other includes
#include "Constants.h"
#include "Callbacks.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Text.h"
#include "Player.h"
#include "Elements.h"
#include "Item.h"
#include "GUIQuad.h"
#include "Menu.h"

#include <ctime>

class Game
{
private:
	//music variables
	irrklang::ISoundEngine* _SoundEngine;
	irrklang::ISound* music;

	Camera* _camera;
	GLFWwindow* _window;

	Shader* _skyboxShader, *_simpleDepthShader, *_simpleMaterialShader, * _textShader, * _3dTextShader, * _GUIShader;
	Player* _player;
	Model* _mainPlate = nullptr,* _mainPlateGrass = nullptr;
	ElementManager* _elementManager = nullptr;
	ItemManager* _itemManager = nullptr;

	//font stuff
	Font* _font;
	Text* _fpsText, *_coordsText;
	Menu* _menu;

	GUIQuad* _testQuad;
	//time keeping
	float _gameTime = 13.0;
	float _gameFloat = 0;
	float _gameSpeed = 1.0f;
	float _gameFloatNorm = 0;

	// camera
	float _lastX = SCR_WIDTH / 2.0f;
	float _lastY = SCR_HEIGHT / 2.0f;
	float _mouseDiffX, _mouseDiffY, _beforeLastX, _beforeLastY;

	// timing
	float _deltaTime = 0.0f;
	float _gameDeltaTime = 0.0f;
	float _lastFrame = 0.0f;

	//skycolour
	glm::vec3 _skyColour = glm::vec3(0.0f);//0.78f, 0.85f, 0.92f

	//opengl textures
	GLuint _skyboxVAO, _skyboxVBO;
	unsigned int _cubemapTexture;
	unsigned int depthMapFBO, depthMap;

	//opengl matricies
	glm::mat4 projection;
	glm::mat4 view;
	
	//other vec3s
	glm::vec3 lightPos;
	glm::vec3 cameraToWorldVector;
	
	//input variables
	InputManager* _inputManager;
	bool _firstMouse = true;
	bool tKeyPressed = false, cKeyPressed = false, usingParallaxOcclusionMapping = false, bKeyPressed = false, leftKeyPressed = false, rightKeyPressed = false, upKeyPressed = false, downKeyPressed = false, rKeyPressed = false;

public:
	Game(int argc, char* argv[]);
	~Game();
	//initializing
	bool InitGL();
	bool InitObjects();

	//game loop
	void RunGameLoop();
	void Display();
	void Update();

	//callbacks
	void ProcessInput(GLFWwindow *window);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	//common shader variables
	void SetShaderVariables(Shader* shader);
	void UpdateShaderVariables(Shader* shader);

	//depth scene for shadows
	void RenderDepthScene();

	//skybox
	void DrawSkybox(Shader* skyboxShader, unsigned int skyboxVAO, unsigned int cubemapTexture);
	
	//misc
	glm::vec3 GetMouseToWorldVector();
};

