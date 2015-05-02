#include "Gamepad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

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
		if (joyButtons[0] == GL_TRUE)
		{
			printf("Pressed button X!\n");
		}

		if (joyButtons[1] == GL_TRUE)
		{
			printf("Pressed button A!\n");
		}

		if (joyButtons[2] == GL_TRUE)
		{
			printf("Pressed button B!\n");
		}

		if (joyButtons[3] == GL_TRUE)
		{
			printf("Pressed button Y!\n");
		}

		if (joyButtons[4] == GL_TRUE)
		{
			printf("Pressed button Left Shoulder!\n");
		}

		if (joyButtons[5] == GL_TRUE)
		{
			printf("Pressed button Right Shoulder!\n");
		}

		if (joyButtons[6] == GL_TRUE)
		{
			printf("Pressed Left Trigger!\n");
		}

		if (joyButtons[7] == GL_TRUE)
		{
			printf("Pressed button Right Trigger!\n");
		}

		if (joyButtons[8] == GL_TRUE)
		{
			printf("Pressed button Select!\n");
		}

		if (joyButtons[9] == GL_TRUE)
		{
			printf("Pressed button Start!\n");
		}

		if (joyButtons[10] == GL_TRUE)
		{
			printf("Pressed L3!\n");
		}

		if (joyButtons[11] == GL_TRUE)
		{
			printf("Pressed R3!\n");
		}

		if (joyButtons[12] == GL_TRUE)
		{
			printf("Pressed Dpad UP!\n");
		}

		if (joyButtons[13] == GL_TRUE)
		{
			printf("Pressed Dpad RIGHT!\n");
		}

		if (joyButtons[14] == GL_TRUE)
		{
			printf("Pressed Dpad DOWN!\n");
		}

		if (joyButtons[15] == GL_TRUE)
		{
			printf("Pressed Dpad LEFT!\n");
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