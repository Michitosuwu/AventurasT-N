#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "EnemyWolf.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Checkpoints.h"

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
	case EntityType::CHECKPOINT:
		entity = new Checkpoints();
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

bool EntityManager::LoadState(pugi::xml_node node) {

	ListItem<Entity*>* Entities = entities.start;

	while (Entities != nullptr)
	{
		if (Entities->data->type == EntityType::PLAYER)
		{
			Player* player = static_cast<Player*>(Entities->data);
			if (player != nullptr)
			{
				player->SetPositionX(node.child("player").attribute("x").as_int());
				player->SetPositionY(node.child("player").attribute("y").as_int());
				player->SetLives(node.child("player").attribute("lives").as_int());
				player->SetHp(node.child("player").attribute("hp").as_int());
				player->SetPoints(node.child("player").attribute("points").as_int());
				player->SetGodMode(node.child("player").attribute("godMode").as_bool());
				player->SetIsJumping(node.child("player").attribute("isJumping").as_bool());
				player->Teleport(node.child("player").attribute("x").as_int(), node.child("player").attribute("y").as_int());
			}
		}else if (Entities->data->type == EntityType::ENEMYWOLF)
		{
			EnemyWolf* enemyWolf = static_cast<EnemyWolf*>(Entities->data);
			if (enemyWolf != nullptr)
			{
				enemyWolf->SetPositionX(node.child("enemyWolf").attribute("x").as_int());
				enemyWolf->SetPositionY(node.child("enemyWolf").attribute("y").as_int());
				enemyWolf->SetAlive(node.child("enemyWolf").attribute("alive").as_bool());
				enemyWolf->Teleport(node.child("enemyWolf").attribute("x").as_int(), node.child("enemyWolf").attribute("y").as_int());
			}
		}else if (Entities->data->type == EntityType::ENEMYBEE)
		{
			EnemyBee* enemyBee = static_cast<EnemyBee*>(Entities->data);
			if (enemyBee != nullptr)
			{
				enemyBee->SetPositionX(node.child("enemyBee").attribute("x").as_int());
				enemyBee->SetPositionY(node.child("enemyBee").attribute("y").as_int());
				enemyBee->SetAlive(node.child("enemyBee").attribute("alive").as_bool());
				enemyBee->Teleport(node.child("enemyBee").attribute("x").as_int(), node.child("enemyBee").attribute("y").as_int());
			}
		}else if (Entities->data->type == EntityType::CHECKPOINT)
		{
			Checkpoints* checkpoint = static_cast<Checkpoints*>(Entities->data);
			if (checkpoint != nullptr)
			{
				checkpoint->SetPositionX(node.child("checkpoints").attribute("x").as_int());
				checkpoint->SetPositionY(node.child("checkpoints").attribute("y").as_int());
				checkpoint->SetPicked(node.child("checkpoints").attribute("picked").as_bool());
			}
		}

		Entities = Entities->next;
	}
	return true;
}

bool EntityManager::SaveState(pugi::xml_node node) {

	int playherHp, playerLives, playerPoints, playerX, playerY, wolfX, wolfY, wolfId, beeX, beeY, beeId, checkpointX, checkpointY, checkpointId;
	bool playerGodMode, playerIsJumping, wolfAlive, beeAlive, checkpointPicked;

	ListItem<Entity*>* Entities = entities.start;
	while (Entities != nullptr)
	{
		if (Entities->data->type == EntityType::PLAYER)
		{
			Player* player = static_cast<Player*>(Entities->data);
			if (player != nullptr)
			{
				playherHp = player->GetHp();
				playerLives = player->GetLives();
				playerX = player->GetPositionX();
				playerY = player->GetPositionY();
				playerGodMode = player->GetGodMode();
				playerIsJumping = player->GetIsJumping();
				playerPoints = player->GetPoints();
			}
			pugi::xml_node playerNode = node.append_child("player");
			playerNode.append_attribute("x").set_value(playerX);
			playerNode.append_attribute("y").set_value(playerY);
			playerNode.append_attribute("lives").set_value(playerLives);
			playerNode.append_attribute("hp").set_value(playherHp);
			playerNode.append_attribute("points").set_value(playerPoints);
			playerNode.append_attribute("godMode").set_value(playerGodMode);
			playerNode.append_attribute("isJumping").set_value(playerIsJumping);
		}else if (Entities->data->type == EntityType::ENEMYWOLF)
		{
			EnemyWolf* enemyWolf = static_cast<EnemyWolf*>(Entities->data);
			if (enemyWolf != nullptr)
			{
				wolfX = enemyWolf->GetPositionX();
				wolfY = enemyWolf->GetPositionY();
				wolfId = enemyWolf->GetId();
				wolfAlive = enemyWolf->GetAlive();
			}
			pugi::xml_node wolfNode = node.append_child("enemyWolf");
			wolfNode.append_attribute("x").set_value(wolfX);
			wolfNode.append_attribute("y").set_value(wolfY);
			wolfNode.append_attribute("id").set_value(wolfId);
			wolfNode.append_attribute("alive").set_value(wolfAlive);
		}else if (Entities->data->type == EntityType::ENEMYBEE)
		{
			EnemyBee* enemyBee = static_cast<EnemyBee*>(Entities->data);
			if (enemyBee != nullptr)
			{
				beeX = enemyBee->GetPositionX();
				beeY = enemyBee->GetPositionY();
				beeId = enemyBee->GetId();
				beeAlive = enemyBee->GetAlive();
			}
			pugi::xml_node beeNode = node.append_child("enemyBee");
			beeNode.append_attribute("x").set_value(beeX);
			beeNode.append_attribute("y").set_value(beeY);
			beeNode.append_attribute("id").set_value(beeId);
			beeNode.append_attribute("alive").set_value(beeAlive);
		}else if (Entities->data->type == EntityType::CHECKPOINT)
		{
			Checkpoints* checkpoint = static_cast<Checkpoints*>(Entities->data);
			if (checkpoint != nullptr)
			{
				checkpointX = checkpoint->GetPositionX();
				checkpointY = checkpoint->GetPositionY();
				checkpointId = checkpoint->GetId();
				checkpointPicked = checkpoint->GetPicked();
			}
			pugi::xml_node checkpointNode = node.append_child("checkpoint");
			checkpointNode.append_attribute("x").set_value(checkpointX);
			checkpointNode.append_attribute("y").set_value(checkpointY);
			checkpointNode.append_attribute("id").set_value(checkpointId);
			checkpointNode.append_attribute("picked").set_value(checkpointPicked);
		}

		Entities = Entities->next;
	}
	return true;
}