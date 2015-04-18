#ifndef SCREEN_ITEM_H
#define SCREEN_ITEM_H

#include "game\gameObject.h"
#include "game\rect.h";

class ScreenItem : public GameObject
{
private:
	int hoverContentIndex;
	int idEvent;
	bool hover;
	bool button;
	Rect collide;
public:
	void init(int id, int idHover = -1, bool button = false, int idEvent =  0);
	int update(int x, int y, bool click);
	int bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
};

#endif