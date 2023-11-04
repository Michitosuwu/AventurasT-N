#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Module.h"
#include "App.h"
#include "Physics.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	//Determinar si está colisionando con el suelo TODO definir y poner colliders
	bool isOnGround;

public:

	Animation idle;
	Animation walkRight;
	Animation walkLeft;
	Animation jumpRight;
	Animation jumpLeft;

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;
	bool flip = false;
	float jumpSpeed = -0.4f; //velocidad inicial de salto hacia arriba

	//nose si esto se tendria q quitar
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	//Audio fx
	int pickCoinFxId;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	//funcion de teleport para el debug
	bool TeleportTo(const char* configFile);

	//setear posicion
	void SetPosition(int x, int y);
};

#endif // __PLAYER_H__