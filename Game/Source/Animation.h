#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SString.h"

#include "PugiXml/src/pugixml.hpp"
#include "Log.h"

#define MAX_FRAMES 30

class Animation
{
public:

	float speed = 1.0f;
	bool loop = true;
	bool pingpong = false;
	bool flip = false;
	int width;
	int height;

	SDL_Rect frames[MAX_FRAMES];

private:

	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongCount = 0;
	int pingpongDirection = 1;

public:

	pugi::xml_document configFile;

	void LoadAnimations(std::string name)
	{
		LOG("Loading animations from XML file");

		pugi::xml_parse_result result = configFile.load_file("config.xml");

		if (result != NULL)
		{
			LOG("XML file loaded successfully.");

			pugi::xml_node animationsNode = configFile.child("config").child("scene").child("animation");
			//pugi::xml_node animationsNode = configFile.child("scene").child("animation");

			// Buscar la animación específica por nombre
			pugi::xml_node animationNode = animationsNode.child(name.c_str());

			if (animationNode)
			{
				LOG("Animation '%s' found.", name.c_str());

				loop = animationNode.attribute("loop").as_bool();
				speed = animationNode.attribute("speed").as_float();

				// Limpiar las frames antes de cargar nuevas
				totalFrames = 0;

				// Almacenar el nodo padre antes del bucle
				pugi::xml_node parentNode = animationNode;

				for (pugi::xml_node frameNode = animationNode; frameNode; frameNode = frameNode.next_sibling(name.c_str()))
				{
					int x = frameNode.attribute("x").as_int();
					int y = frameNode.attribute("y").as_int();
					int w = frameNode.attribute("w").as_int();
					int h = frameNode.attribute("h").as_int();

					LOG("Loaded frame - x: %d, y: %d, w: %d, h: %d", x, y, w, h);

					PushBack({ x, y, w, h });
					width = w;
					height = h;
				}

				// Eliminar el nodo después de cargar las animaciones
				// Utilizar el nodo padre para eliminar el nodo después del bucle
				parentNode.remove_child(name.c_str());

				LOG("Animations loaded successfully.");

			}

			else 
			{
				LOG("Animation '%s' not found.", name.c_str());
			}
		}
		else 
		{
			LOG("Failed to load XML file.");
		}
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}

	void ResetLoopCount()
	{
		loopCount = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update(float dt)
	{
		currentFrame += speed * (dt / 1000) * 60;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong) {
				pingpongDirection = -pingpongDirection;
				if (currentFrame == 0) {
					pingpong = false;
				}
			}
		}
	}

	int GetLoopCount() const
	{
		return loopCount;
	}

	int GetPingPongCount() const
	{
		return pingpongCount;
	}

	float GetCurrentFrameCount() const
	{
		return currentFrame;
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
		{
			actualFrame = totalFrames - currentFrame;
		}
			
		return frames[actualFrame];
	}
};

#endif