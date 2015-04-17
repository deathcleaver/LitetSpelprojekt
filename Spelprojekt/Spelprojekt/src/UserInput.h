#ifndef USER_INPUT
#define USER_INPUT

#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include <..\glm\gtc\quaternion.hpp>

using namespace glm;

class UserInput
{
private:
	mat4* viewMatrix = 0;
	vec3 pos = { 0, 0, 0 };
	vec3 target = { 0, 0, 0 };
	float angleH = 0;
	float angleV = 0;
	vec3 up = { 0, 0, 0 };
	float cameraOffset = 5.f;
	float currOffset;

	mat3 rotH;
	mat3 rotV;

	float speed = 10.0f;
	double lastX = 0;
	double lastY = 0;

	bool lmb = false;
	bool rmb = false;
	bool shift = false;
	bool space = false;
	bool ctrl = false;
	bool W = false;
	bool A = false;
	bool S = false;
	bool D = false;

	float toDegree(float val);
	float toRadian(float val);
public:
	UserInput(){};
	~UserInput();
	void Init(mat4* mat, vec3 posIn, vec3 lookIn, vec3 upIn);
	void KeyDown(char c);
	void KeyUp(char c);
	void Mouse(float x, float y);
	void Act(float deltaTime);
	void LMB(bool);
	void RMB(bool);
	void Shift(bool);
	void Space(bool);
	void Ctrl(bool);
	vec3* GetPos();
	vec3 getToTarget();

	void followPlayer(vec3 p, vec2 s, float deltaTime);
	bool getKeyState(char c);
	void resetZoomViewDir();
	bool updateMouse();
	void cameraPan(vec3 moveTo, float delay, float deltaTime, bool playerPeak = false);
};

#endif