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

	void OnCollision(PhysBody* physA, PhysBody* physB);

	int GetPositionX() const;
	int GetPositionY() const;
	int GetId() const;
	int GetType() const;

	void SetPositionX(int x);
	void SetPositionY(int y);
	void SetId(int id);
	void SetType(int type);

public:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;

	pugi::xml_node config;

	//L07 DONE 4: Add a physics to an item
	PhysBody* pbody;

	int type;
	int id;

	//Audio fx
	int touched;

private:
};

#endif // __ITEM_H__