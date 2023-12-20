#include "Particle.h"
#include "Physics.h"
#include "Collider.h"
#include "App.h"


Particle::Particle(const Particle& p) : Entity(EntityType::BULLET), anim(p.anim), position(p.position), speed(p.speed),
frameCount(p.frameCount), lifetime(p.lifetime)
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

bool Particle::Start() {

	texW = 5;

	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	pbody->listener = this;

	pbody->ctype = ColliderType::BULLET;
}

bool Particle::Update(float dt)
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		anim->Update(dt);

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
				ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim->Finished())
			ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

		if (collider != nullptr)
			collider->SetPos(position.x, position.y);
	}

	return ret;
}

void Particle::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}
