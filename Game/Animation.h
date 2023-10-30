#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"

#include "Textures.h"


class Animation
{
public:
    Animation(SDL_Texture* texture, int startX, int startY, int frameWidth, int frameHeight, int frameCount, float switchTime);

    void update(float deltaTime);
    sf::IntRect getFrame();

private:
    sf::IntRect frame;
    int frameCount;
    int currentFrame;
    float totalTime;
    float switchTime;
};

#endif 
