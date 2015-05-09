#ifndef SCREEN_ITEM_H
#define SCREEN_ITEM_H

#include "game\gameObject.h"
#include "game\rect.h"
#include "engine\ContentManager.h"

class ScreenItem : public GameObject
{
private:
	bool hover;
	bool button;
	bool switchButton;
	bool switchActive;
	bool show;
	Rect collide;
public:
	int hoverContentIndex;
	int idEvent;
	void init(int id, int idHover, bool button = false, int idEvent =  0, bool show = true);
	void MoveAutoSize(float x, float y, ContentManager* content);
	int update(float x, float y, bool click);
	int bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
	bool visable(){ return show; };
	void initSwitch();
	void setActive(bool set);

};

#endif