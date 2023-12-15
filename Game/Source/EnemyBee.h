#ifndef __ENEMYBEE_H__
#define __ENEMYBEE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class EnemyBee : public Entity
{
public:
	EnemyBee();

	virtual ~EnemyBee();

	bool Awake();

	bool Start();

	bool Update(float dt);
	
	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	//void StateMachine();

public:
	
	//Declaare enemy parameters
	SDL_Texture* texture = NULL;

	// Add physics to the enemy - declare a Physics body
	float speed = 5.0f;
	PhysBody* pbody;

	pugi::xml_node config;
	uint texW, texH;

private:
	//Por si de cara a futuro lo necesitamos
};

#endif // __ENEMYBEE_H__