#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Audio.h"
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include "../legend.h"
#include "../Align16.h"
#include "../DebugLog.h"

class GameObject : public Align_16
{
protected: //private 
	glm::mat4 worldMat = { 1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 };
protected:
	bool facingRight;
	int contentIndex;
public:
	glm::mat4* getWorldMat();
	GameObject(){ facingRight = true; contentIndex = -1; };
	void init(int idi);
	int bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
	void coppyMat(GameObject* obj);
	void setWorldMat(glm::mat4* mat);

	void rotateTo(float x, float y, float z);
	void rotateToX(float x);
	void rotateToXSNAP(float x);
	void rotateToY(float y);
	void rotateToYSNAP(float y);
	void rotateToZ(float z);
	void rotateToZSNAP(float z);
	void rotateToXY(float x, float y);
	void rotateToXYSNAP(float x, float y);

	void translate(float x, float y, float z);
	void translate(float x, float y);
	void moveTo(float x, float y);
	void moveTo(float x, float y, float z);
	void moveToXY(float x, float y);
	void moveToZ(float z);
	void moveTo(glm::vec3 target);
	void scaleUniformAD(float val);
	void scaleUniformFactor(float val);
	void scaleFactor(float x, float y, float z);
	void scaleAD(float x, float y, float z);
	void scaleSNAP(float x, float y, float z);
	void scaleFactorADsnap(float x, float y, float z);
	void translateSNAP(float x, float y, float z);
	void translateSNAPXY(float x, float y);
	void translateSNAPZ(float z);
	void translateEDITOR(float x, float y, float z);

	virtual int update(float deltaTime);

	glm::vec3 readPos() const;

	int returnID() const;
	
	float posXfloatsave = -99999.0f;
	float posYfloatsave = -99999.0f;
	float posZfloatsave = -99999.0f;
	float scaleXfloatsave = -99999.0f;
	float scaleYfloatsave = -99999.0f;
	float scaleZfloatsave = -99999.0f;
	float rotXfloatsave = 0;
	float rotYfloatsave = 0;
	float rotZfloatsave = 0;
	float rotXfloatsaveTEMP = 0;
	float rotYfloatsaveTEMP = 0;
	float rotZfloatsaveTEMP = 0;
private:
};

#endif


