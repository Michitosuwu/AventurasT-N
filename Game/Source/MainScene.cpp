#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Item.h"
#include "MainScene.h"
#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

MainScene::MainScene() : Module()
{
	name.Create("mainScene");
}

MainScene::MainScene(bool startEnabled) : Module(startEnabled)
{
	name.Create("mainScene");
}

// Destructor
MainScene::~MainScene()
{}

bool MainScene::Awake(pugi::xml_node config)
{
	LOG("Loading  MainScene");
	bool ret = true;

	path = config.child("titleScreen").attribute("texturePath").as_string();
	BTexH = config.child("titleScreen").attribute("height").as_uint();
	BTexW = config.child("titleScreen").attribute("width").as_uint();

	return ret;
}

bool MainScene::Start()
{

	// UI 
	SDL_Rect btPos = { 300, 200, 120, 20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);

	backgroundTexture = app->tex->Load(path.GetString());

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	////Get the size of the texture
	//app->tex->GetSize(img, texW, texH);
	app->tex->GetSize(backgroundTexture, BTexW, BTexH);

	backgroundX = (float)windowW / 2 - (float)BTexW / 2;
	backgroundY = (float)windowH / 2 - (float)BTexH / 2;

	return true;
}

bool MainScene::PreUpdate()
{
	return true;
}

bool MainScene::Update(float dt)
{
	app->render->DrawTexture(backgroundTexture, 0, 0);

	return true;
}

bool MainScene::PostUpdate() 
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool MainScene::CleanUp() 
{
	return true;
}

bool MainScene::OnGuiMouseClickEvent(GuiControl* control) 
{
	return true;
}
