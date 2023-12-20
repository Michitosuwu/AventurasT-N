#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

#define PIXELS_TO_METERS(pixels) (pixels / METERS_TO_PIXELS_RATIO)
#define METERS_TO_PIXELS_RATIO 100.0f

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	//pbody = app->physics->CreateRectangle(position.x, position.y, texW, texH, bodyType::DYNAMIC);
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	jumpFxId = app->audio->LoadFx(config.attribute("jumpFxPath").as_string());

	//godmode
	godMode = false;
	godModeSpeed = 4.0f;

	return true;
}

void Player::StateMachine()
{
	// Obtén la gravedad del mundo de Box2D a través del módulo de física
	b2Vec2 gravity = app->physics->GetGravity();

	// Obtener la velocidad actual del cuerpo del jugador
	b2Vec2 velocity = pbody->body->GetLinearVelocity();

	velocity = Move(velocity);
	velocity = Jump(velocity);

	switch (currentState)
	{
		case EntityState::IDLE:

			if (isMoving)
			{
				currentState = EntityState::WALK;
				LOG("Current State: Walk");
			}

			if (isJumping)
			{
				currentState = EntityState::JUMP;
				LOG("Current State: Jump");
			}

			if (isDeath)
			{
				currentState = EntityState::DEATH;
				LOG("Current State: Death");
			}

			if (godMode)
			{
				currentState = EntityState::GODMODE;
				LOG("Current State: GodMode");
			}

			break;

		case EntityState::WALK:

			if (!isMoving)
			{
				currentState = EntityState::IDLE;
				LOG("Current State: Idle");
			}

			if (isJumping)
			{
				currentState = EntityState::JUMP;
				LOG("Current State: Jump");
			}

			if (isDeath)
			{
				currentState = EntityState::DEATH;
				LOG("Current State: Death");
			}

			if (godMode)
			{
				currentState = EntityState::GODMODE;
				LOG("Current State: GodMode");
			}

			break;

		case EntityState::JUMP:
			
			if (!isJumping && isMoving)
			{
				currentState = EntityState::WALK;
				LOG("Current State: Walk");
			}

			if (!isJumping && !isMoving)
			{
				currentState = EntityState::IDLE;
				LOG("Current State: Idle");
			}

			if (!isJumping && isDeath)
			{
				currentState = EntityState::DEATH;
				LOG("Current State: Death");
			}

			if (godMode)
			{
				currentState = EntityState::GODMODE;
				LOG("Current State: GodMode");
			}
			
			break;

		case EntityState::DEATH:

			// Pensar que hacer aqui
			break;

		case EntityState::GODMODE:
			velocity = GodMode(velocity);

			if (!godMode && !isMoving)
			{
				currentState = EntityState::IDLE;
				LOG("Current State: Idle");
			}

			if (!godMode && isMoving)
			{
				currentState = EntityState::WALK;
				LOG("Current State: Walk");
			}

			break;
	}

	// Actualizar la posición basada en la velocidad
	pbody->body->SetLinearVelocity(velocity);

	//// Obtener la posición del cuerpo
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
}

// Función de movimiento
b2Vec2 Player::Move(b2Vec2 vel)
{
	pbody->body->SetGravityScale(1);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		vel.x = -speed;
		SetIsMoving(true);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		vel.x = speed;
		SetIsMoving(true);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		SetIsMoving(false);
		vel.x = 0;
	}

	return vel;
}

 //Función de salto
b2Vec2 Player::Jump(b2Vec2 vel)
{
	pbody->body->SetGravityScale(1);

    // Controlar el salto con la tecla espacio
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		isJumping = true;
        if (canJump && isJumping)
        {
			vel.y = -jumpSpeed;
			app->audio->PlayFx(jumpFxId);
            canJump = false;
        }
        LOG("JUMP");
    }
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		isJumping = false;
	}

	return vel;
}

b2Vec2 Player::GodMode(b2Vec2 vel)
{
	pbody->body->SetGravityScale(0);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		// Mover hacia la izquierda
		vel.x = -godModeSpeed; // Ajusta la velocidad seg�n tu necesidad
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		// Mover hacia la derecha
		vel.x = godModeSpeed; // Ajusta la velocidad seg�n tu necesidad
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		// Mover hacia arriba
		vel.y = -godModeSpeed; // Ajusta la velocidad seg�n tu necesidad
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		// Mover hacia abajo
		vel.y = godModeSpeed; // Ajusta la velocidad seg�n tu necesidad
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		vel.y = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		vel.x = 0;
	}

	return vel;
}

bool Player::Update(float dt)
{
	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;
	
	//Start from first level F1
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {

		Teleport(config.attribute("x").as_int(), config.attribute("y").as_int());
	}
	
	//Tp to the beginning of the current level F3
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		Teleport(config.attribute("x").as_int(), config.attribute("y").as_int());
	}
	
	//GodMode F10
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	StateMachine();

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		canJump = true;
		break;
	case ColliderType::WALKINGENEMY:
		LOG("Collision ENEMY");
		// TODO : IMPLEMENTAR COLISION CON ENEMIGO
		break;
	case ColliderType::FLYINGGENEMY:
		LOG("Collision ENEMY");
		// TODO : IMPLEMENTAR COLISION CON ENEMIGO
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}
bool Player::GetIsMoving() const
{
	return isMoving;
}
void Player::SetIsMoving(bool det)
{
	this->isMoving = det;
}
void Player::Teleport(int x, int y)
{
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0);
	pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	// Actualizar la variable position con las coordenadas en p�xeles
	position.x = x;
	position.y = y;

}
int Player::GetHp() const
{
	return hp;
}
bool Player::GetIsJumping() const
{
	return isJumping;
}
bool Player::GetGodMode() const
{
	return godMode;
}
int Player::GetPositionX() const
{
	return position.x;
}
int Player::GetPositionY() const
{
	return position.y;
}
void Player::SetHp(int hp)
{
	this->hp = hp;
}
void Player::SetIsJumping(bool isJumping)
{
	this->isJumping = isJumping;
}
void Player::SetGodMode(bool godMode)
{
	this->godMode = godMode;
}
void Player::SetPositionX(int x)
{
	position.x = x;
}
void Player::SetPositionY(int y)
{
	position.y = y;
}