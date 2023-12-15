#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
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

	//create getters and setters
	int GetHp() const;
	bool GetIsJumping() const;
	bool GetGodMode() const;
	int GetPositionX() const;
	int GetPositionY() const;
	
	void SetHp(int hp);
	void SetIsJumping(bool isJumping);
	void SetGodMode(bool godMode);
	void SetPositionX(int x);
	void SetPositionY(int y);

public:

	//L02: DONE 2: Declare player parameters
	int hp = 100;
	float speed = 5.0f;
	bool canJump = false;
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

	//teleport
	void Teleport(int x, int y);

	// Return position X
	int GetPositionX();

	// Return position Y
	int GetPositionY();

	//god mode
	bool godMode = false;

	float godModeSpeed;

	// Debug mode
	bool debug;
};

#endif // __PLAYER_H__