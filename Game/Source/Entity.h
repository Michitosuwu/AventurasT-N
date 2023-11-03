#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Animation.h"

enum class EntityType
{
	PLAYER,
	ITEM,
	UNKNOWN
};

struct SDL_Texture;
struct Collider;
class PhysBody;
class Animation;

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}
	//Entity(int x, int y, EntityType type);

	Entity(int x, int y, EntityType type);

	virtual bool Awake();
	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool CleanUp();
	virtual bool LoadState(pugi::xml_node&);
	virtual bool SaveState(pugi::xml_node&);

	//void Entity::Enable() {};
	//void Entity::Disable() {};

	virtual void OnCollision(PhysBody* physA, PhysBody* physB);
	virtual void BlitEntity(SDL_Rect r, bool flip = false, float x = 0, float y = 0);

public:

	SString name;
	Animation* animation = nullptr;
	SDL_Texture* sprites = nullptr;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;
};

#endif // __ENTITY_H__