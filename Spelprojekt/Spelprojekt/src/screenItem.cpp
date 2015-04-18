#include "screenItem.h"

void ScreenItem::init(int id, int idHover, bool button, int idEvent)
{
	GameObject::init(id);
	hoverContentIndex = idHover;
	this->button = button;
	hover = false;
	this->idEvent = idEvent;
}

int ScreenItem::update(int x, int y, bool click)
{
	if (!button)
		return 0;

	//if collide
	//hover = true;
		//if click
		//return idEvent;
}

int ScreenItem::bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const
{
	glProgramUniformMatrix4fv(*shaderProgram, *shaderuniform, 1, false, &worldMat[0][0]);
	if (hover)
		return hoverContentIndex;
	else
		return (GameObject::returnID());
}