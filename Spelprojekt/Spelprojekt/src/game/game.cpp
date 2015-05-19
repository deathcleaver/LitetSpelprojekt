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
	if (gamePad)
		delete gamePad;
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
	player->init();
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

	// do not delete in this class
	this->windowRef = windowRef;

	// read from settings file
	initSettings();

	savedStartPos = glm::vec2(-20, -20);
	for (int c = 0; c < 12; c++)
		savedPickups[c].x = savedPickups[c].y = -1;
	checkForSave();

	gui = new GUI();
	if (savedStartPos.x != -20 && savedStartPos.y != -20)
		gui->init(in, player, content, false);
	else
		gui->init(in, player, content);

	freeze = false;
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
		
		if (freeze)
			deltaTime = 0.0f;

		readInput(deltaTime);
		
		update(deltaTime);

		int swapTimer = startTimer("Swap");
		glfwSwapBuffers(windowRef);
		stopTimer(swapTimer);

		fpsCount++;
		clock = float((std::clock() - start) / (double)CLOCKS_PER_SEC);
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

			Debug::DebugOutput("----\nMirror calls: %d\n"
				"Render calls: %d\n"
				"Enemy draw: %d\n"
				"World draw: %d\n"
				"Misc draw: %d\n"
				"----\n", engine->mirrorDraw, engine->renderCall, engine->enemyDraw, engine->worldDraw, engine->miscDraw);

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
	if (gamePad->joyStickDetected() && !(playerintrorun && current == PLAY))
		gamePad->update(deltaTime);

	//dept of field, on/off
	if (in->getKeyNumberState(9))
		engine->setDoF(true);
	if (in->getKeyNumberState(0))
		engine->setDoF(false);

	if (GameConfig::get().configFullscreen && (current != PLAY && current != INTRO))
		glfwSetInputMode(windowRef, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (finalbossToCredits)
	{
		finalbossFreeze--;
		if (finalbossFreeze <= 0)
		{
			freeze = false;
			finalbossToCredits = false;
			buttonEvents(CREDITS);
		}
	}

	switch (current)
	{
		case(MENU) :
		{
			cameraUpdate();
			in->Act(deltaTime);
			map->update(deltaTime, player);
			map->setUpDrawMainMenuCamera(*in->GetPos());
			engine->setFade(1.0f);
			Audio::getAudio().playMusic(MusicID::intro);
			Audio::getAudio().updateListener(player->readPos());
			if (gamePad->isButtonPressedSticky(gamePad->getButtons().Start))
				buttonEvents(12); // difficulty
			break;
		}
		case(CREDITS) :
		{
			cameraUpdateCredits();
			in->Act(deltaTime);
			map->setUpDrawCredits(*in->GetPos());
			map->update(deltaTime, player);
			//Animations
			static int updateAnim = 1;
			if (updateAnim % 2 == 0)
				content->update(updateAnimCheck);
			updateAnim++;
			engine->setFade(1.0f);
			Audio::getAudio().playMusic(MusicID::credits);
			Audio::getAudio().updateListener(player->readPos());

			if (in->getESC())
				current = MENU;
			break;
		}
		case(PLAY) :
		{
			if (GameConfig::get().configFullscreen == true)
				glfwSetInputMode(windowRef, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			
			//debug boss kill keys
			/*
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
			*/

			if (playerintrorun)
			{
				if (player->readPos().x > 105.0f)
					playerintrorun = false;
				in->KeyDown('D');
				in->KeyUp('A');
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
						   int msg = player->update(in, gamePad, map, gui, deltaTime);
						   Audio::getAudio().updateListener(player->readPos());
						   glm::vec3 playerPos = player->readPos();
						   if (msg == 1)
						   {
							   in->cameraPan(glm::vec3(103, -13, 0), 5, deltaTime, gamePad, true);
						   }
						   else if (!inBossRoom)
						   {
							   in->cameraPan(playerPos, 5, deltaTime, gamePad, true);
						   }
						   else
						   {
								glm::vec3 currentPos((3 * bossRoomMiddle.x) / 4.0f + playerPos.x / 4.0f, (2 * bossRoomMiddle.y) / 3.0f + playerPos.y / 3.0f, 0);
								if (map->getBoss(playerPos, false) == "Grim")
									currentPos = playerPos;
								in->cameraPan(currentPos, 5, deltaTime, gamePad, true);
								if (bossRoomMiddle.x + 9.0f > playerPos.x && bossRoomMiddle.x - 9.0f < playerPos.x &&
									bossRoomMiddle.y + 10.0f > playerPos.y && bossRoomMiddle.y - 10.0f < playerPos.y && !player->isBossFighting())
								{
									if (map->getBoss(playerPos, true) != "ChuckTesta")
									player->fightThatBossBro();
								}
						   }
						   if (GameConfig::get().configFirstperson)
						   {
							   vec3 ppos = player->readPos();
							   ppos.y += 0.5f;
							   if (player->getSpeed().x > 0.001)
								   lastdirFPS = vec3(1, 0, 0);
							   else if (player->getSpeed().x < -0.001)
								   lastdirFPS = vec3(-1, 0, 0);
							
							   if (lastdirFPS.x > 0)
								   ppos.x += 0.25f;
							   else
								   ppos.x -= 0.25f;
							   in->setpos(ppos, lastdirFPS);
							  
						   }

						   if (msg == 5)
						   {
							   engine->setFadeOut();
							   freeze = true;
						   }
						   else if (msg == 6)
						   {
							   glm::vec3 pos = player->getTelePos();
							   pos.z = 10;
							   in->setpos(pos, glm::vec3(0, 0, -1));
							   engine->setFadeIn();
							   freeze = false;
						   }
					   }
					   content->setPlayerState(player->getAnimState());
					   map->setUpDraw3x2(*in->GetPos());
					   
					   //Animations
					   static int updateAnim = 1;
					   if (updateAnim % 2 == 0 && !freeze)
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
							   else if (boss == "Grim")
							   {
								   freeze = true;
								   gui->showNewUpgrade(4);
								   engine->setFadeOut();
								   finalbossFreeze = finalbossFreezeTimer;
								   finalbossToCredits = true;
							   }

							   if (boss != "Grim")
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
						if (GameConfig::get().configFullscreen == true)
							glfwSetInputMode(windowRef, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

						if (gamePad->isButtonPressedSticky(gamePad->getButtons().Start))
						{
							gui->setIntroState(6); //skip intro
						}

						break;
		}
		case(EDIT) :
		{
					   map->setUpDraw3x2(*in->GetPos());
					   edit->update((float)lastX, (float)lastY, gui);

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
						cameraUpdate();
						if (in->getESC())
						{
							//save player progression
							saveSettings();
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
		case(DIFFICULTY) :
		{
							cameraUpdate();
							if (in->getESC())
							{
								//save player progression
								current = MENU;
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
			in->Mouse((float)(xpos - lastX), (float)(ypos - lastY)); //moves mouse
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
		player->init();
		playerintrorun = true;
		in->setpos(vec3(60, -2, -13), vec3(0, 0, -1));
		in->resetZoomViewDir();
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
		playerintrorun = false;
		delete player;
		player = new Player();
		player->init();
		in->setpos(player->readPos(), vec3(0,0,-1));
		in->resetZoomViewDir();
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
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;

	case(7) : // toggle audio
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		Audio::getAudio().toggleAudio();
		if (Audio::getAudio().getAudioEnabled())
			Audio::getAudio().playMusic(MusicID::intro);
		break;

	case(8) : // toggle music
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		Audio::getAudio().toggleMusic();
		if (Audio::getAudio().getMusicEnabled())
			Audio::getAudio().playMusic(MusicID::intro);
		break;

	case(9) : // toggle sound
		Audio::getAudio().toggleSound();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(10) : // toggle resolution
		toggleFullscreen();
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(11) : // Credits
		current = CREDITS;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(12) : // difficulty
		current = DIFFICULTY;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(13) : // casual
		current = INTRO;
		GameConfig::get().configDifficulty = GameConfig::Casual;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(14) : // hardcore
		current = INTRO;
		GameConfig::get().configDifficulty = GameConfig::Hardcore;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(15) : // DmonInHell
		current = INTRO;
		GameConfig::get().configDifficulty = GameConfig::DmonInHell;
		Audio::getAudio().playSound(SoundID::interface_button, false); //button
		break;
	case(16) : // toggle fps mode
		if (GameConfig::get().configFirstperson)
			GameConfig::get().configFirstperson = false;
		else
			GameConfig::get().configFirstperson = true;
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
	in->setMousePos((float)xpos, (float)ypos);

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
	state = glfwGetKey(windowRef, GLFW_KEY_X);
	state == GLFW_PRESS ? in->KeyDown('X') : in->KeyUp('X');

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

		state = glfwGetKey(windowRef, GLFW_KEY_7);
		if (state == GLFW_PRESS)
			GameConfig::get().configFirstperson = true;
		state = glfwGetKey(windowRef, GLFW_KEY_8);
		if (state == GLFW_PRESS)
		{
			GameConfig::get().configFirstperson = false;
			in->resetZoomViewDir();
		}
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
		getline(in, line); // fullscreen
		ss = stringstream(line);
		ss >> sub;
		GameConfig::get().configFullscreen = atoi(sub.c_str()) ? 1 : 0;

		getline(in, line); // fps
		ss = stringstream(line);
		ss >> sub;
		GameConfig::get().configFirstperson = atoi(sub.c_str()) ? 1 : 0;

		getline(in, line); // resolution
		ss = stringstream(line);
		ss >> sub;
		GameConfig::get().configResX = atoi(sub.c_str());
		ss >> sub;
		GameConfig::get().configResY = atoi(sub.c_str());

		getline(in, line); // glows enabled
		ss = stringstream(line);
		ss >> sub;
		GameConfig::get().configRenderGlow = atoi(sub.c_str()) ? 1 : 0;

		// apply graphic settings
		engine->applySettings(GameConfig::get().configRenderGlow);
		
		// --- Audio ---
		getline(in, line);
		ss = stringstream(line);
		float musicV, soundV, audioV;
		ss >> sub;
		musicV = (float)atof(sub.c_str());
		ss >> sub;
		soundV = (float)atof(sub.c_str());
		ss >> sub;
		audioV = (float)atof(sub.c_str());

		bool musicE, soundE, audioE;
		ss >> sub;
		musicE = atoi(sub.c_str()) ? 1 : 0;
		ss >> sub;
		soundE = atoi(sub.c_str()) ? 1 : 0;
		ss >> sub;
		audioE = atoi(sub.c_str()) ? 1 : 0;

		// apply audio settings
		Audio::getAudio().applySettings(musicV, soundV, audioV, musicE, soundE, audioE);

		// toggle fullscreen WIP
		//toggleFullscreen();

		in.close();
	}
}

void Game::toggleFullscreen()
{
	if (GameConfig::get().configFullscreen)
		GameConfig::get().configFullscreen = false;
	else
		GameConfig::get().configFullscreen = true;

	//if (windowRef != NULL)
	//{
	//	glfwInit();
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	//	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	//	glfwWindowHint(GLFW_RESIZABLE, false);

	//	GLFWwindow* newWindow = glfwCreateWindow(GameConfig::get().configResX, GameConfig::get().configResY, "ASUM PROJECT 2", NULL, windowRef);
	//	glfwDestroyWindow(windowRef);

	//	windowRef = newWindow;
	//	
	//	glfwMakeContextCurrent(windowRef);
	//	glfwSwapInterval(0);

	//	//glfwMakeContextCurrent(newWindow);
	//	//glewInit();

	//	//windowRef = newWindow;

	//	//glfwDestroyWindow(windowRef);
	//}
}

void Game::saveSettings()
{
	ofstream out;
	char* settings = "Config/settings.s";
	out.open(settings, ios::trunc);
	if (out)
	{
		float mV = 1.0f, sV = 1.0f, aV = 1.0f;
		bool mE = true, sE = true, aE = true;

		mV = Audio::getAudio().getMusicVolume();
		sV = Audio::getAudio().getSoundVolume();
		aV = Audio::getAudio().getMasterVolume();

		mE = Audio::getAudio().getMusicEnabled();
		sE = Audio::getAudio().getSoundEnabled();
		aE = Audio::getAudio().getAudioEnabled();

		out << GameConfig::get().configFullscreen << " // fullscreen\n";
		out << GameConfig::get().configFirstperson << " // firstperson\n";
		out << GameConfig::get().configResX << " " << GameConfig::get().configResY << " // resolution\n";
		out << GameConfig::get().configRenderGlow << " // render glow\n";
		out << mV << " " << sV << " " << aV << " " << mE << " " << sE << " " << aE << " // audio";

		out.close();
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
		GameConfig::get().configDifficulty = atoi(sub.c_str());
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
			savePickupNr = 0;
			getline(in, line);
			ss = stringstream(line);
			ss >> sub;
			int nrOfPickups = atoi(sub.c_str());
			for (int c = 0; c < nrOfPickups; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				savedPickups[c].x = (float)atoi(sub.c_str());
				ss >> sub;
				savedPickups[c].y = (float)atoi(sub.c_str());
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
			out << GameConfig::get().configDifficulty << "\n";
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
			out << "0\n-1 -1\nNo\nNo\nNo";
		}
		out.close();
	}
}

void Game::cameraUpdate()
{
	if ((last != current  && (current != DIFFICULTY && current != SETTINGS_MAIN)) || lastpos.x > (35 * 3) + 17.0f) // reset
	{
		lastpos = vec3(-25.0f, -35.0f * 6 + 1.75f, 0);
	}

	lastpos.x += speed;
	in->setpos(lastpos, vec3(1, 0, 0));
}

void Game::cameraUpdateCredits()
{
	if (last != current) 
		lastposCredits = vec3( 35 * 7, -35 * 3 + 17.5f, 10);

	if (lastposCredits.y < -((35 * 6) + 17.0f))
	{
		speed = 0;
	}
	else
	{
		lastposCredits.y -= speed;
		in->setpos(lastposCredits, vec3(0, 0, -1));
	}
}
