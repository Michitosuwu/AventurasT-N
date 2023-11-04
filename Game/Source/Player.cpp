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
	pbody = app->physics->CreateRectangle(position.x, position.y, texW, texH, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	return true;
}

bool Player::Update(float dt)
{
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