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
	bool lmbReleased = false;
	bool rmbReleased = false;

	bool esc = false;
	bool escReleased = false;
	bool shift = false;
	bool space = false;
	bool ctrl = false;
	bool W = false;
	bool A = false;
	bool S = false;
	bool D = false;
	bool G = false;
	bool M = false;
	bool C = false;
	bool N = false;
	bool Q = false;
	bool E = false;
	bool Ereleased = false;
	
	bool _1 = false;
	bool _2 = false;
	bool _3 = false;
	bool _4 = false;
	bool _5 = false;
	bool _6 = false;
	bool _7 = false;
	bool _8 = false;
	bool _9 = false;
	bool _0 = false;

	float toDegree(float val);
	float toRadian(float val);
public:
	UserInput(){};
	~UserInput();
	void Init(mat4* mat, vec3 posIn, vec3 lookIn, vec3 upIn);
	void KeyDown(char c);
	void KeyNumberDown(int n);
	void KeyUp(char c);
	void KeyNumberUp(int n);
	void Mouse(float x, float y);
	void Act(float deltaTime);
	void LMB(bool);
	void RMB(bool);
	void ESC(bool);
	void Shift(bool);
	void Space(bool);
	bool getSpace();
	bool getESC();
	void Ctrl(bool);
	vec3* GetPos();
	vec3 getToTarget();
	void setMousePos(float x, float y);
	void getMouseState(float* x, float* y, bool* right, bool* left);
	
	bool getRMBrelease();
	bool getRMBdown();
	bool getLMBrelease();
	bool getLMBdown();

	bool getEreleased();

	void followPlayer(vec3 p, vec2 s, float deltaTime);
	bool getKeyState(char c);
	bool getKeyNumberState(int n);
	void resetZoomViewDir();
	bool updateMouse();
	void cameraPan(vec3 moveTo, float delay, float deltaTime, bool playerPeak = false);
	void setMenuCamDir();
	void setpos(vec3 inpos, vec3 viewdir);
};

#endif