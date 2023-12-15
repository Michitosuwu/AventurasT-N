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
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	//godmode
	godMode = false;
	godModeSpeed = 4.0f;

	return true;
}

bool Player::Update(float dt)
{

	if (app->render->camera.x - position.x + 200 <= -24 && app->render->camera.x - position.x + 200 >= -10000) {
		app->render->camera.x = -position.x + 200;

	}

	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
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

	//god mode
	if (godMode)
	{
		// En God Mode, el jugador puede moverse libremente sin gravedad
		// Agregar aquí la lógica para el movimiento libre del jugador
		// Obtener la velocidad actual del cuerpo del jugador
		b2Vec2 velocity = pbody->body->GetLinearVelocity();

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			// Mover hacia la izquierda
			velocity.x = -godModeSpeed; // Ajusta la velocidad según tu necesidad
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			// Mover hacia la derecha
			velocity.x = godModeSpeed; // Ajusta la velocidad según tu necesidad
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			// Mover hacia arriba
			velocity.y = -godModeSpeed; // Ajusta la velocidad según tu necesidad
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			// Mover hacia abajo
			velocity.y = godModeSpeed; // Ajusta la velocidad según tu necesidad
		}

		// Actualizar la velocidad del cuerpo
		pbody->body->SetLinearVelocity(velocity);

		// Actualizar la posición del cuerpo basada en la velocidad
		b2Transform pbodyPos = pbody->body->GetTransform();
		position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
		position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
	}
	else {
		// Aplicar la lógica de movimiento normal con gravedad
		// Definir la gravedad
		b2Vec2 gravity(0, GRAVITY_Y);

		// Obtener la velocidad actual del cuerpo del jugador
		b2Vec2 velocity = pbody->body->GetLinearVelocity();

		// Aplicar la gravedad
		if (isJumping)
		{
			velocity.x += gravity.x * dt;
			velocity.y += gravity.y * dt;
			isJumping = false;
		}

		// Controlar el movimiento horizontal
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.x = -speed;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.x = speed;
		}

		// Controlar el salto con la tecla espacio
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping) {
			velocity.y = -jumpSpeed;
			isJumping = true;
		}

		// Limitar la velocidad vertical máxima para evitar un salto brusco
		if (velocity.y < -maxJumpSpeed) {
			velocity.y = -maxJumpSpeed;
		}

		// Actualizar la posición basada en la velocidad
		pbody->body->SetLinearVelocity(velocity);

		// Obtener la posición del cuerpo
		b2Transform pbodyPos = pbody->body->GetTransform();
		position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
		position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
	}

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
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::Teleport(int x, int y)
{
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0);
	pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	// Actualizar la variable position con las coordenadas en píxeles
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