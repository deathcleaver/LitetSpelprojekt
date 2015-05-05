#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <sstream>
#include <fstream>
#include <istream>

#define DEADZONE 0.2f

using namespace std;

class Gamepad
{
public:
	struct Buttons
	{
		int X = 0;
		int A = 1;
		int B = 2;
		int Y = 3;
		int LS = 4;
		int RS = 5;
		int LT = 6;
		int RT = 7;
		int Select = 8;
		int Start = 9;
		int L3 = 10;
		int R3 = 11;
		int Dpad_Up = 12;
		int Dpad_Right = 13;
		int Dpad_Down = 14;
		int Dpad_Left = 15;
	};

	struct Axes
	{
		int Left_X	= 0;
		int Left_Y	= 1;
		int Right_X = 2;
		int Right_Y = 3;
	};

public:
	Gamepad() {};
	~Gamepad();
	void init();
	void update(float deltaTime);
	bool joyStickDetected();
	// button checks
	bool isButtonPressed(int button);
	bool isButtonPressedSticky(int button);
	void getAxesValues(int axes, float &x, float &y);
	float getDeadzone(){ return DEADZONE; };
	Buttons getButtons(){ return buttonBinds; };
	Axes getAxes(){ return axesBinds; };

private:
	void detectJoystick();
	void loadConfig();
	void saveConfig();
	char* getButtonSymbol(int button);

private:
	bool debugging;
	int joyStick = -1;
	int buttonCount = 0, axesCount = 0;
	const unsigned char* joyButtons = 0; // array containing button states
	const float* joyAxes = 0; // array containing axes X/Y values
	bool* buttonsStates; // used for "sticky keys"

	//keybinds
	Buttons buttonBinds;
	Axes axesBinds;
};

#endif