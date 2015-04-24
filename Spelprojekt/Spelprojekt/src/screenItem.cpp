#include "screenItem.h"

void ScreenItem::init(int id, int idHover, bool button, int idEvent, bool show)
{
	GameObject::init(id);
	hoverContentIndex = idHover;
	this->button = button;
	hover = false;
	this->idEvent = idEvent;
	this->show = show;
	switchButton = false;
	switchActive = false;
}

int ScreenItem::update(float x, float y, bool click)
{
	if (!button)
		return 0;

	hover = false;
	if (switchActive)
		hover = true;
	
	if (this->collide.intersectsPoint(x, y))
	{
		hover = true;
		if (click)
		{
			if (switchButton)
				switchActive = true;
			return idEvent;
		}
	}
	
	return 0;
}

int ScreenItem::bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const
{
	glProgramUniformMatrix4fv(*shaderProgram, *shaderuniform, 1, false, &worldMat[0][0]);
	if (hover)
		return hoverContentIndex;
	else
		return (GameObject::returnID());
}

void ScreenItem::MoveAutoSize(float x, float y, ContentManager* content)
{
	Object* temp = content->gui()[this->hoverContentIndex];
	this->scaleFactor(temp->scaleX(), temp->scaleY(), 1);
	this->translate(x, y);
	collide = Rect();
	collide.initGameObjectRect(&(this->worldMat), 2 * temp->scaleX(), 2 * temp->scaleY());
}

void ScreenItem::initSwitch()
{
	switchButton = true;
	switchActive = false;
}

void ScreenItem::setActive(bool set)
{
	switchActive = set;
}