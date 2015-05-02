#include "Gamepad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

Gamepad::~Gamepad()
{

}

void Gamepad::init()
{
	detectJoystick();
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

			// get axes
			joyAxes = glfwGetJoystickAxes(joyStick, &axesCount);

			printf("Detected gamepad of type (%s) with %i buttons and %i axes.\n", joystickName, buttonCount, axesCount);

			break;
		}
	}
	
	if (joyStick == NULL)
		printf("Could not detect gamepad!");
}

void Gamepad::update(float deltaTime)
{
	// test code
	// get buttons
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
		// left stick
		if (joyAxes[0] > 0.1f)
		{
			printf("Left Stick X: %f\n", joyAxes[0]);
		}
		if (joyAxes[0] < -0.1f)
		{
			printf("Left Stick X: %f\n", joyAxes[0]);
		}
		if (joyAxes[1] > 0.1f)
		{
			printf("Left Stick Y: %f\n", joyAxes[1]);
		}
		if (joyAxes[1] < -0.1f)
		{
			printf("Left Stick Y: %f\n", joyAxes[1]);
		}
		// right stick
		if (joyAxes[2] > 0.1f)
		{
			printf("Right Stick X: %f\n", joyAxes[2]);
		}
		if (joyAxes[2] < -0.1f)
		{
			printf("Right Stick X: %f\n", joyAxes[2]);
		}
		if (joyAxes[3] > 0.1f)
		{
			printf("Right Stick Y: %f\n", joyAxes[3]);
		}
		if (joyAxes[3] < -0.1f)
		{
			printf("Right Stick Y: %f\n", joyAxes[3]);
		}
	}
}

bool Gamepad::joyStickDetected()
{
	if (joyStick == 0)
		return false;
	else
		return true;
}

bool Gamepad::isButtonPressed(int button)
{
	if (joyButtons[Buttons(button)])
	{
		printf("Pressed button: %s\n", getButtonSymbol(Buttons(button)));
	}
		
	return joyButtons[Buttons(button)];
}

char* Gamepad::getButtonSymbol(enum Buttons button)
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