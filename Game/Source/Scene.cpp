#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Item.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all items in the scene

	// MAP
	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();

	//Checkpoints
	for (pugi::xml_node checkpointNode = config.child("checkpoint"); checkpointNode; checkpointNode = checkpointNode.next_sibling("checkpoint"))
	{
		Checkpoints* Checkpoint = (Checkpoints*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		Checkpoint->config = checkpointNode;
		LOG("checkpoint created");
	}

	//Items
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->config = itemNode;
		LOG("item created");
	}

	//ENEMIES
	// Wolf enemy 
	for (pugi::xml_node wolfNode = config.child("enemies").child("enemywolf"); wolfNode; wolfNode = wolfNode.next_sibling("enemywolf"))
	{
		EnemyWolf* wolf = (EnemyWolf*)app->entityManager->CreateEntity(EntityType::ENEMYWOLF);
		wolf->config = wolfNode;
		LOG("wolf created");
	}
	
	// Bee enemy 
	for (pugi::xml_node beeNode = config.child("enemies").child("enemybee"); beeNode; beeNode = beeNode.next_sibling("enemybee"))
	{
		EnemyBee* bee = (EnemyBee*)app->entityManager->CreateEntity(EntityType::ENEMYBEE);
		bee->config = beeNode;
		LOG("bee created");
	}

	// PLAYER
	// Instantiate the player using the entity manager
	// Get player paremeters
	// Assigns the XML node to a member in player
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->config = config.child("player");

	//BACKGROUND
	// Loading and saving background attributes
	backgroundPosX = config.child("background").attribute("x").as_uint();
	backgroundPosY = config.child("background").attribute("y").as_uint();
	BTexH = config.child("background").attribute("height").as_uint();
	BTexW = config.child("background").attribute("width").as_uint();
	path = config.child("background").attribute("texturePath").as_string();

	//DEATH SCREEN
	// Loading and saving death screen attributes
	deathPosX = config.child("deathscreen").attribute("x").as_uint();
	deathPosY = config.child("deathscreen").attribute("y").as_uint();
	DTexW = config.child("deathscreen").attribute("height").as_uint();
	DTexH = config.child("deathscreen").attribute("width").as_uint();
	deathpath = config.child("deathscreen").attribute("texturePath").as_string();

	app->audio->PlayMusic(config.child("mainmusic").attribute("path").as_string());

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	backgroundTexture = app->tex->Load(path.GetString());
	deathTexture = app->tex->Load(deathpath.GetString());
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	////Get the size of the texture
	//app->tex->GetSize(img, texW, texH);
	app->tex->GetSize(backgroundTexture, BTexW, BTexH);

	backgroundX = (float)windowW / 2 - (float)BTexW / 2;
	backgroundY = (float)windowH / 2 - (float)BTexH / 2;

	//Get the size of the texture
	app->tex->GetSize(deathTexture, DTexW, DTexH);

	deathX = (float)windowW / 2 - (float)DTexW / 2;
	deathY = (float)windowH / 2 - (float)DTexH / 2;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 

	if (app->render->camera.x - player->GetPositionX() + 200 <= -24 && app->render->camera.x - player->GetPositionX() + 200 >= -10000 && !cameraDebug) 
	{
		app->render->camera.x = -player->GetPositionX() + 200;
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		app->render->camera.y -= (int)ceil(camSpeed * dt);
		cameraDebug = true;
	}	
	else if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
	{
		// Restablecer la posicion de la camara
		cameraDebug = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		app->render->camera.y += (int)ceil(camSpeed * dt);
		cameraDebug = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		// Restablecer la posicion de la camara
		cameraDebug = false;
	}	

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		app->render->camera.x += (int)ceil(camSpeed * dt);
	}
	else if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		// Restablecer la posicion de la camara
		cameraDebug = false;
	}	

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		app->render->camera.x -= (int)ceil(camSpeed * dt);
		cameraDebug = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		// Restablecer la posicion de la camara
		cameraDebug = false;
	}

	int cameraX = -app->render->camera.x;
	int cameraY = -app->render->camera.y;

	int adjustedX = backgroundX + cameraX;
	int adjustedY = backgroundY + cameraY / 2;

	app->render->DrawTexture(backgroundTexture, adjustedX, adjustedY);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
