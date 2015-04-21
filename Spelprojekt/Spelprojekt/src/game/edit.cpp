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

		if (newItem)
		{
			newItem = false;
			if (editContentID != -1)
			{
				internalPlaceState = 0;
				current = new GameObject();
				current->init(editContentID);
				current->moveTo(in->GetPos()->x, in->GetPos()->y, 0);
				placeState = MOVE;
			}
		}

		bool inHud = false;
		if (y > 560)
			inHud = true;
		//convert mouse x, y to world
		mouseToWorld(&x, &y);


		//get current chunk index edit
		map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);
		map->getChunkIndex(*in->GetPos(), &chunkXMouse, &chunkYMouse);
		
		if (chunkXCam == -1 || chunkXMouse == -1)
			return;

		if (chunkXCam == chunkXMouse && chunkYCam == chunkYMouse && !inHud)
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
		lastMousePosX = x;
		lastMousePosY = y;
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
	if (in->getKeyNumberState(5)) //reset
	{
		delete current;
		current = new GameObject();
		current->init(editContentID);
		current->moveTo(in->GetPos()->x, in->GetPos()->y, 0);
	}
	if (placeState == NONEP)
	{
		if (in->getKeyNumberState(2))
			placeState = MOVE;
		else if (in->getKeyNumberState(3))
			placeState = SCALE;
		else if (in->getKeyNumberState(4))
			placeState = ROT;
	}
	else
	{
		if (in->getKeyNumberState(1))
			placeState = NONEP;

		if (in->getLMBrelease())
			internalPlaceState++;
		if (in->getRMBdown() || in->updateMouse())
		{
			x = 0;
			y = 0;
			lastMousePosX = 0;
			lastMousePosY = 0;
		}
		switch (placeState)
		{
		case MOVE:
			if (internalPlaceState == 0)
			{
				if(in->getKeyState('Q'))
					current->translateSNAP(x - lastMousePosX, y - lastMousePosY, 0);
				else
					current->translateEDITOR(x - lastMousePosX, y - lastMousePosY, 0);
			}
			else if (internalPlaceState == 1)
			{
				if (in->getKeyState('Q'))
					current->translateSNAP(0, 0, y - lastMousePosY);
				else
					current->translateEDITOR(0, 0, y - lastMousePosY);
			}
			else if (internalPlaceState == 2)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		case SCALE:
			if (internalPlaceState == 0)
				if (in->getKeyState('Q'))
					current->scaleSNAP(x - lastMousePosX, y - lastMousePosY, 0);
				else
					current->scaleAD(x - lastMousePosX, y - lastMousePosY, 0);
			else if (internalPlaceState == 1)
				if (in->getKeyState('Q'))
					current->scaleSNAP(0, 0, y - lastMousePosY);
				else
					current->scaleAD(0, 0, y - lastMousePosY);
			else if (internalPlaceState == 2)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		case ROT:
			if (internalPlaceState == 0)
				if (in->getKeyState('Q'))
					current->rotateToYSNAP(x * 0.2); //CURRENTLY NOT WORKING
				else
					current->rotateToX((y - lastMousePosY) * 0.8);

			else if (internalPlaceState == 1)
			{
				current->rotateToY((x - lastMousePosX) * 0.8);
			}

			else if (internalPlaceState == 2)
			{
				current->rotateToZ((y - lastMousePosY) * 0.8);
			}
			else if (internalPlaceState == 3)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		}
	}
}

void Edit::giveObjectToChunk()
{

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
			//placeState = PlaceState::MOVE;
		}
	}
}

GameObject* Edit::getObject()
{
	return current;
}