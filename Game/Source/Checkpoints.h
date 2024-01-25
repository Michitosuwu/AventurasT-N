#ifndef __CHECKPOINTS_H__
#define __CHECKPOINTS_H__

#include "Entity.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class Checkpoints : public Entity
{
public:

	Checkpoints();

	virtual ~Checkpoints();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	SDL_Texture* texture;
	iPoint size;
	pugi::xml_node config;

	bool isPicked = false;

	int id;

private:

	PhysBody* pbody;
};

#endif // __CHECKPOINTS_H__