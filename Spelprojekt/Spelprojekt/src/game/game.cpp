#include "game.h"
#include <ctime>
#include "../TimeQuery.h"

#include "../DebugLog.h"

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
			Debug::DebugOutput("---------------------opengl-callback-start------------\n");
			Debug::DebugOutput("message: %s\n", message);
			Debug::DebugOutput("type: ");
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				Debug::DebugOutput("ERROR");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				Debug::DebugOutput("DEPRECATED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				Debug::DebugOutput("UNDEFINED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				Debug::DebugOutput("PORTABILITY");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				Debug::DebugOutput("PERFORMANCE");
				break;
			case GL_DEBUG_TYPE_OTHER:
				Debug::DebugOutput("OTHER");
				break;
			}
			Debug::DebugOutput("\n");

			Debug::DebugOutput("id: %d\n", id);
			Debug::DebugOutput("severity: ");
			switch (severity){
			case GL_DEBUG_SEVERITY_LOW:
				Debug::DebugOutput("LOW");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				Debug::DebugOutput("MEDIUM");
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				Debug::DebugOutput("HIGH");
				break;
			}
			Debug::DebugOutput("\n");
			Debug::DebugOutput("---------------------opengl-callback-end--------------\n");
		}
	}

}
#endif

Game::~Game()
{
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
	if (updateAnimCheck)
		delete updateAnimCheck;

	//clean audio buffers
	Audio::getAudio().shutdown();
}

void Game::init(GLFWwindow* windowRef)
{
	// register callback
#ifdef _DEBUG
	if (glDebugMessageCallback){
		Debug::DebugOutput("Register OpenGL debug callback\n");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	}
	else
		Debug::DebugOutput("glDebugMessageCallback not available\n");
#endif

	frame = 0;

	viewMat = new glm::mat4(); //deleted in UserInput
	engine = new Engine();
	engine->init(viewMat);
	content = new ContentManager();
	content->init();

	updateAnimCheck = new UpdateAnimCheck();
	
	gamePad = new Gamepad();
	gamePad->init();
		
	player = new Player();
	player->init(gamePad);
	map = new Map();
	map->LoadMap(1, 0);
	map->init();
	in = new UserInput();
	glfwGetCursorPos(windowRef, &lastX, &lastY);
	in->Init(viewMat, glm::vec3(0, 0, 10), glm::vec3(0, 0, 9), glm::vec3(0, 1, 0));
	cameraFollow = true;
	edit = new Edit();
	edit->init(map, in);
	//start state
	cameraUpdate();
	current = MENU;


	//start audio
	Audio::getAudio().init(.5f, .5f, 1.0f, true, true, false);
	
	// read from settings file
	initSettings();

	// do not delete in this class
	this->windowRef = windowRef;

	savedStartPos = glm::vec2(-20, -20);
	checkForSave();

	gui = new GUI();
	if (savedStartPos.x != -20 && savedStartPos.y != -20)
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
		resetQuery();
		int timerID = startTimer("Frame");
		glfwPollEvents();

		deltaTime = 0.0166666f;
		readInput(deltaTime);

		update(deltaTime);

		int swapTimer = startTimer("Swap");
		glfwSwapBuffers(windowRef);
		stopTimer(swapTimer);

		fpsCount++;
		clock = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		deltaTime = clock - lastClock;
		lastClock = clock;

		stopTimer(timerID);
		terminateQuery();

		if (clock > 1)
		{
			start = std::clock();
			lastClock = 0;
			std::stringstream ss;
			ss << to_string(fpsCount);
			if (map)
			{
				int x, y;
				int ix = 0;
				int iy = 0;
				glm::vec3 campos = *in->GetPos();
				map->getChunkIndex(campos, &x, &y);
				if (x != -1 && y != -1)
				{
					ix = (int(campos.x + 17.5f) % 35);
					if (ix < 17)
						ix = -(17 - ix);
					else if (ix > 17)
						ix = ix - 17;
					else
						ix = 0;
					iy = (int(campos.y - 17.5f) % 35);
					iy *= -1;
					if (iy < 17)
						iy = (17 - iy);
					else if (iy > 17)
						iy = 17 - iy;
					else
						iy = 0;
				}
				ss << " :  Chunk: " << x << "_" << y << "  Index:  " << ix << "  " << iy;
			}

			//enable for time quarry result
			//cout << getQueryResult();
			Debug::DebugOutput("----\n%sFor frame %d\n----\n", getQueryResult().c_str(), frame);

			fpsCount = 0;
			glfwSetWindowTitle(windowRef, ss.str().c_str());
		}

		// will spam debugLog, large files are a risk
		//Debug::DebugOutput("----\n%sFor frame %d\n----\n", getQueryResult().c_str(), frame);

		frame++;

	}
}

void Game::update(float deltaTime)
{
	buttonEvents(gui->update((int)current));

	// update gamepad states
	if (gamePad->joyStickDetected())
		gamePad->update(deltaTime);

	//dept of field, on/off
	if (in->getKeyNumberState(9))
		engine->setDoF(true);
	if (in->getKeyNumberState(0))
		engine->setDoF(false);

	switch (current)
	{
		case(MENU) :
		{
			cameraUpdate();
			in->Act(deltaTime);
			map->update(deltaTime, player);
			map->setUpDraw3x2(*in->GetPos());
			engine->setFade(1.0f);
			Audio::getAudio().playMusic(MusicID::intro);
			Audio::getAudio().updateListener(player->readPos());
			break;
		}
		case(PLAY) :
		{
			//debug boss kill keys
			if (in->getKeyNumberState(1)){
				player->dingDongTheBossIsDead("Bossbat");
				gui->showNewUpgrade(1);
			}
			if (in->getKeyNumberState(2)){
				player->dingDongTheBossIsDead("Bossspider");
				gui->showNewUpgrade(2);
			}
			if (in->getKeyNumberState(3)){
				player->dingDongTheBossIsDead("Bossghost");
				gui->showNewUpgrade(3);
			}

					   // music
					   int tempX, tempY, tempId;
					   MapChunk** tempChunk = map->getChunks();
					   map->getChunkIndex(player->readPos(), &tempX, &tempY);
					   if (tempX != -1 && tempY != -1)
					   {
						   tempId = tempChunk[tempX][tempY].getMusicId();
						   if (tempId != NULL)//change music track
						   {
							   Audio::getAudio().playMusicFade(tempId, deltaTime);
						   }
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
								//glm::vec3 currentPos((3 * bossRoomMiddle.x) / 4.0f + playerPos.x / 4.0f, (2 * bossRoomMiddle.y) / 3.0f + playerPos.y / 3.0f, 0);
								glm::vec3 currentPos = playerPos;
								in->cameraPan(currentPos, 5, deltaTime, true);
								if (bossRoomMiddle.x + 10.0f > playerPos.x && bossRoomMiddle.x - 10.0f < playerPos.x &&
									bossRoomMiddle.y + 10.0f > playerPos.y && bossRoomMiddle.y - 10.0f < playerPos.y && !player->isBossFighting())
								{
									if (map->getBoss(playerPos, true) != "ChuckTesta")
									player->fightThatBossBro();
								}
						   }
						   player->update(in, map, gui, deltaTime);
						   Audio::getAudio().updateListener(player->readPos());
					   }
					   content->setPlayerState(player->getAnimState());
					   map->setUpDraw3x2(*in->GetPos());
					   
					   //Animations
					   static int updateAnim = 1;
					   if (updateAnim % 2 == 0)
						content->update(updateAnimCheck);
					   updateAnim++;

					   vec3 pPos = player->readPos();
					   int mapMsg = map->update(deltaTime, player);
					   if (!mapMsg)
					   {
						   inBossRoom = false;
						   if (player->isBossFighting())
						   {
							   player->dingDongTheBossIsDead("No boss at all");
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
							   if (boss == "Bossbat")
								   gui->showNewUpgrade(1);
							   else if (boss == "Bossspider")
								   gui->showNewUpgrade(2);
							   else if (boss == "Bossghost")
								   gui->showNewUpgrade(3);
							   Audio::getAudio().playSound(SoundID::boss_clear, false);//boss_defeted
						   }
						   Audio::getAudio().playMusicFade(-1, deltaTime);//stop music if the boss is dead
					   }
					   else if (mapMsg == 5)
					   {
						   player->execute(map);
					   }

					   //leave State code
					   if (in->getESC() || gamePad->isButtonPressedSticky(gamePad->getButtons().Start))
					   {
						   //save player progression
						   Audio::getAudio().playSound(SoundID::interface_pause, false); //pause
						   current = PAUSE;
					   }
					   break;
		}
		case(INTRO) :
		{
						if (gamePad->isButtonPressed(gamePad->getButtons().Start))
						{
							gui->setIntroState(6); //skip intro
						}

						break;
		}
		case(EDIT) :
		{
					   map->setUpDraw3x2(*in->GetPos());
					   edit->update(lastX, lastY, gui);

					   //load/save check
					   if (in->getLMBrelease())
					   {
						   bool load, save;
						   int nr;
						   edit->saveloadCheck(&save, &load, &nr);

						   if (load)
						   {
							   map->LoadMap(nr, 0);
							   edit->init(map, in);
						   }
						   if (save)
							   map->SaveMap(nr);
					   }
					   if (in->getESC())
					   {
						   //save map
						   current = MENU;
					   }
					   break;
		}
		case(PAUSE) :
		{
						if (in->getESC() || gamePad->isButtonPressedSticky(gamePad->getButtons().Start))
						{
							//save player progression
							Audio::getAudio().playSound(SoundID::interface_pause, false);
							current = PLAY;
						}
						break;
		}
		case(SETTINGS_MAIN) :
		{
						if (in->getESC())
						{
							//save player progression
							engine->setFade(0.0f);
							current = MENU;
						}
						break;
		}
		case(SETTINGS_AUDIO) :
		{
								if (in->getESC())
								{
									//save player progression
									current = SETTINGS_MAIN;
								}
								break;
		}
		case(SETTINGS_GRAPHICS) :
		{
								if (in->getESC())
								{
									//save player progression
									current = SETTINGS_MAIN;
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
	Audio::getAudio().update(deltaTime);

	//Render const
	updateAnimCheck->reset();
	engine->render(player, map, content, gui, in->GetPos(), (int)current, edit, updateAnimCheck);
}

void Game::buttonEvents(int buttonEv)
{
	switch (buttonEv)
	{
	case(0) : //default empty event;
		break;
	case(1) : // New game menu button
		map->LoadMap(1, 0);
		map->init();
		delete player;
		player = new Player();
		player->init(gamePad);
		gui->newPlayerRef(player);
		player->setStartPos(map->playerspawnX, map->playerspawnY);
		current = PLAY;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		cameraFollow = true;
		engine->setFadeIn();
		in->resetZoomViewDir();
		gui->showNewUpgrade(0); //show boss progression
		break;
		engine->setFade(1.0f);
	case(2) : // Mapmaker main button
		current = EDIT;
		edit->refreshOnEnter();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		Audio::getAudio().playMusic(-1); //stop menu music
		engine->setFadeIn();
		cameraFollow = false;
		break;
	case(3) : // Exit button. Play -> Menu
		saveGame();
		savedStartPos = glm::vec2(-20, -20);
		checkForSave();
		if (savedStartPos.x != -20 && savedStartPos.y != -20)
			gui->setGrayContinue(false);
		else
			gui->setGrayContinue(true);
		current = MENU;
		edit->refreshOnEnter();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		engine->setFade(0.0f);
		engine->setFadeOut();
		break;
	case(4) : // Continue menu button 
	{
		delete player;
		player = new Player();
		player->init(gamePad);
		gui->newPlayerRef(player);
		glm::vec2 pPos = savedStartPos;
		map->LoadMap(1, savedPickups);
		map->init();
		current = PLAY;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		engine->setFadeIn();
		cameraFollow = true;
		for (int c = 0; c < savePickupNr; c++)
		{
			player->getPickup(savedPickups[c]);
		}
		player->setProgress(playerProgress);
		player->moveTo(pPos.x, pPos.y);
		gui->showNewUpgrade(0); //show boss progression
	}
		break;

	case(5) : //intro
		current = INTRO;
		break;

	case(6) : // settings
		current = SETTINGS_MAIN;
		edit->refreshOnEnter();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;

	case(7) : // settings audio
		current = SETTINGS_AUDIO;
		edit->refreshOnEnter();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;

	case(8) : // settings graphics
		current = SETTINGS_GRAPHICS;
		edit->refreshOnEnter();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
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
	}

	//dof switches, always update
	state = glfwGetKey(windowRef, GLFW_KEY_9);
	state == GLFW_PRESS ? in->KeyNumberDown(9) : in->KeyNumberUp(9);
	state = glfwGetKey(windowRef, GLFW_KEY_0);
	state == GLFW_PRESS ? in->KeyNumberDown(0) : in->KeyNumberUp(0);
	
	//1 2 3 debug keys
	if (current == PLAY)
	{
		state = glfwGetKey(windowRef, GLFW_KEY_1);
		state == GLFW_PRESS ? in->KeyNumberDown(1) : in->KeyNumberUp(1);
		state = glfwGetKey(windowRef, GLFW_KEY_2);
		state == GLFW_PRESS ? in->KeyNumberDown(2) : in->KeyNumberUp(2);
		state = glfwGetKey(windowRef, GLFW_KEY_3);
		state == GLFW_PRESS ? in->KeyNumberDown(3) : in->KeyNumberUp(3);
	}


	//camera follow keys
	state = glfwGetKey(windowRef, GLFW_KEY_C);
	if (state == GLFW_PRESS)
	{
		if (current == PLAY)
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

void Game::initSettings()
{
	ifstream in;
	char* settings = "Config/settings.s";
	in.open(settings);
	if (in)
	{
		string line;
		string sub;
		stringstream ss;

		// --- Graphics ---
		getline(in, line);
		ss = stringstream(line);
		ss >> sub; //read past fullscreen line

		getline(in, line);
		ss = stringstream(line);
		ss >> sub; //read past resolution line

		getline(in, line);
		ss = stringstream(line);
		bool glows;
		ss >> sub;
		glows = atoi(sub.c_str());

		// apply graphic settings
		engine->applySettings(glows);
		
		// --- Audio ---
		getline(in, line);
		ss = stringstream(line);
		float musicV, soundV, audioV;
		ss >> sub;
		musicV = atof(sub.c_str());
		ss >> sub;
		soundV = atof(sub.c_str());
		ss >> sub;
		audioV = atof(sub.c_str());

		bool musicE, soundE, audioE;
		ss >> sub;
		musicE = atoi(sub.c_str());
		ss >> sub;
		soundE = atoi(sub.c_str());
		ss >> sub;
		audioE = atoi(sub.c_str());

		// apply audio settings
		Audio::getAudio().applySettings(musicV, soundV, audioV, musicE, soundE, audioE);

		in.close();
	}
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
			savedStartPos = chunks[xIndex][yIndex].shrine->getPos();
			for (int c = 0; c < 3; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				playerProgress.checkBossType(sub);
			}
			getline(in, line);
			ss = stringstream(line);
			ss >> sub;
			int nrOfPickups = atoi(sub.c_str());
			for (int c = 0; c < nrOfPickups; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				savedPickups[c].x = atoi(sub.c_str());
				ss >> sub;
				savedPickups[c].y = atoi(sub.c_str());
				savePickupNr++;
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
			out << playerProgress.nrOfPickups << "\n";
			for (int c = 0; c < playerProgress.nrOfPickups; c++)
			{
				out << playerProgress.pickups[c].x << " " << playerProgress.pickups[c].y << "\n";
			}
		}
		else
		{
			out << "-1 -1\nNo\nNo\nNo";
		}
		out.close();
	}
}

void Game::cameraUpdate()
{
	if (last != current || lastpos.x > (35 * 4) + 17.0f) // reset
	{
		lastpos = vec3(-20.0f, -35.0f * 6 + 2, 0);
	}

	lastpos.x += speed;
	in->setpos(lastpos, vec3(1,0,0));
}
