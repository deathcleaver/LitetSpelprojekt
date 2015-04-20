#include "edit.h"

void Edit::init(Map* map, UserInput* in)
{
	this->map = map;
	this->in = in;
	this->width = map->readSizeX();
	this->height = map->readSizeY();
	editMode = EditMode::WORLD;
}

void Edit::update(float x, float y)
{
	if (editMode != editModeLast)
	{
		placeState = PlaceState::MOVE;
	}

	//convert mouse x, y to world
	mouseToWorld(&x, &y);

	//get current chunk index edit
	map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);
	map->getChunkIndex(*in->GetPos(), &chunkXMouse, &chunkYMouse);
}

void Edit::mouseToWorld(float* x, float* y)
{
	mouseToScreenSpace(x, y);
	glm::vec3* temp = in->GetPos();
	*x = temp->x + (*x * 15);
	*y = temp->y + (*y * 10);
}

void Edit::mouseToScreenSpace(float* x, float* y)
{
	*x = (*x / (SCREENWIDTH * 0.5)) - 1;
	*y = ((*y / (SCREENHEIGHT * 0.5)) - 1) * -1;
}

void Edit::guiHandle(int bEvent)
{
	switch (bEvent)
	{
	case(100):

		break;
	case(101) :
		editMode = EditMode::WORLD;
		break;
	case(102) :

		break;
	case(103) :

		break;
	}
}
//Edit::