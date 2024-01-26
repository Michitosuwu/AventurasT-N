#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;

	pugi::xml_node config;

	//L07 DONE 4: Add a physics to an item
	PhysBody* pbody;

	bool isPicked = false;
	int type;
	int id;

	//Audio fx
	int touched;

private:
};

#endif // __ITEM_H__