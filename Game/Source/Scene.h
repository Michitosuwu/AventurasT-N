#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "EnemyWolf.h"
#include "EnemyBee.h"
#include "Boss.h"
#include "Checkpoints.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

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

	//For second level
	//void LoadNextMap();

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

	// Finish screen attributes
	SString finishpath;
	SDL_Texture* finishTexture = NULL;
	uint finishPosX, finishPosY = 0;
	float finishX, finishY = 0;
	uint FTexW, FTexH = 0;


	// Declare a Player attribute
	Player* player;

	// Declare wolf enemy
	EnemyWolf* wolf;
	EnemyBee* bee;
	Boss* boss;
	Checkpoints* checkpoint;
	Item* item;
};

#endif // __SCENE_H__