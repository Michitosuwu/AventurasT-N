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

	//getters setters
	int GetPositionX() const;
	int GetPositionY() const;
	bool GetPicked() const;

	void SetPositionX(int x);
	void SetPositionY(int y);
	void SetPicked(bool picked);

public:
	SDL_Texture* texture;
	const char* texturePath;
	pugi::xml_node config;
	uint texW, texH;
	PhysBody* pbody;

	int id;

	bool isPicked = false;

private:
	
};

#endif // __CHECKPOINTS_H__