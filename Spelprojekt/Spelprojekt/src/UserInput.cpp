#include "UserInput.h"
#include <stdio.h>
using namespace glm;

UserInput::~UserInput()
{
	delete viewMatrix;
}
void UserInput::setMenuCamDir()
{
	target = pos + vec3(1, 0, 0);
}

void UserInput::setpos(vec3 inpos, vec3 viewdir)
{
	pos = inpos;
	target = pos + viewdir;
	*viewMatrix = lookAt(pos, target, up);
}

void UserInput::KeyDown(char c)
{
	switch (c)
	{
	case('W') :
		W = true;
		break;
	case('S') :
		S = true;
		break;
	case('A') :
		A = true;
		break;
	case('D') :
		D = true;
		break;
	case('G') :
		G = true;
		break;
	case('M') :
		M = true;
		break;
	case('C') :
		C = true;
		break;
	case('N') :
		N = true;
		break;
	case('Q') :
		Q = true;
		break;
	case('E') :
		E = true;
		Ereleased = false;
		break;
	}
}

void UserInput::KeyNumberDown(int n)
{
	switch (n)
	{
	case(1) :
		_1 = true;
		break;
	case(2) :
		_2 = true;
		break;
	case(3) :
		_3 = true;
		break;
	case(4) :
		_4 = true;
		break;
	case(5) :
		_5 = true;
		break;
	case(6) :
		_6 = true;
		break;
	case(7) :
		_7 = true;
		break;
	case(8) :
		_8 = true;
		break;
	case(9) :
		_9 = true;
		break;
	case(0) :
		_0 = true;
		break;
	}
}

void UserInput::KeyUp(char c)
{
	switch (c)
	{
	case('W') :
		W = false;
		break;
	case('S') :
		S = false;
		break;
	case('A') :
		A = false;
		break;
	case('D') :
		D = false;
		break;
	case('G') :
		G = false;
		break;
	case('M') :
		M = false;
		break;
	case('C') :
		C = false;
		break;
	case('N') :
		N = false;
		break;
	case('Q') :
		Q = false;
		break;
	case('E') :
		if (E == true)
			Ereleased = true;
		else
			Ereleased = false;
		E = false;
		break;
	}
}

void UserInput::KeyNumberUp(int n)
{
	switch (n)
	{
	case(1) :
		_1 = false;
		break;
	case(2) :
		_2 = false;
		break;
	case(3) :
		_3 = false;
		break;
	case(4) :
		_4 = false;
		break;
	case(5) :
		_5 = false;
		break;
	case(6) :
		_6 = false;
		break;
	case(7) :
		_7 = false;
		break;
	case(8) :
		_8 = false;
		break;
	case(9) :
		_9 = false;
		break;
	case(0) :
		_0 = false;
		break;
	}
}

void UserInput::Act(float deltaTime)
{
	float speedorig = speed;
	if (shift)
		speed *= 3;
	speed *= deltaTime;

	if (W)
	{
		vec3 oldPos = pos;
		//pos += (target * speed);
		vec3 toTarget = (target - pos);
		pos += toTarget * (speed/2);
		target += (pos - oldPos);

	}
	if (S)
	{
		vec3 oldPos = pos;
		//pos -= (target * speed);
		vec3 toTarget = (target - pos);
		pos -= toTarget * (speed/2);
		target += (pos - oldPos);
	}
	if (A)
	{
		vec3 oldPos = pos;
		vec3 toTarget = (target - pos);
		vec3 left = cross(up, toTarget);
		left = normalize(left);
		pos += left*speed;
		target += (pos - oldPos);
	}
	if (D)
	{
		vec3 oldPos = pos;
		vec3 toTarget = (target - pos);
		vec3 left = cross(toTarget, up);
		left = normalize(left);
		pos += left*speed;
		target += (pos - oldPos);
	}
	if (space)
	{
		vec3 oldPos = pos;
		pos.y += speed;
		target += (pos - oldPos);
	}
	if (ctrl)
	{
		vec3 oldPos = pos;
		pos.y -= speed;
		target += (pos - oldPos);
	}

	*viewMatrix = lookAt(pos, target, up);

	speed = speedorig;
}

void UserInput::Shift(bool set)
{
	shift = set;
}

void UserInput::Space(bool set)
{
	space = set;
}

bool UserInput::getSpace()
{
	return space;
}

void UserInput::Ctrl(bool set)
{
	ctrl = set;
}

void UserInput::LMB(bool set)
{
	if (lmb == true && set == false)
		lmbReleased = true;
	else
		lmbReleased = false;
	lmb = set;
}

void UserInput::RMB(bool set)
{
	if (rmb == true && set == false)
		rmbReleased = true;
	else
		rmbReleased = false;
	rmb = set;
}

void UserInput::ESC(bool set)
{
	if (esc == true && set == false)
		escReleased = true;
	else
		escReleased = false;
	esc = set;
}

bool UserInput::getESC()
{
	return escReleased;
}

bool UserInput::getRMBrelease()
{
	return rmbReleased;
}

bool UserInput::getRMBdown()
{
	return rmb;
}

bool UserInput::getLMBrelease()
{
	return lmbReleased;
}

bool UserInput::getLMBdown()
{
	return lmb;
}

void UserInput::Mouse(float x, float y)
{
	angleH += x / 5.0f;
	angleV += y / 5.0f;
	if (angleV > 89)
		angleV = 89;
	if (angleV < -89)
		angleV = -89;

	//rotate vertically around x
	float rotateRad;
	vec3 view = vec3(1.0f, 0.0f, 0.0f);
	rotateRad = toRadian(angleV);
	rotV = mat3(cos(rotateRad), -sin(rotateRad), 0.0f,
		sin(rotateRad), cos(rotateRad), 0.0f,
		0.0f, 0.0f, 1.0f);
	view = rotV * view;
	view = normalize(view);

	//rotate horizontally around y
	rotateRad = toRadian(angleH);
	rotH = mat3(cos(rotateRad), 0.0f, -sin(rotateRad), 
				0.0f, 1.0f, 0.0f,
				sin(rotateRad), 0.0f, cos(rotateRad));
	view = rotH * view;
	view = normalize(view);

	//set new view pos
	target = pos - view;
}

void UserInput::Init(mat4* mat, vec3 posIn, vec3 targetIn, vec3 upIn)
{
	viewMatrix = mat;
	pos = posIn;
	target = targetIn;
	up = upIn;

	*viewMatrix = lookAt(pos, target, up);

	//Sets the initial view angels 
	vec3 hTarget = getToTarget();
	hTarget.y = 0;
	hTarget = normalize(hTarget);
	
	if (hTarget.z >= 0.0f)
	{
		if (hTarget.x > 0.0f)
			angleH = 180 - toDegree(asin(hTarget.z));
		else
			angleH = - toDegree(asin(hTarget.z));
	}
	else
	{
		if (hTarget.x >= 0.0f)
			angleH = - toDegree(asin(-hTarget.z));
		else
			angleH = 90.0f + toDegree(asin(-hTarget.z));
	}
	angleV = -toDegree(asin(target.y));
}

float UserInput::toDegree(float val)
{
	return val * 57.2957795f;
}

float UserInput::toRadian(float val)
{
	return val * 0.0174532925f;
}

vec3* UserInput::GetPos()
{
	return &pos;
}

vec3 UserInput::getToTarget()
{
	return (target - pos);
}

bool UserInput::getKeyState(char c)
{
	switch (c)
	{
	case('W') :
		return W;
		break;
	case('S') :
		return S;
		break;
	case('A') :
		return A;
		break;
	case('D') :
		return D;
		break;
	case('G') :
		return G;
		break;
	case('M') :
		return M;
		break;
	case('C') :
		return C;
		break;
	case('N') :
		return N;
		break;
	case('Q') :
		return Q;
		break;
	case('E') :
		return E;
		break;
	}
	return false;
}

bool UserInput::getKeyNumberState(int n)
{
	switch (n)
	{
	case(1) :
		return _1;
		break;
	case(2) :
		return _2;
		break;
	case(3) :
		return _3;
		break;
	case(4) :
		return _4;
		break;
	case(5) :
		return _5;
		break;
	case(6) :
		return _6;
		break;
	case(7) :
		return _7;
		break;
	case(8) :
		return _8;
		break;
	case(9) :
		return _9;
		break;
	case(0) :
		return _0;
		break;
	}
	return false;
}

void UserInput::cameraPan(vec3 moveTo, float factor, float deltaTime, Gamepad* pad, bool playerpeek)
{
	glm::vec3 oldPos = pos;
	float keepZ = pos.z;
	glm::vec3 toTarget = moveTo - pos;
	if (playerpeek && shift)
	{
		if (D)
			toTarget.x += 7;
		if (A)
			toTarget.x -= 7;
		if (W)
			toTarget.y += 7;
		if (S)
			toTarget.y -= 7;
		D = false;
		A = false;
		W = false;
		S = false;
	}
	else
	{
		float axesX, axesY;
		if (pad->joyStickDetected())
		{
			pad->getAxesValues(1, axesX, axesY); // right stick
		}
		else
		{
			axesX = 0;
			axesY = 0;
		}

		toTarget.x += (7 * axesX);
		toTarget.y -= (7 * axesY);
	}
	pos = pos + (toTarget * deltaTime * factor);
	pos.z = keepZ;
	target += (pos - oldPos);
	*viewMatrix = lookAt(pos, target, up);
}

bool UserInput::updateMouse()
{
	return shift;
}

void UserInput::resetZoomViewDir()
{
	angleH = -90;
	angleV = -0;
	pos.z = 10;
	target = pos;
	target.z--;
}

void UserInput::getMouseState(float* x, float* y, bool* right, bool* left)
{
	*x = lastX;
	*y = lastY;
	*right = rmbReleased;
	*left = lmbReleased;
}

void UserInput::setMousePos(float x, float y)
{
	lastX = x;
	lastY = y;
}

bool UserInput::getEreleased()
{
	return Ereleased;
}