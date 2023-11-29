﻿// Animation.h

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SString.h"
//#include <string>
//#include <vector>

#include "PugiXml/src/pugixml.hpp"
#include "Log.h"




#define MAX_FRAMES 200 

class Animation {

public:

    Animation::Animation(): current_frame(0.0f), last_frame(0), loop(false), speed(0.0f), loops(0){}

    Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
    {
        SDL_memcpy(&frames, anim.frames, sizeof(frames));
    }

    void PushBack(const SDL_Rect& rect)
    {
        frames[last_frame++] = rect;
    }

    SDL_Rect& GetCurrentFrame(float dt)
    {
        if (this)
        {
            current_frame += speed * dt;
            if (current_frame >= last_frame)
            {
                current_frame = (loop) ? 0.0f : last_frame - 1;
                loops++;
            }
            return frames[(int)current_frame];
        }
    }

    pugi::xml_document configFile;

    void LoadAnimations(std::string name)
    {
        /*pugi::xml_parse_result result = configFile.load_file("config.xml");

        if (result != NULL)
        {
            pugi::xml_node animation_name = configFile.child("scene").child("animations").child(name.GetString());
            loop = animation_name.attribute("loop").as_bool();
            speed = animation_name.attribute("speed").as_float();

            for (pugi::xml_node animation = animation_name.child("animation"); animation; animation = animation.next_sibling("animation"))
            {
                PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
            }
        }*/
        
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
                last_frame = 0;

                // Almacenar el nodo padre antes del bucle
                pugi::xml_node parentNode = animationNode;

                for (pugi::xml_node frameNode = animationNode; frameNode; frameNode = frameNode.next_sibling(name.c_str()))
                {
                    /*PushBack({
                        frameNode.attribute("x").as_int(),
                        frameNode.attribute("y").as_int(),
                        frameNode.attribute("w").as_int(),
                        frameNode.attribute("h").as_int()
                        });*/
                    int x = frameNode.attribute("x").as_int();
                    int y = frameNode.attribute("y").as_int();
                    int w = frameNode.attribute("w").as_int();
                    int h = frameNode.attribute("h").as_int();

                    LOG("Loaded frame - x: %d, y: %d, w: %d, h: %d", x, y, w, h);

                    PushBack({ x, y, w, h });
                }

                // Eliminar el nodo después de cargar las animaciones
                // Utilizar el nodo padre para eliminar el nodo después del bucle
                parentNode.remove_child(name.c_str());

                LOG("Animations loaded successfully.");

            }

            else {
                LOG("Animation '%s' not found.", name.c_str());
            }
        }
        else {
            LOG("Failed to load XML file.");
        }
    }

    float Getframe_pos() {
        return current_frame;
    }

    bool isLastFrame() {
        return (int)current_frame >= last_frame - 1;
    }

    bool equal(Animation* anim) {
        return (this == anim);
    }

    bool Finished()
    {
        if (loops > 0) {
            return true;
        }
        else
            return false;
    }

    void Reset()
    {
        current_frame = 0;
        loops = 0;
    }

    bool loop;
    float speed;
    int loops;
    SDL_Rect frames[MAX_FRAMES];

    void Update(float dt){}

private:

    float current_frame;
    int last_frame = 0;

};

#endif 
