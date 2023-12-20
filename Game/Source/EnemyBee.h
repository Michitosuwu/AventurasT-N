#ifndef __ENEMYBEE_H__
#define __ENEMYBEE_H__

#include "Entity.h"
#include "Point.h"
#include "DynArray.h"
#include "Animationn.h"
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

	void StateMachine();

	void Attack();

	void Shoot();

	////funcion de teleport para el debug
	void Teleport(int x, int y);

	//create getters and setters
	int GetPositionX() const;
	int GetPositionY() const;
	bool GetAlive() const;

	void SetPositionX(int x);
	void SetPositionY(int y);
	void SetAlive(bool alive);

public:
	
	//Declare enemy parameters
	SDL_Texture* texture = NULL;

	// Add physics to the enemy - declare a Physics body
	float speed = 3.0f;
	PhysBody* pbody;

	pugi::xml_node config;
	uint texW, texH;

	iPoint origin, destiny;

	DynArray<iPoint> lastPath;

	bool alive = true;

	SDL_Texture* tileTex = nullptr;
	//Declaramos animaciones
	Animation beeFlyAnim;
	Animation beeAttackAnim;
	Animation beeDeathAnim;
	
	Animation* currentAnim;

private:
	//Por si de cara a futuro lo necesitamos
};

#endif // __ENEMYBEE_H__