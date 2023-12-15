#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animationn.h"

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

public:

	//L02: DONE 2: Declare player parameters
	float speed = 5.0f;
	bool isJumping = false;
	float jumpSpeed = 5.0f;
	float maxJumpSpeed = 8.0f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Audio fx
	int pickCoinFxId;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	//funcion de teleport para el debug
	bool TeleportTo();

	//setear posicion
	void SetPosition(int x, int y);

	//god mode
	bool godMode = false;

	float godModeSpeed;

	// Debug mode
	bool debug;

	//Declaramos animaciones
	Animation idleAnimation;
	Animation moveRightAnimation;
	Animation moveLeftAnimation;
	Animation jumpRightAnimation;
	Animation jumpLeftAnimation;

	Animation* currentAnimation;
};

#endif // __PLAYER_H__