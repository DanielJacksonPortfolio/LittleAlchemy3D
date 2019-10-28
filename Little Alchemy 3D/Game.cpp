#include "Game.h"



Game::Game(int argc, char* argv[])
{
	if (InitGL() && InitObjects()) {
		RunGameLoop();
	}
	glfwTerminate();
}


Game::~Game()
{

}

bool Game::InitGL()
{
	Callbacks::Init(this);

	_SoundEngine = irrklang::createIrrKlangDevice();
	music = _SoundEngine->play3D("res/audio/marioMenuMusic.wav", irrklang::vec3df(0, 25, 0), true, false, true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (FULLSCREEN) _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CONTEXT", glfwGetPrimaryMonitor(), NULL);
	else _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CONTEXT", NULL, NULL);

	if (_window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, Callbacks::FramebufferSizeCallback);
	glfwSetCursorPosCallback(_window, Callbacks::MouseCallback);
	glfwSetMouseButtonCallback(_window, Callbacks::MouseButtonCallback);
	glfwSetScrollCallback(_window, Callbacks::ScrollCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_LINE_SMOOTH);
	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	return true;
}

bool Game::InitObjects()
{
	lightPos = glm::vec3(1.0f, 10.0f, -10.0f);
	srand(time(NULL));
	
	//_SoundEngine->setSoundVolume(0.2f);
	//_SoundEngine->play2D("res/audio/music.wav", GL_TRUE);

	_camera = new Camera(glm::vec3(0.0f, 0.35f, 0.0f));


	_skyboxShader = new Shader("shaders/skybox_vertex.glsl", "shaders/skybox_fragment.glsl");
	_simpleDepthShader = new Shader("shaders/depth_vertex.glsl","shaders/depth_fragment.glsl");
	_simpleMaterialShader = new Shader("shaders/simple_material_vertex.glsl", "shaders/simple_material_fragment.glsl");
	_textShader = new Shader("shaders/text_vertex.glsl", "shaders/text_fragment.glsl");
	_3dTextShader = new Shader("shaders/3d_text_vertex.glsl", "shaders/3d_text_fragment.glsl");
	_GUIShader = new Shader("shaders/gui_vertex.glsl", "shaders/gui_fragment.glsl");
	std::cout << "loaded shaders" << std::endl;


	//glm::mat4 projection2 = glm::perspective(90.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
	glm::mat4 projection2 = glm::ortho(-0.5f * static_cast<float>(SCR_WIDTH),0.5f * static_cast<float>(SCR_WIDTH), -0.5f * static_cast<float>(SCR_HEIGHT), 0.5f * static_cast<float>(SCR_HEIGHT),0.01f,1000.0f);
	glm::mat4 view2 = glm::lookAt(glm::vec3(0.0f,0.0f,100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	_3dTextShader->use();
	_3dTextShader->setMat4("projection", projection2);
	_3dTextShader->setMat4("view", view2);
	glm::mat4 model = glm::mat4(1.0f);
	_3dTextShader->setMat4("model", model);

	_GUIShader->use();
	_GUIShader->setBool("useTexture", true);
	_GUIShader->setInt("GUITexture", 0);
	_GUIShader->setVec3("colour", glm::vec3(1.0f,1.0f,1.0f));

	SetShaderVariables(_simpleMaterialShader);

	_simpleMaterialShader->setInt("material.texture_diffuse1", 0);
	_simpleMaterialShader->setInt("material.texture_specular1", 1);
	_simpleMaterialShader->setInt("material.texture_normal1", 2);
	_simpleMaterialShader->setVec3("material.specular_colour", glm::vec3(0.7f, 0.7f, 0.7f));

	_textShader->use();
	_textShader->setInt("textureAtlas", 0);

	std::cout << "set shader variables" << std::endl;

	//shadow frame buffer
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::string path = "res/images/cube_maps/";
	std::string skyboxName = "field_night";//tga->(jaspercoast,lagoon,morning,miramar,violentdays,mandaris,drakeq,ss)//jpg->(lake1,default,axishelp)//
	std::string fileExtension = "png";

	std::vector<std::string> faces = {
		path + skyboxName + "/px." + fileExtension,//right
		path + skyboxName + "/nx." + fileExtension,//left
		path + skyboxName + "/py." + fileExtension,//up
		path + skyboxName + "/ny." + fileExtension,//down
		path + skyboxName + "/pz." + fileExtension,//back
		path + skyboxName + "/nz." + fileExtension//front
	};

	_cubemapTexture = TextureLoader::LoadCubemap(faces);

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

		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,


		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &_skyboxVAO);
	glGenBuffers(1, &_skyboxVBO);
	glBindVertexArray(_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	_simpleMaterialShader->use();
	_simpleMaterialShader->setInt("skybox", 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);

	std::cout << "loaded skybox" << std::endl;

	//font stuff
	_font = new Font();
	_font->LoadFont("monofont");//openGL//smooth//monofont
	//_fpsText = new Text("FPS", _font, Rect(100.0f,0.0f,100.0f,100.0f), 50.0f, glm::vec3(0.85f, 0.85f, 0.85f), 0.8f, 0);
	//_fpsText->UpdateQuads();
	//_coordsText = new Text("SPACE TEST", _font, Rect(300.0f, 0.0f, 700.0f, 100.0f), 50.0f, glm::vec3(0.85f, 0.85f, 0.85f), 0.8f, 0);
	//_coordsText->UpdateQuads();
	//GUITexture* testTexture = new GUITexture("res/images/gui_textures/pacman_map.png");
	//_testQuad = new GUIQuad(new Rect(0, 200, 100, 100), glm::vec3(1.0f, 1.0f, 1.0f), new Rect(0.0f, 0.0f, testTexture->width, testTexture->height), testTexture);
	_player = new Player(_camera);

	_inputManager = new InputManager();
	_mainPlate = new Model("main_plate");
	_mainPlateGrass = new Model("main_plate_grass");
	_elementManager = new ElementManager(_font);
	_itemManager = new ItemManager(_elementManager,_camera, _inputManager);

	_itemManager->AddItem(glm::vec3(7.0f, 5.0f, 0.0f), 0);
	_itemManager->AddItem(glm::vec3(8.0f, 6.0f, 0.0f), 1);
	_itemManager->AddItem(glm::vec3(9.0f, 7.0f, 0.0f), 2);
	_itemManager->AddItem(glm::vec3(10.0f, 8.0f, 0.0f), 670);

	_menu = new Menu(_window, _elementManager, _itemManager);
	std::cout << "loaded menu" << std::endl;

	return true;
}

void Game::SetShaderVariables(Shader* shader) {
	shader->use();

	shader->setFloat("material.shininess", 32.0f);

	shader->setVec3("dirLight.direction", glm::vec3(2.0f, -4.0f, 1.0f));
	shader->setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));//1.0f, 0.95f, 0.9f
	shader->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));

	shader->setVec3("pointLights[0].position", _camera->Position);
	shader->setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader->setVec3("pointLights[0].diffuse", glm::vec3(0.85f, 0.25f, 0.0f));
	shader->setVec3("pointLights[0].specular", glm::vec3(0.85f, 0.25f, 0.0f));
	shader->setFloat("pointLights[0].constant", 1.0f);
	shader->setFloat("pointLights[0].linear", 0.09);
	shader->setFloat("pointLights[0].quadratic", 0.032);

	shader->setVec3("spotLight.position", _camera->Position);
	shader->setVec3("spotLight.direction", _camera->Front);
	shader->setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setVec3("spotLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
	shader->setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->setFloat("spotLight.constant", 1.0f);
	shader->setFloat("spotLight.linear", 0.09);
	shader->setFloat("spotLight.quadratic", 0.032);
	shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(17.5f)));
	shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.5f)));

	shader->setVec3("skyColour", _skyColour);
}

void Game::RunGameLoop()
{
	while (!glfwWindowShouldClose(_window)) {
		Update();
		Display();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Game::UpdateShaderVariables(Shader* shader) {
	shader->use();

	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setVec3("viewPos", _camera->Position);
	//shader->setVec3("pointLights[0].position", _camera->Position);
	shader->setVec3("spotLight.position", _camera->Position);
	shader->setVec3("spotLight.direction", _camera->Front);
}
void Game::Display()
{
	UpdateShaderVariables(_simpleMaterialShader);
	_3dTextShader->use();
	_3dTextShader->setMat4("projection", projection);
	_3dTextShader->setMat4("view", view);

	//shadow map variables
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 0.1f, far_plane = 100.0f;
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(0.0f,2.0f,0.0f) + lightPos, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	// render scene from light's point of view
	_simpleDepthShader->use();
	_simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	RenderDepthScene();
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	
	//clear buffers
	glClearColor(_skyColour.x, _skyColour.y, _skyColour.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawSkybox(_skyboxShader, _skyboxVAO, _cubemapTexture);

	_simpleMaterialShader->use();
	_simpleMaterialShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	_simpleMaterialShader->setVec3("lightPos", lightPos);
	_simpleMaterialShader->setInt("shadowMap", 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glm::mat4 model = glm::mat4(1.0f);
	_simpleMaterialShader->setMat4("model", model);

	_mainPlate->Draw(_simpleMaterialShader);
	_simpleMaterialShader->setBool("projectCubemapTexture", true);
	_mainPlateGrass->Draw(_simpleMaterialShader);
	_simpleMaterialShader->setBool("projectCubemapTexture", false);

	_itemManager->DrawItems(_simpleMaterialShader);
	_itemManager->DrawItemTexts(_3dTextShader);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//_fpsText->Draw(_textShader);
	//_coordsText->Draw(_textShader);
	_menu->Draw(_GUIShader, _textShader);
	//_GUIShader->use();
	//_testQuad->Draw(_GUIShader);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}

void Game::RenderDepthScene() {
	glm::mat4 model = glm::mat4(1.0f);

	_simpleDepthShader->setMat4("model", model);
	_mainPlate->Draw(_simpleDepthShader);

	_itemManager->DrawItems(_simpleDepthShader, false);
	//_elementManager->GetElements()[665].GetModel()->Draw(_simpleMaterialShader);
	//_elementManager->GetDefaultModel()->Draw(_simpleDepthShader);

	//_terrain->Draw(_simpleDepthShader, false);
	//_twister1->Draw(_simpleDepthShader);
	//_rollerCoaster1->Draw(_simpleDepthShader, _simpleDepthShader, _lineShader, true, false);
	//_testmesh->Draw(_simpleDepthShader);
}

void Game::Update()
{
	//per frame logic
	float currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;

	if (_deltaTime > 0.1f) _deltaTime = 0.1f;
	_gameDeltaTime = _deltaTime * _gameSpeed;
	//get inputs
	ProcessInput(_window);
	_gameTime += _deltaTime;
	_gameFloat = _gameTime * _gameSpeed;
	_gameFloatNorm = 1.0 - (sin(_gameFloat) * 0.5 + 0.5);

	if (_beforeLastX == _lastX && _beforeLastY == _lastY) {
		_mouseDiffX = 0.0f;
		_mouseDiffY = 0.0f;
	}

	/*_fpsText->SetText("FPS: " + std::to_string(1.0f/_deltaTime).substr(0,7));
	_fpsText->UpdateQuads();
	_coordsText->SetText("COORDS: x:" + std::to_string(_camera->Position.x).substr(0, 5) + " y:" + std::to_string(_camera->Position.y).substr(0, 5) + " z:" + std::to_string(_camera->Position.z).substr(0, 5));
	_coordsText->UpdateQuads();*/


	_menu->Input(glm::vec2(_lastX, _lastY), _inputManager);
	_player->Update(_deltaTime);
	_itemManager->UpdateItems(_deltaTime, !(_menu->showMenu || _menu->showEGrid || _menu->showPauseMenu));

	// view/projection transformations
	projection = glm::perspective(glm::radians(_camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
	view = _camera->GetViewMatrix();

	cameraToWorldVector = GetMouseToWorldVector();
	
	_beforeLastX = _lastX;
	_beforeLastY = _lastY;

	_SoundEngine->setListenerPosition(irrklang::vec3df(-_camera->Position.x, _camera->Position.y, -_camera->Position.z), irrklang::vec3df(_camera->Front.x, _camera->Front.y, _camera->Front.z));
}

//INPUT
void Game::ProcessInput(GLFWwindow *window)
{
	float speedMod = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speedMod = 2.6f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		speedMod = 0.25f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		if (!_menu->showEGrid && !_menu->showMenu && !_menu->showPauseMenu) _player->ProcessKeyboard(PM_FORWARD, _deltaTime, speedMod);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		if (!_menu->showEGrid && !_menu->showMenu && !_menu->showPauseMenu) _player->ProcessKeyboard(PM_BACKWARD, _deltaTime, speedMod);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		if (!_menu->showEGrid && !_menu->showMenu && !_menu->showPauseMenu) _player->ProcessKeyboard(PM_LEFT, _deltaTime, speedMod);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		if (!_menu->showEGrid && !_menu->showMenu && !_menu->showPauseMenu) _player->ProcessKeyboard(PM_RIGHT, _deltaTime, speedMod);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		if (!_menu->showEGrid && !_menu->showMenu && !_menu->showPauseMenu) _player->ProcessKeyboard(PM_JUMP, _deltaTime, speedMod);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		_itemManager->AddItem(_player->position, _elementManager->elementsWithModels.at(rand() % _elementManager->elementsWithModels.size()));
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		while (1) {
			std::cout << "name: ";
			//std::string name;
			//std::cin >> name;
			char name[25];
			std::cin.getline(name, 25);
			if (std::string(name) == "done") break;
			int id = _elementManager->GetIDFromName(name);
			if (id >= 0) {
				_itemManager->AddItem(_camera->Position, id);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) tKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) leftKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) rightKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) upKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) downKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) cKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) bKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) rKeyPressed = false;

	if (_inputManager->keyStates["a"] == 0 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) _inputManager->keyStates["a"] = 2;
	if (_inputManager->keyStates["b"] == 0 && glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) _inputManager->keyStates["b"] = 2;
	if (_inputManager->keyStates["c"] == 0 && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) _inputManager->keyStates["c"] = 2;
	if (_inputManager->keyStates["d"] == 0 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) _inputManager->keyStates["d"] = 2;
	if (_inputManager->keyStates["e"] == 0 && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) _inputManager->keyStates["e"] = 2;
	if (_inputManager->keyStates["f"] == 0 && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) _inputManager->keyStates["f"] = 2;
	if (_inputManager->keyStates["g"] == 0 && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) _inputManager->keyStates["g"] = 2;
	if (_inputManager->keyStates["h"] == 0 && glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) _inputManager->keyStates["h"] = 2;
	if (_inputManager->keyStates["i"] == 0 && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) _inputManager->keyStates["i"] = 2;
	if (_inputManager->keyStates["j"] == 0 && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) _inputManager->keyStates["j"] = 2;
	if (_inputManager->keyStates["k"] == 0 && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) _inputManager->keyStates["k"] = 2;
	if (_inputManager->keyStates["l"] == 0 && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) _inputManager->keyStates["l"] = 2;
	if (_inputManager->keyStates["m"] == 0 && glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) _inputManager->keyStates["m"] = 2;
	if (_inputManager->keyStates["n"] == 0 && glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) _inputManager->keyStates["n"] = 2;
	if (_inputManager->keyStates["o"] == 0 && glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) _inputManager->keyStates["o"] = 2;
	if (_inputManager->keyStates["p"] == 0 && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) _inputManager->keyStates["p"] = 2;
	if (_inputManager->keyStates["q"] == 0 && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) _inputManager->keyStates["q"] = 2;
	if (_inputManager->keyStates["r"] == 0 && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) _inputManager->keyStates["r"] = 2;
	if (_inputManager->keyStates["s"] == 0 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) _inputManager->keyStates["s"] = 2;
	if (_inputManager->keyStates["t"] == 0 && glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) _inputManager->keyStates["t"] = 2;
	if (_inputManager->keyStates["u"] == 0 && glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) _inputManager->keyStates["u"] = 2;
	if (_inputManager->keyStates["v"] == 0 && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) _inputManager->keyStates["v"] = 2;
	if (_inputManager->keyStates["w"] == 0 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) _inputManager->keyStates["w"] = 2;
	if (_inputManager->keyStates["x"] == 0 && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) _inputManager->keyStates["x"] = 2;
	if (_inputManager->keyStates["y"] == 0 && glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) _inputManager->keyStates["y"] = 2;
	if (_inputManager->keyStates["z"] == 0 && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) _inputManager->keyStates["z"] = 2;
	if (_inputManager->keyStates["0"] == 0 && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) _inputManager->keyStates["0"] = 2;
	if (_inputManager->keyStates["1"] == 0 && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) _inputManager->keyStates["1"] = 2;
	if (_inputManager->keyStates["2"] == 0 && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) _inputManager->keyStates["2"] = 2;
	if (_inputManager->keyStates["3"] == 0 && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) _inputManager->keyStates["3"] = 2;
	if (_inputManager->keyStates["4"] == 0 && glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) _inputManager->keyStates["4"] = 2;
	if (_inputManager->keyStates["5"] == 0 && glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) _inputManager->keyStates["5"] = 2;
	if (_inputManager->keyStates["6"] == 0 && glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) _inputManager->keyStates["6"] = 2;
	if (_inputManager->keyStates["7"] == 0 && glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) _inputManager->keyStates["7"] = 2;
	if (_inputManager->keyStates["8"] == 0 && glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) _inputManager->keyStates["8"] = 2;
	if (_inputManager->keyStates["9"] == 0 && glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) _inputManager->keyStates["9"] = 2;
	if (_inputManager->keyStates["sp"] == 0 && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) _inputManager->keyStates["sp"] = 2;
	if (_inputManager->keyStates["lsh"] == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) _inputManager->keyStates["lsh"] = 2;
	if (_inputManager->keyStates["es"] == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) _inputManager->keyStates["es"] = 2;
	if (_inputManager->keyStates["lct"] == 0 && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) _inputManager->keyStates["lct"] = 2;
	if (_inputManager->keyStates["bksp"] == 0 && glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) _inputManager->keyStates["bksp"] = 2;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) _inputManager->keyStates["a"] = 0;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) _inputManager->keyStates["b"] = 0;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) _inputManager->keyStates["c"] = 0;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) _inputManager->keyStates["d"] = 0;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) _inputManager->keyStates["e"] = 0;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) _inputManager->keyStates["f"] = 0;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) _inputManager->keyStates["g"] = 0;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) _inputManager->keyStates["h"] = 0;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) _inputManager->keyStates["i"] = 0;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) _inputManager->keyStates["j"] = 0;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) _inputManager->keyStates["k"] = 0;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) _inputManager->keyStates["l"] = 0;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) _inputManager->keyStates["m"] = 0;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) _inputManager->keyStates["n"] = 0;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) _inputManager->keyStates["o"] = 0;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) _inputManager->keyStates["p"] = 0;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) _inputManager->keyStates["q"] = 0;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) _inputManager->keyStates["r"] = 0;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) _inputManager->keyStates["s"] = 0;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) _inputManager->keyStates["t"] = 0;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE) _inputManager->keyStates["u"] = 0;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) _inputManager->keyStates["v"] = 0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) _inputManager->keyStates["w"] = 0;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) _inputManager->keyStates["x"] = 0;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_RELEASE) _inputManager->keyStates["y"] = 0;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) _inputManager->keyStates["z"] = 0;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE) _inputManager->keyStates["0"] = 0;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) _inputManager->keyStates["1"] = 0;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) _inputManager->keyStates["2"] = 0;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) _inputManager->keyStates["3"] = 0;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) _inputManager->keyStates["4"] = 0;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE) _inputManager->keyStates["5"] = 0;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE) _inputManager->keyStates["6"] = 0;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_RELEASE) _inputManager->keyStates["7"] = 0;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_RELEASE) _inputManager->keyStates["8"] = 0;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_RELEASE) _inputManager->keyStates["9"] = 0;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) _inputManager->keyStates["sp"] = 0;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) _inputManager->keyStates["lsh"] = 0;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) _inputManager->keyStates["es"] = 0;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) _inputManager->keyStates["lct"] = 0;
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE) _inputManager->keyStates["bksp"] = 0;

}

//CALLBACKS
void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (_firstMouse) {
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	_mouseDiffX = xpos - _lastX;
	_mouseDiffY = _lastY - ypos;

	_lastX = xpos;
	_lastY = ypos;

	if (_menu->showMenu == false && _menu->showEGrid == false && _menu->showPauseMenu == false) {
		_camera->ProcessMouseMovement(_mouseDiffX, _mouseDiffY);
	}
}

void Game::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (_menu->showMenu == false && _menu->showEGrid == false && _menu->showPauseMenu == false) {
		_camera->ProcessMouseScroll(yoffset);
	}
	_inputManager->scrollOffset = yoffset;
}

void Game::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		_inputManager->leftMouseState = 2;
	}
	else {
		_inputManager->leftMouseState = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		_inputManager->rightMouseState = 2;
	}
	else {
		_inputManager->rightMouseState = 0;
	}
	
}

//DRAW FUNCTIONS
void Game::DrawSkybox(Shader* skyboxShader, unsigned int skyboxVAO, unsigned int cubemapTexture)
{
	glDepthMask(GL_FALSE);
	skyboxShader->use();
	skyboxShader->setMat4("projection", projection);
	skyboxShader->setMat4("view", glm::mat4(glm::mat3(view)));
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

glm::vec3 Game::GetMouseToWorldVector() {
	//glm::vec2 ndc = glm::vec2((2.0f * _lastX) / SCR_WIDTH - 1.0f, -((2.0f * _lastY) / SCR_HEIGHT - 1.0f));//normalised device coords
	//glm::vec4 clipCoords = glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);
	//glm::vec4 eyeCoords = clipCoords * glm::inverse(projection);
	//eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 1.0f);
	//glm::vec4 worldVector = eyeCoords * glm::inverse(view);
	//return glm::normalize(glm::vec3(worldVector.x, worldVector.y, worldVector.z));

	glm::vec2 ndc = glm::vec2((2.0f * _lastX) / SCR_WIDTH - 1.0f, -((2.0f * _lastY) / SCR_HEIGHT - 1.0f));//normalised device coords
	glm::mat4 toWorld = glm::inverse(projection * view);
	glm::vec4 from = toWorld * glm::vec4(ndc.x, ndc.y - 0.025f, -1.0f, 1.0f);
	glm::vec4 to = toWorld * glm::vec4(ndc.x, ndc.y - 0.025f, 1.0f, 1.0f);
	from /= from.w;
	to /= to.w;
	return glm::normalize(glm::vec3(to.x - from.x, to.y - from.y, to.z - from.z));

}

//float correction = (float)SCR_HEIGHT / (float)SCR_WIDTH;
//std::cout << correction << std::endl;
//glm::vec2 ndc = glm::vec2((2.0f * _lastX) / SCR_WIDTH - 1.0f, -((2.0f * _lastY) / SCR_HEIGHT - 1.0f));//normalised device coords
//glm::vec2 ndcAngles = glm::vec2(glm::radians(ndc.x * _camera->Zoom * 0.5f) * 1.71f, glm::radians(ndc.y * _camera->Zoom * 0.5f));
//glm::vec3 worldVector = _camera->Front * cos(ndcAngles.x) * cos(ndcAngles.y) + _camera->Right * sin(ndcAngles.x) * cos(ndcAngles.y) + _camera->Up * sin(ndcAngles.y) * 1.05f;
//return glm::normalize(worldVector);
