#ifndef SCREEN_ITEM_H
#define SCREEN_ITEM_H

#include "game\gameObject.h"
#include "game\rect.h";
#include "engine\ContentManager.h"

class ScreenItem : public GameObject
{
private:
	int hoverContentIndex;
	int idEvent;
	bool hover;
	bool button;
	bool show;
	Rect collide;
public:
	void init(int id, int idHover, bool button = false, int idEvent =  0, bool show = true);
	void MoveAutoSize(float x, float y, ContentManager* content);
	int update(float x, float y, bool click);
	int bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
	bool visable(){ return show; };
};

#endif