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

	for (pugi::xml_node animationNode = config.child("animation"); animationNode != NULL; animationNode = animationNode.next_sibling("animation")) {
		std::string animationName = animationNode.attribute("name").as_string();

		if (animationName == "idle") {
			idle.pushBack(0, 0, 32, 32, 1, 0.1);
		}
		
		//if (animationNode.attribute("name").as_string() == "idle") {
		//	
		//	//idle->pushBack(0,0,32,32);
		//}

		//if (animationNode.attribute("name").as_string() == "walk") {
		//	//walk->pushBack(0,0,32,32);
		//}


		//config.child("animation").attribute("name").as_string();
		////app->tex->Load(config.attribute())

			
	}

	

	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());
	//TODO add meow sound for jump


	return true;
}

bool Player::Update(float dt)
{
	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2*dt;
	}
		
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		//TODO implementar salto
		velocity.y = 0.2 * dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y = -0.2 * dt;
	}

	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	app->render->DrawTexture(texture,position.x,position.y);

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