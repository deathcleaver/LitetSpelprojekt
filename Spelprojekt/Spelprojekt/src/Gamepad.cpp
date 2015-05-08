#include "Gamepad.h"
#include <GLFW/glfw3.h>
#include "DebugLog.h"

Gamepad::~Gamepad()
{

}

void Gamepad::init()
{
	debugging = false;
	detectJoystick();

	if (joyStickDetected())
		loadConfig();
}

void Gamepad::detectJoystick()
{
	const char* joystickName;
	for (int i = 0; i < 15; i++) //glfw supports 16 joystick thus i < 15
	{
		joystickName = glfwGetJoystickName(i);
		if (joystickName != NULL)
		{
			joyStick = i;

			// get buttons
			joyButtons = glfwGetJoystickButtons(joyStick, &buttonCount);
			buttonsStates = new bool[buttonCount];

			// get axes
			joyAxes = glfwGetJoystickAxes(joyStick, &axesCount);

			Debug::DebugOutput("Detected gamepad of type (%s) with %i buttons and %i axes.\n", joystickName, buttonCount, axesCount);

			break;
		}
	}
	
	if (joyStick == -1)
		Debug::DebugOutput("Could not detect gamepad!\n");
}

void Gamepad::loadConfig()
{
	ifstream in;
	char* config = "Config/Gamepad/gamepad_xinput.s";
	in.open(config);
	if (in)
	{
		string line;
		string sub;
		stringstream ss;

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.X = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.A = atoi(sub.c_str());
		
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.B = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Y = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.LS = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.RS = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.LT = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.RT = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Select = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Start = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.L3 = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.R3 = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Dpad_Up = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Dpad_Right = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Dpad_Down = atoi(sub.c_str());

		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		buttonBinds.Dpad_Left = atoi(sub.c_str());

		in.close();
	}
}

void Gamepad::update(float deltaTime)
{
	// test code
	// get buttons
	if (joyStick > -1)
	{
		joyButtons = glfwGetJoystickButtons(joyStick, &buttonCount);

		// get axes
		joyAxes = glfwGetJoystickAxes(joyStick, &axesCount);

		if (joyButtons != NULL && joyAxes != NULL)
		{

			// buttons test
			for (int i = 0; i < buttonCount; i++)
			{
				isButtonPressed(i);
			}

			// axes
			float x, y;

			// left stick
			getAxesValues(0, x, y);

			// right stick
			getAxesValues(1, x, y);
		}
	}
}

bool Gamepad::joyStickDetected()
{
	if (joyStick == -1)
		return false;
	else
		return true;
}

bool Gamepad::isButtonPressed(int button)
{
	if (joyStick > -1)
	{
		if (joyButtons != NULL)
		{
			if (joyButtons[button])
			{
				if (debugging)
					Debug::DebugOutput("Pressed button: %s\n", getButtonSymbol(button));

				return joyButtons[button];
			}
		}
	}	
		
	return GL_FALSE;
}

bool Gamepad::isButtonPressedSticky(int button) // "sticky keys for the gamepad"
{
	if (joyStick > -1)
	{
		if (joyButtons != NULL)
		{
			if (joyButtons[button])
			{
				if (debugging)
					Debug::DebugOutput("Pressed button: %s\n", getButtonSymbol(button));

				if (buttonsStates[button] == false) // first time pressed
				{
					buttonsStates[button] = true;
					return true;
				}
				else
					return false;
			}
			else
			{
				buttonsStates[button] = false;
			}
		}
	}
	
	return GL_FALSE;
}

void Gamepad::getAxesValues(int axes, float &x, float &y)
{
	if (joyStick != -1)
	{
		if (joyAxes != NULL) // gamepad detected
		{
			if (axes == 0) // left stick
			{
				if (x > DEADZONE || x < -DEADZONE)
					x = joyAxes[0];
				else
					x = 0;

				if (y > DEADZONE || y < -DEADZONE)
					y = joyAxes[1];
				else y = 0;
			}
			else if (axes == 1) // right stick
			{
				if (x > DEADZONE || x < -DEADZONE)
					x = joyAxes[2];
				else
					x = 0;

				if (y > DEADZONE || y < -DEADZONE)
					y = joyAxes[3];
				else y = 0;
			}
			else // no gamepad detected
			{
				x = 0;
				y = 0;
			}

			if (debugging)
				Debug::DebugOutput("Left Stick:(%f,%f)\n", x, y);
		}
	}
	else // no gamepad detected
	{
		x = 0;
		y = 0;
	}
}

char* Gamepad::getButtonSymbol(int button)
{
	switch (button)
	{
		case 0: return "X";
		case 1: return "A";
		case 2: return "B";
		case 3: return "Y";
		case 4: return "LS";
		case 5: return "RS";
		case 6: return "LT";
		case 7: return "RT";
		case 8: return "Select";
		case 9: return "Start";
		case 10: return "L3";
		case 11: return "R3";
		case 12: return "D-pad up";
		case 13: return "D-pad right";
		case 14: return "D-pad down";
		case 15: return "D-pad left";
	}

	return "null";
}