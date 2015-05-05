#ifndef EDIT_H
#define EDIT_H

#include "gameObject.h"
#include "map.h"
#include "mapChunk.h"
#include "../UserInput.h"
#include "../GUI.h"
<<<<<<< HEAD
#include "../GameConfig.h"
#include "Light.h"
=======
#include "Effect\Light.h"
>>>>>>> 000076c905b6cff24b327e4e674e91c83e06521a

class Edit
{
private:
	enum EditMode
	{
		WORLD,
		REKT,
		LIGHT,
		MONSTER,
		BACK,
		SHRINE,
		BOSS,
		AUDIO,
		NONEM,
	};
	enum EditState
	{
		PLACE,
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

	Map* map;
	MapChunk** chunks;
	int chunkXCam = 0;
	int chunkYCam = 0;
	int chunkXMouse = 0;
	int chunkYMouse = 0;
	float lastMousePosX = 0;
	float lastMousePosY = 0;
	UserInput* in;

	int width;
	int height;

	float SCREENWIDTH = configResX;
	float SCREENHEIGHT = configResY;
	EditMode editMode = EditMode::NONEM;
	EditMode editModeLast = EditMode::NONEM;
	EditState editState = EditState::NONES;
	EditState editStateLast = EditState::NONES;
	PlaceState placeState = PlaceState::NONEP;
	PlaceState placeStateLast = PlaceState::NONEP;

	int editContentID = -1;

	int internalPlaceState = 0;
	GameObject* current = 0;
	GameObject lastPlaced;
	GameObject takenCopy;

	Light* currentLight = 0;
	Light lastPlacedLight;
	Light takenCopyLight;

	bool newItem = false;
	bool coppyLast = false;
	bool itemtaken = false;
	bool itemPlaced = false;

	void EditorMode();

	void EditorState();
	void HoldNewItem();
	void PlaceEditorState(float x, float y);
	void discard();

	void RektEdit();

	void EditID();
	bool debugBackground = false;
	bool ForceEditBG();

	void mouseToScreenSpace(float* x, float* y);	
	void mouseToWorld(float* x, float* y);
	
	void undo();
	
	void placeObject(float x, float y);
	void giveObjectToChunk();
	void itemTableClick();
public:

	Edit(){};
	~Edit();
	void init(Map* map, UserInput* in);
	void update(float x, float y, GUI* gui);
	void refreshOnEnter();
	void guiHandle(int bEvent);
	EditMode getEditMode();
	GameObject* getObject();
	Light* getLight();
	void invalidID();
	bool isMovingLights();
	bool forceRekts = false;
	void saveloadCheck(bool* save, bool* load, int* nr);
};

#endif