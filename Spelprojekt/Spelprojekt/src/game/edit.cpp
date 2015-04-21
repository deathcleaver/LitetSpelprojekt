#include "edit.h"

void Edit::init(Map* map, UserInput* in)
{
	this->map = map;
	this->in = in;
	this->width = map->readSizeX();
	this->height = map->readSizeY();
	editMode = EditMode::NONEM;
}

void Edit::update(float x, float y)
{
	if (editMode != NONEM)
	{
		if (editMode != editModeLast)
		{
			placeState = PlaceState::NONEP;
			internalPlaceState = 0;
		}
		//convert mouse x, y to world
		mouseToWorld(&x, &y);

		//get current chunk index edit
		map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);
		map->getChunkIndex(*in->GetPos(), &chunkXMouse, &chunkYMouse);
		
		if (chunkXCam == -1 || chunkXMouse == -1)
			return;

		if (chunkXCam == chunkXMouse && chunkYCam == chunkYMouse)
		{
			switch (editState)
			{
			case PLACE:
				placeObject(x, y);
				break;
			case REMOVE:
				break;
			case CHANGE:
				break;
			case NONES:
				break;
			default:
				break;
			}
		}
		editModeLast = editMode;
		placeStateLast = placeState;
	}
	else
	{
		//temporary force
		editMode = WORLD;
		editState = PLACE;
		editContentID = 1;
	}
}

void Edit::placeObject(float x, float y)
{
	if (y > 500)
		return;
	if (placeState == NONEP)
	{
		if (editContentID != -1)
		{
			internalPlaceState = 0;
			current = new GameObject();
			current->init(editContentID);
			placeState = MOVE;
		}
	}
	else
	{
		float tempx, tempy;
		bool left, right;
		in->getMouseState(&tempx, &tempy, &right, &left);
		if (left)
			internalPlaceState++;

		switch (placeState)
		{
		case MOVE:
			if (internalPlaceState == 0)
				current->moveToXY(x, y);
			else if (internalPlaceState == 1)
				current->moveToZ(y);
			else if (internalPlaceState == 2)
			{
				placeState == SCALE;
				internalPlaceState = 0;
			}
			break;
		case SCALE:
		{
			int k = 1;
			//if (internalPlaceState == 0)
			//	current->scaleUniformFactor(y);
			//else if (internalPlaceState == 1)
			//	current->scaleUniformFactor(y);
			//else if (internalPlaceState == 2)
			//{
			//	placeState == ROT;
			//	internalPlaceState = 0;
			//}
		}
			break;
		case ROT:

			break;
		}
	}
}

EditMode Edit::getEditMode()
{
	return editMode;
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
		//EDIT MODE BUTTON SET

	case(100) : //EDIT MODE BACKGROUND
		editMode = EditMode::BACK;
		editContentID = -1;
		break;
	case(101) : //EDIT MODE WORLD
		editMode = EditMode::WORLD;
		editContentID = -1;
		break;
	case(102) : //EDIT MODE MONSTER
		editMode = EditMode::MONSTER;
		editContentID = -1;
		break;
	case(103) : //EDIT MODE REKT
		editMode = EditMode::REKT;
		editContentID = -1;
		break;
	case(104) : //EDIT MODE LIGHT
		editMode = EditMode::LIGHT;
		editContentID = -1;
		break;
	case(105) : //EDIT MODE SPECIAL
		editMode = EditMode::SPECIAL;
		editContentID = -1;
		break;
	case(106) : //EDIT MODE NONEM
		editState = EditState::NONES;
		editContentID = -1;
		break;

		//EDIT STATE SET

	case(110) :
		editState = EditState::PLACE;
		break;
	case(111) :
		editState = EditState::REMOVE;
		break;
	case(112) :
		editState = EditState::CHANGE;
		break;
	case(113) :
		editState = EditState::NONES;
		break;

		//PLACE STATE SET

	case(120) :
		placeState = PlaceState::MOVE;
		internalPlaceState = 0;
		break;
	case(121) :
		placeState = PlaceState::SCALE;
		internalPlaceState = 0;
		break;
	case(122) :
		placeState = PlaceState::ROT;
		internalPlaceState = 0;
		break;
	case(123) :
		placeState = PlaceState::NONEP;
		internalPlaceState = 0;
		break;

		// OBJECT ID
		if (bEvent > 199)
		{
			editContentID = bEvent - 200;
			editState = EditState::PLACE;
			placeState = PlaceState::MOVE;
		}
	}
}

GameObject* Edit::getObject()
{
	return current;
}