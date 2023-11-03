#include "Entity.h"
#include "Render.h"
#include "App.h"


Entity::Entity(int x, int y, EntityType type) 
{ 

}

bool Entity::Awake()
{
	return true;
}

bool Entity::Start()
{
	return true;
}

bool Entity::Update(float dt)
{
	return true;
}

bool Entity::CleanUp()
{
	return true;
}

bool Entity::LoadState(pugi::xml_node&)
{
	return true;
}

bool Entity::SaveState(pugi::xml_node&)
{
	return true;
}

//void Entity::Enable()
//{
//	if (!active)
//	{
//		active = true;
//		Start();
//	}
//}
//
//void Entity::Disable()
//{
//	if (active)
//	{
//		active = false;
//		CleanUp();
//	}
//}

void Entity::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Entity::BlitEntity(SDL_Rect r, bool flip, float x, float y)
{
	if (flip == false)
		app->render->Blit(sprites, (int)position.x + x, (int)position.y + y, &r, SDL_FLIP_NONE);
	else
		app->render->Blit(sprites, (int)position.x + x, (int)position.y + y, &r, SDL_FLIP_HORIZONTAL);
}