#ifndef GAMEPAD_H
#define GAMEPAD_H

#define DEADZONE 0.1f

using namespace std;

class Gamepad
{
public:
	enum Buttons
	{
		X = 0,
		A = 1,
		B = 2,
		Y = 3,
		LS = 4,
		RS = 5,
		LT = 6,
		RT = 7,
		Select = 8,
		Start = 9,
		L3 = 10,
		R3 = 11,
		Dpad_Up = 12,
		Dpad_Right = 13,
		Dpad_Down = 14,
		Dpad_Left = 15
	};

	enum Axes
	{
		Left_X = 0,
		Left_Y = 1,
		Right_X = 2,
		Right_Y = 3
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

private:
	void detectJoystick();
	char* getButtonSymbol(Buttons button);

private:
	bool debugging;
	int joyStick;
	int buttonCount = 0, axesCount = 0;
	const unsigned char* joyButtons = 0; // array containing button states
	const float* joyAxes = 0; // array containing axes X/Y values
	bool* buttonsStates; // used for "sticky keys"
};

#endif