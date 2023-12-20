#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
#include "Animation.h"

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
	void OnCollision(PhysBody* physA, PhysBody* physB);

	// Main functions
	void StateMachine();
	b2Vec2 Move(b2Vec2 vel);
	b2Vec2 Jump(b2Vec2 vel);
	b2Vec2 GodMode(b2Vec2 vel);

	// Create Getters and Setters
	int GetHp() const;
	bool GetIsJumping() const;
	bool GetIsMoving() const;
	bool GetGodMode() const;
	int GetPositionX() const;
	int GetPositionY() const;
	
	void SetHp(int hp);
	void SetIsMoving(bool det);
	void SetIsJumping(bool isJumping);
	void SetGodMode(bool godMode);
	void SetPositionX(int x);
	void SetPositionY(int y);

	// Teleport
	void Teleport(int x, int y);

public:

	// Animations
	Animation idleAnimation;
	Animation moveRightAnimation;
	Animation moveLeftAnimation;
	Animation jumpRightAnimation;
	Animation jumpLeftAnimation;

	Animation* currentAnimation;

	// Declare player parameters
	int hp = 100;
	float speed = 5.0f;
	bool canJump = false;
	float jumpSpeed = 6.5f;
	float maxJumpSpeed = 8.0f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	// State Machine
	EntityState currentState;

	// Booleanos 
	bool isJumping = false;
	bool isMoving = false;
	bool isDeath = false;

	//Audio fx
	int jumpFxId;
	int hitFxId;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	//god mode
	bool godMode = false;
	float godModeSpeed;

	// Debug mode
	bool debug;
};

#endif // __PLAYER_H__