#include "game.h"
#include <ctime>
//#include <iostream>
//#include <Windows.h>

// opengl debug callback

#ifdef _DEBUG
extern "C"
{
	void APIENTRY openglCallbackFunction(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
		{
			std::cout << "---------------------opengl-callback-start------------" << std::endl;
			std::cout << "message: " << message << std::endl;
			std::cout << "type: ";
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				std::cout << "ERROR";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				std::cout << "DEPRECATED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				std::cout << "UNDEFINED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				std::cout << "PORTABILITY";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				std::cout << "PERFORMANCE";
				break;
			case GL_DEBUG_TYPE_OTHER:
				std::cout << "OTHER";
				break;
			}
			std::cout << std::endl;

			std::cout << "id: " << id << std::endl;
			std::cout << "severity: ";
			switch (severity){
			case GL_DEBUG_SEVERITY_LOW:
				std::cout << "LOW";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				std::cout << "MEDIUM";
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				std::cout << "HIGH";
				break;
			}
			std::cout << std::endl;
			std::cout << "---------------------opengl-callback-end--------------" << std::endl;
		}
	}

}
#endif

Game::~Game()
{
	saveGame();
	if (engine)
		delete engine;
	if (content)
		delete content;
	if (player)
		delete player;
	if (map)
		delete map;
	if (in)
		delete in;
	if (gui)
		delete gui;
	if (edit)
		delete edit;

	if (audio)
	{
		audio->shutdown();
		delete audio;
	}
}

void Game::init(GLFWwindow* windowRef)
{
	// register callback
#ifdef _DEBUG
	if (glDebugMessageCallback){
		std::cout << "Register OpenGL debug callback " << std::endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	}
	else
		std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

	viewMat = new glm::mat4(); //deleted in UserInput
	engine = new Engine();
	engine->init(viewMat);
	content = new ContentManager();
	content->init();
	player = new Player();
	player->init();
	map = new Map();
	map->init();
	in = new UserInput();
	glfwGetCursorPos(windowRef, &lastX, &lastY);
	in->Init(viewMat, glm::vec3(0, 0, 10), glm::vec3(0, 0, 9), glm::vec3(0, 1, 0));
	cameraFollow = true;
	edit = new Edit();
	edit->init(map, in);
	//start state
	current = MENU;

	//start audio
	audio = new Audio();
	audio->init();

	// do not delete in this class
	this->windowRef = windowRef;

	start = 0;
	checkForSave();
	gui = new GUI();
	if (start)
		gui->init(in, player, content, false);
	else
		gui->init(in, player, content);
}

void Game::mainLoop()
{
	clock_t start = clock();
	//how much of a second have passed since last frame
	float deltaTime = 0.0f;
	float clock;
	float lastClock = 0.0f;
	int fpsCount = 0;
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(windowRef))
	{
		glfwPollEvents();

		deltaTime = 0.0166666f;
		readInput(deltaTime);

		update(deltaTime);

		glfwSwapBuffers(windowRef);

		fpsCount++;
		clock = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		deltaTime = clock - lastClock;
		lastClock = clock;
		
		if (clock > 1)
		{
			start = std::clock();
			lastClock = 0;
			std::string s = std::to_string(fpsCount);
			fpsCount = 0;
			glfwSetWindowTitle(windowRef, s.c_str());
		}
	}
}

void Game::update(float deltaTime)
{
	buttonEvents(gui->update((int)current));
	switch(current) 
	{
		case(MENU):
		{

					  engine->setFade(0.0f);
					  audio->playMusic(0);
					  audio->updateListener(player->readPos());
			break;
		}
		case(PLAY):
		{
					  // music
					  int tempX, tempY, tempId;
					  MapChunk** tempChunk = map->getChunks();
					  map->getChunkIndex(player->readPos(), &tempX, &tempY);
					  tempId = tempChunk[tempX][tempY].getMusicId();
					  if (tempId != NULL)//change music track
					  {
							  audio->playMusicFade(tempId, deltaTime);
					  }
						  
			if (cameraFollow)
			{
				glm::vec3 playerPos = player->readPos();
				if (!inBossRoom)
				{
					in->cameraPan(playerPos, 5, deltaTime, true);
				}
				else
				{
					glm::vec3 currentPos((3 * bossRoomMiddle.x) / 4.0f + playerPos.x / 4.0f, (2 * bossRoomMiddle.y) / 3.0f + playerPos.y / 3.0f, 0);
					in->cameraPan(currentPos, 5, deltaTime, true);
					if (bossRoomMiddle.x + 10.0f > playerPos.x && bossRoomMiddle.x - 10.0f < playerPos.x &&
						bossRoomMiddle.y + 10.0f > playerPos.y && bossRoomMiddle.y - 10.0f < playerPos.y && !player->isBossFighting())
					{
						if (map->getBoss(playerPos, true) != "ChuckTesta")
							player->fightThatBossBro();
					}
				}
				player->update(in, map, audio, deltaTime);
				audio->updateListener(player->readPos());
			}
			content->setPlayerState(player->getAnimState());
			map->setUpDraw3x2(*in->GetPos());
			//Animations
			content->update();
			vec3 pPos = player->readPos();
			int mapMsg = map->update(deltaTime, player);
			if (!mapMsg)
			{
				inBossRoom = false;
				if (player->isBossFighting())
				{
					player->dingDongTheBossIsDead("No boss at all");
					audio->playMusicFade(-1, deltaTime); //don't play any music since the boss is dead
				}
					
			}
			else if (mapMsg == 1)
			{
				bossRoomMiddle = map->getChunkMiddle(pPos);
				inBossRoom = true;
			}
			else if (mapMsg == 2)
			{
				inBossRoom = false;
				if (player->isBossFighting())
				{
					std::string boss = map->getBoss(pPos, false);
					player->dingDongTheBossIsDead(boss);
				}
			}
			else if (mapMsg == 5)
			{
				player->execute(map);
			}

			//leave State code
			if (in->getESC())
			{
				//save player progression
				audio->playSound(7); //pause
				current = PAUSE;
			}
			break;
		}
		case(INTRO):
		{
			break;
		}
		case(EDIT):
		{
			map->setUpDraw3x2(*in->GetPos());
			edit->update(lastX, lastY, gui);
			if (in->getESC())
			{
				//save map
				current = MENU;
			}
			break;
		}
		case(PAUSE):
		{
			if (in->getESC())
			{
				//save player progression
				current = PLAY;
			}
			break;
		}
	}
	last = current;

	glfwGetCursorPos(windowRef, &xpos, &ypos);
	if (!cameraFollow)
	{
		in->Act(deltaTime); //moves sideways
		
		if (in->updateMouse())
			in->Mouse(xpos - lastX, ypos - lastY); //moves mouse
	}
	lastX = xpos;
	lastY = ypos;

	//update audio
	audio->update(deltaTime);

	//Render const
	engine->render(player, map, content, gui, in->GetPos(),(int)current, edit);
}

void Game::buttonEvents(int buttonEv)
{
	switch (buttonEv)
	{
	case(0) : //default empty event;
		break;
	case(1) :
		current = PLAY;
		audio->playSound(6); //button
		cameraFollow = true;
		engine->setFadeIn();
		in->resetZoomViewDir();
		break;
	case(2) :
		engine->setFade(1.0f);
		current = EDIT;
		edit->refreshOnEnter();
		audio->playSound(6); //button
		cameraFollow = false;
		break;
	case(3) :
		saveGame();
		current = MENU;
		edit->refreshOnEnter();
		audio->playSound(6); //button
		engine->setFade(0.0f);
		engine->setFadeOut();
		break;
	case(4) :
		current = PLAY;
		audio->playSound(6); //button
		engine->setFadeIn();
		cameraFollow = true;
		player->setProgress(playerProgress);
		player->moveTo(start->returnThis()->readPos());
		break;
	}
	//Editor buttons
	if (buttonEv > 99)
		edit->guiHandle(buttonEv);
}

void Game::readInput(float deltaTime)
{
	int state;
	//Mouse Buttons
	state = glfwGetMouseButton(windowRef, GLFW_MOUSE_BUTTON_RIGHT);
	state == GLFW_PRESS ? in->RMB(true) : in->RMB(false);
	state = glfwGetMouseButton(windowRef, GLFW_MOUSE_BUTTON_LEFT);
	state == GLFW_PRESS ? in->LMB(true) : in->LMB(false);
	glfwGetCursorPos(windowRef, &xpos, &ypos);
	in->setMousePos(xpos, ypos);

	//Character Keys
	state = glfwGetKey(windowRef, GLFW_KEY_W);
	state == GLFW_PRESS ? in->KeyDown('W') : in->KeyUp('W');
	state = glfwGetKey(windowRef, GLFW_KEY_A);
	state == GLFW_PRESS ? in->KeyDown('A') : in->KeyUp('A');
	state = glfwGetKey(windowRef, GLFW_KEY_S);
	state == GLFW_PRESS ? in->KeyDown('S') : in->KeyUp('S');
	state = glfwGetKey(windowRef, GLFW_KEY_D);
	state == GLFW_PRESS ? in->KeyDown('D') : in->KeyUp('D');
	state = glfwGetKey(windowRef, GLFW_KEY_G);
	state == GLFW_PRESS ? in->KeyDown('G') : in->KeyUp('G');
	state = glfwGetKey(windowRef, GLFW_KEY_M);
	state == GLFW_PRESS ? in->KeyDown('M') : in->KeyUp('M');
	state = glfwGetKey(windowRef, GLFW_KEY_C);
	state == GLFW_PRESS ? in->KeyDown('C') : in->KeyUp('C');
	state = glfwGetKey(windowRef, GLFW_KEY_N);
	state == GLFW_PRESS ? in->KeyDown('N') : in->KeyUp('N');
	state = glfwGetKey(windowRef, GLFW_KEY_Q);
	state == GLFW_PRESS ? in->KeyDown('Q') : in->KeyUp('Q');
	state = glfwGetKey(windowRef, GLFW_KEY_E);
	state == GLFW_PRESS ? in->KeyDown('E') : in->KeyUp('E');

	//Editor Keys
	if (current == EDIT)
	{
		state = glfwGetKey(windowRef, GLFW_KEY_1);
		state == GLFW_PRESS ? in->KeyNumberDown(1) : in->KeyNumberUp(1);
		state = glfwGetKey(windowRef, GLFW_KEY_2);
		state == GLFW_PRESS ? in->KeyNumberDown(2) : in->KeyNumberUp(2);
		state = glfwGetKey(windowRef, GLFW_KEY_3);
		state == GLFW_PRESS ? in->KeyNumberDown(3) : in->KeyNumberUp(3);
		state = glfwGetKey(windowRef, GLFW_KEY_4);
		state == GLFW_PRESS ? in->KeyNumberDown(4) : in->KeyNumberUp(4);
		state = glfwGetKey(windowRef, GLFW_KEY_5);
		state == GLFW_PRESS ? in->KeyNumberDown(5) : in->KeyNumberUp(5);
		state = glfwGetKey(windowRef, GLFW_KEY_6);
		state == GLFW_PRESS ? in->KeyNumberDown(6) : in->KeyNumberUp(6);
		state = glfwGetKey(windowRef, GLFW_KEY_7);
		state == GLFW_PRESS ? in->KeyNumberDown(7) : in->KeyNumberUp(7);
		state = glfwGetKey(windowRef, GLFW_KEY_8);
		state == GLFW_PRESS ? in->KeyNumberDown(8) : in->KeyNumberUp(8);
		state = glfwGetKey(windowRef, GLFW_KEY_9);
		state == GLFW_PRESS ? in->KeyNumberDown(9) : in->KeyNumberUp(9);
		state = glfwGetKey(windowRef, GLFW_KEY_0);
		state == GLFW_PRESS ? in->KeyNumberDown(0) : in->KeyNumberUp(0);
	}
	
	//camera follow keys
	state = glfwGetKey(windowRef, GLFW_KEY_C);
	if (state == GLFW_PRESS) 
	{ 
		if(current == PLAY)
			cameraFollow = true;
		in->resetZoomViewDir();
	};
	state = glfwGetKey(windowRef, GLFW_KEY_V);
	if (state == GLFW_PRESS)
		cameraFollow = false;

	//Special Keys
	state = glfwGetKey(windowRef, GLFW_KEY_LEFT_SHIFT);
	state == GLFW_PRESS ? in->Shift(true) : in->Shift(false);
	state = glfwGetKey(windowRef, GLFW_KEY_SPACE);
	state == GLFW_PRESS ? in->Space(true) : in->Space(false);
	state = glfwGetKey(windowRef, GLFW_KEY_LEFT_CONTROL);
	state == GLFW_PRESS ? in->Ctrl(true) : in->Ctrl(false);
	state = glfwGetKey(windowRef, GLFW_KEY_ESCAPE);
	state == GLFW_PRESS ? in->ESC(true) : in->ESC(false);
}

void Game::checkForSave()
{
	ifstream in;
	char* save = "save.s";
	int xIndex = -1, yIndex = -1;
	in.open(save);
	if (in)
	{
		string line;
		string sub;
		stringstream ss;
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		xIndex = atoi(sub.c_str());
		ss >> sub;
		yIndex = atoi(sub.c_str());
		if (xIndex != -1 && yIndex != -1)
		{
			MapChunk** chunks = map->getChunks();
			start = chunks[xIndex][yIndex].shrine;
			for (int c = 0; c < 3; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				playerProgress.checkBossType(sub);
			}
		}
		in.close();
	}
}

void Game::saveGame()
{
	Shrine* savePoint = player->getCurrentSpawn();
	playerProgress = player->getProgress();
	ofstream out;
	out.open("save.s", ios::trunc);
	if (out)
	{
		if (savePoint)
		{
			int idX, idY;
			glm::vec3 savePos = savePoint->returnThis()->readPos();
			map->getChunkIndex(savePos, &idX, &idY);
			out << idX << " " << idY << "\n";
			for (int c = 0; c < 3; c++)
			{
				out << playerProgress.getBossType(c) << "\n";
			}
		}
		else
		{
			out << "-1 -1\nNo\nNo\nNo";
		}
		out.close();
	}
}