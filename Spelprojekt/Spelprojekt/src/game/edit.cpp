#include "edit.h"

Edit::~Edit()
{
	delete current;
}

void Edit::init(Map* map, UserInput* in)
{
	this->map = map;
	chunks = map->getChunks();
	this->in = in;
	this->width = map->readSizeX();
	this->height = map->readSizeY();
	editMode = EditMode::NONEM;
	lastMousePosX = 0;
	lastMousePosY = 0;
	current = 0;
	lastPlaced = 0;
}

void Edit::update(float x, float y)
{
	if (editMode != NONEM)
	{
		EditorMode();
	}

	editModeLast = editMode;
	editStateLast = editState;
	placeStateLast = placeState;
}

void Edit::EditorMode()
{
	//If edit mode was changed
	if (editMode != editModeLast)
	{
		placeState = PlaceState::NONEP;
		internalPlaceState = 0;
		editContentID = -1;
		discard();
	}
	
	switch (editMode)
	{
	case BACK:
	case WORLD:
	case MONSTER:
	{
		//hold item code
		itemTableClick();

		if (editState != NONES)
		{
			EditorState();
		}
		break;
	}		
	case REKT:
		RektEdit();
		break;
	case LIGHT:
		break;
	}
}

void Edit::itemTableClick()
{
	if (in->getLMBrelease())
	{
		float x, y;
		bool l, r;
		in->getMouseState(&x, &y, &r, &l);

		if (x > 247 && x < 596 && y > 596 && y < 717)
		{
			int indexX = (x - 247) / 24;
			int indexY = (y - 596) / 24;

			editContentID = 12 * indexY + indexX;

			discard();
			HoldNewItem();
		}
	}
}

void Edit::RektEdit()
{
	//only if we are holding down a key
	int action = -1;
	if (in->getLMBdown())
		action = 1; //add
	if (in->getRMBdown())
		action = 0; // remove
	if (action == -1)
		return;

	float x, y;
	bool l, r;
	in->getMouseState(&x, &y, &r, &l);

	if (y > 560) //in hud
		return; 

	mouseToWorld(&x, &y);

	map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);

	if (chunkXCam > -1 && chunkXCam < map->readSizeX() &&
		chunkYCam > -1 && chunkYCam < map->readSizeY())
	{
		int xindex = int(x + 17.5f) % 35;
		int yindex = -(int(y - 17.5f) % 35);

		if (xindex > -1 && xindex < 35 && yindex > -1 && yindex < 35)
		{
			//remove
			if (action == 0)
				chunks[chunkXCam][chunkYCam].removeRekt(xindex, yindex);
			//add
			else if (action == 1)
				chunks[chunkXCam][chunkYCam].addRekt(xindex, yindex);
		}
	}
}

void Edit::EditorState()
{
	//if edit state was changed
	if (editState != editStateLast)
	{
		discard();
		editContentID = -1;
		placeState = PlaceState::NONEP;
		internalPlaceState = 0;
	}
	
	float x, y;
	bool lmb, rmb;
	in->getMouseState(&x, &y, &rmb, &lmb);

	if (editState == EditState::PLACE)
	{
		if(editContentID != -1)
		{
			if (in->getKeyNumberState(0))
				newItem = true;
			if (in->getKeyNumberState(9))
			{
				newItem = true;
				coppyLast = true;
			}
			if (newItem)
				HoldNewItem();

			if (current)
			{
				PlaceEditorState(x, y);
			}
		}
	}
	else if (editState == EditState::CHANGE)
	{
		//grab item
		if (in->getLMBdown())
		{
			
		}
	}
}

void Edit::HoldNewItem()
{
	bool useOld = false;
	glm::vec3 pos;
	//init new at currentposs if possible
	//if (current)
	//{
	//	useOld = true;
	//	pos = current->readPos();
	//	discard();
	//}
	discard();

	internalPlaceState = 0;
	placeState = NONEP;
	editState = PLACE;
	newItem = false;
	if (coppyLast)
	{
		coppyLast = false;
		if (lastPlaced)
		{
			current = new GameObject();
			current->init(lastPlaced->returnID());
			current->coppyMat(lastPlaced);
		}
	}
	else
	{	
		current = new GameObject();
		current->init(editContentID);
		//if(useOld)
		//	current->moveTo(pos.x, pos.y, pos.z);
		//else
		current->moveTo(in->GetPos()->x, in->GetPos()->y, 0);
	}
}

void Edit::PlaceEditorState(float x, float y)
{
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
			if (current)
				if (in->getEreleased())
					giveObjectToChunk();
			break;
		case CHANGE:
			break;
		case NONES:
			break;
		}
	}
}

void Edit::placeObject(float x, float y)
{
	//Set Place State
	if (in->getKeyNumberState(1))
		placeState = NONEP;
	if (in->getKeyNumberState(2))
	{
		placeState = MOVE;
		internalPlaceState = 0;
	}
	else if (in->getKeyNumberState(3))
	{
		placeState = SCALE;
		internalPlaceState = 0;
	}
	else if (in->getKeyNumberState(4))
	{
		placeState = ROT;
		internalPlaceState = 0;
	}
	if (placeState != placeStateLast)
	{
		internalPlaceState = 0;
	}
	if (in->getLMBrelease())
		internalPlaceState++;
	if (in->getRMBdown() || in->updateMouse())
	{

	}
	else //update if lmb or shift isnt down
	{
		switch (placeState)
		{
		case MOVE:
			if (internalPlaceState == 0)
			{
				if (in->getKeyState('Q'))
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
				//if (in->getKeyState('Q'))
				//	current->rotateToXSNAP((y - lastMousePosY) * 0.8); //CURRENTLY NOT WORKING
				current->rotateToX((y - lastMousePosY) * 0.8);

			else if (internalPlaceState == 1)
				current->rotateToY((x - lastMousePosX) * 0.8);

			else if (internalPlaceState == 2)
				current->rotateToZ((y - lastMousePosY) * 0.8);

			else if (internalPlaceState == 3)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		}
	}
	lastMousePosX = x;
	lastMousePosY = y;
}

void Edit::giveObjectToChunk()
{
	switch (editMode)
	{
	case BACK:
		break;
	case WORLD:
		chunks[chunkXCam][chunkYCam].recieveWorld(current);
		lastPlaced = current;
		current = 0;
		newItem = true;
		break;
	case MONSTER:
		break;
	case REKT:
		break;
	case LIGHT:
		break;
	}
}

void Edit::discard()
{
	delete current;
	current = 0;
}

Edit::EditMode Edit::getEditMode()
{
	return editMode;
}

void Edit::mouseToWorld(float* x, float* y)
{
	mouseToScreenSpace(x, y);
	glm::vec3* temp = in->GetPos();
	*x = temp->x + (*x * 14);
	*y = temp->y + (*y * 9.5);
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

	case(100) :
		editMode = EditMode::WORLD;
		editContentID = -1;
		break;
	case(101) : 
		editMode = EditMode::REKT;
		editContentID = -1;
		break;
	case(102) : 
		editMode = EditMode::LIGHT;
		editContentID = -1;
		break;
	case(103) :
		editMode = EditMode::MONSTER;
		editContentID = -1;
		break;
	case(104) :
		editMode = EditMode::BACK;
		editContentID = -1;
		break;
	case(105) : 
		editMode = EditMode::SHRINE;
		editContentID = -1;
		break;
	case(106) : 
		editMode = EditMode::BOSS;
		editContentID = -1;
		break;
	case(107) :
		editMode = EditMode::AUDIO;
		editContentID = -1;
		break;
	case(108) :
		editMode = EditMode::NONEM;
		editContentID = -1;
		break;
		//EDIT STATE SET

	case(110) :
		editState = EditState::PLACE;
		break;
	case(111) :
		editState = EditState::CHANGE;
		break;
	case(112) :
		editState = EditState::NONES;
		break;
	}
}

GameObject* Edit::getObject()
{
	return current;
}

void Edit::invalidID()
{
	discard();
	editContentID = -1;
}