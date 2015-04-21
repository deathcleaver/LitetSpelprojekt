#ifndef EDIT_H
#define EDIT_H

#include "gameObject.h"
#include "map.h"
#include "mapChunk.h"
#include "../UserInput.h"

enum EditMode
{
	BACK,
	WORLD,
	MONSTER,
	REKT,
	LIGHT,
	SPECIAL,
	NONEM,
};

enum EditState
{
	PLACE,
	REMOVE,
	CHANGE,
	NONES,
};

enum PlaceState
{
	MOVE,
	SCALE,
	ROT,
	NONEP,
};

class Edit
{
private:
	Map* map;
	MapChunk** chunks;
	int chunkXCam = 0;
	int chunkYCam = 0;
	int chunkXMouse = 0;
	int chunkYMouse = 0;
	float lastMousePosX = 0;
	float lastMousePosY = 0;
	UserInput* in;
	GameObject* current = 0;
	int width;
	int height;
	float SCREENWIDTH = 1080;
	float SCREENHEIGHT = 720;
	EditMode editMode = EditMode::NONEM;
	EditMode editModeLast = EditMode::NONEM;
	EditState editState = EditState::NONES;
	EditState editStateLast = EditState::NONES;
	PlaceState placeState = PlaceState::NONEP;
	PlaceState placeStateLast = PlaceState::NONEP;

	int editContentID = -1;
	int internalPlaceState = 0;
	void mouseToScreenSpace(float* x, float* y);	
	void mouseToWorld(float* x, float* y);
	
	void undo();
	
	void placeObject(float x, float y);

public:
	Edit(){};
	void init(Map* map, UserInput* in);
	void update(float x, float y);
	void guiHandle(int bEvent);
	EditMode getEditMode();
	GameObject* getObject();
};

#endif