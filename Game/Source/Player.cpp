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
#include "Animation.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	animation = NULL;

	idle.LoadAnimations("idle");
	walkRight.LoadAnimations("walkRight");
	jumpRight.LoadAnimations("jumpRight");
	walkLeft.LoadAnimations("walkLeft");
	jumpLeft.LoadAnimations("jumpLeft");
	
	isOnGround = true; //inicialmente está en el suelo
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	sprites = app->tex->Load(config.attribute("texturePath").as_string());

	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateRectangle(position.x, position.y, texW / 2, texH / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect 
	//pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());
	//TODO add meow sound for jump


	return true;
}

bool Player::Update(float dt)
{
	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	animation = &idle;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2*dt;
	}
		
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		//TODO implementacion salto correcta?

		if (isOnGround) {  //verificamos si está en el suelo
			velocity.y = -10.0 * dt;  //aplicamos velocidad inicial salto
			isOnGround = false; //marcamos jugador en el aire
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y = -0.2 * dt;
	}

	//aplicamos velocidad al cuerpo fisico
	pbody->body->SetLinearVelocity(velocity);

	//obtiene velocidad del cuerpo fisico
	b2Transform pbodyPos = pbody->body->GetTransform();
	
	//obtiene posicion del cuerpo fisico
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	//renderiza la animacion actual en la posicion actual
	BlitEntity(animation->GetCurrentFrame(dt), flip, position.x, position.y);

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

bool isOnGround() {
	//TODO implementar bien logica

	//preguntarle esto a marta
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
	uint texW, texH;
	iPoint position;
	
	
	int groundHeight = 10;

	//calcula pos vertical jugador
	int playerBottomY = position.y + texH / 2;

	//comprueba si el jugador esta cerca del suelo
	bool nearGround = (playerBottomY >= groundHeight - 2);

	//comprueba si velocidad vertical del jugador es cero o positiva (indicando que no está cayendo)
	bool noVerticalVelocity = (velocity.y >= 0);

	//el jugador esta en el suelo si esta cerca del suelo y su velocidad vertical no es negativa
	return nearGround && noVerticalVelocity;
}