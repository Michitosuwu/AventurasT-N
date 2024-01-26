#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "EnemyWolf.h"
#include "EnemyBee.h"
#include "Checkpoints.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	float textPosX, textPosY = 0;
	bool cameraDebug = false;
	uint windowW, windowH;

public:

	// Background attributes
	SString path;
	SDL_Texture* backgroundTexture = NULL;
	uint backgroundPosX, backgroundPosY = 0;
	float backgroundX, backgroundY = 0;
	uint BTexW, BTexH = 0;
	
	// Death screen attributes
	SString deathpath;
	SDL_Texture* deathTexture = NULL;
	uint deathPosX, deathPosY = 0;
	float deathX, deathY = 0;
	uint DTexW, DTexH = 0;


	// Declare a Player attribute
	Player* player;

	// Declare wolf enemy
	EnemyWolf* wolf;
	EnemyBee* bee;
	Checkpoints* checkpoint;

	GuiControlButton* gcButtom;
};

#endif // __SCENE_H__