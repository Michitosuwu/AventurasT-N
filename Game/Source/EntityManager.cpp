#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "EnemyWolf.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ENEMYWOLF:
		entity = new EnemyWolf();
		break;
	case EntityType::ENEMYBEE:
		entity = new EnemyBee();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

//bool EntityManager::TeleportPlayerToConfig()
//{
//	// Itera sobre las entidades para encontrar el jugador
//	ListItem<Entity*>* item = entities.start;
//	while (item != NULL)
//	{
//		if (item->data->type == EntityType::PLAYER)
//		{
//			// Encuentra el jugador y llama a la función TeleportTo
//			Player* player = static_cast<Player*>(item->data);
//			if (player != nullptr)
//			{
//				return player->TeleportTo("config.xml");
//			}
//		}
//		item = item->next;
//	}
//
//	// Si no se encontró al jugador, retorna false (error)
//	return false;
//}

bool EntityManager::LoadState(pugi::xml_node node) {

	ListItem<Entity*>* Entities = entities.start;
	if (Entities->data->type == EntityType::PLAYER)
	{
		Player* player = static_cast<Player*>(Entities->data);
		if (player != nullptr)
		{
			player->SetPositionX(node.child("player").attribute("x").as_int());
			player->SetPositionY(node.child("player").attribute("y").as_int());
			player->SetHp(node.child("player").attribute("hp").as_int());
			player->SetGodMode(node.child("player").attribute("godMode").as_bool());
			player->SetIsJumping(node.child("player").attribute("isJumping").as_bool());
			player->Teleport(node.child("player").attribute("x").as_int(), node.child("player").attribute("y").as_int());
		}
	}
	return true;
}

bool EntityManager::SaveState(pugi::xml_node node) {

	int playherHp, playerX, playerY;
	bool playerGodMode, playerIsJumping;

	ListItem<Entity*>* Entities = entities.start;
	if (Entities->data->type == EntityType::PLAYER)
	{
		Player* player = static_cast<Player*>(Entities->data);
		if (player != nullptr)
		{
			playherHp = player->GetHp();
			playerX = player->GetPositionX();
			playerY = player->GetPositionY();
			playerGodMode = player->GetGodMode();
			playerIsJumping = player->GetIsJumping();

		}
		pugi::xml_node playerNode = node.append_child("player");
		playerNode.append_attribute("x").set_value(playerX);
		playerNode.append_attribute("y").set_value(playerY);
		playerNode.append_attribute("hp").set_value(playherHp);
		playerNode.append_attribute("godMode").set_value(playerGodMode);
		playerNode.append_attribute("isJumping").set_value(playerIsJumping);
	}
	return true;
}