#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"

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

private:
	float textPosX, textPosY = 0;
	bool cameraDebug = false;
	uint windowW, windowH;

	//L03: DONE 3b: Declare a Player attribute
	Player* player;

public:

	// Background attributes
	SString path;
	SDL_Texture* backgroundTexture = NULL;
	uint backgroundPosX, backgroundPosY = 0;
	float backgroundX, backgroundY = 0;
	uint BTexW, BTexH = 0;
};

#endif // __SCENE_H__