#include "Animation.h"


// Constructor de la clase Animation
Animation::Animation(SDL_Texture* texture, int startX, int startY, int frameWidth, int frameHeight, int frameCount, float switchTime)
{
    frame.left = startX;
    frame.top = startY;
    frame.width = frameWidth;
    frame.height = frameHeight;
    this->frameCount = frameCount;
    currentFrame = 0;
    totalTime = 0.0f;
    this->switchTime = switchTime;
}

// Actualizar la animación
void Animation::update(float deltaTime)
{
    totalTime += deltaTime;
    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentFrame = (currentFrame + 1) % frameCount;
    }
}

// Obtener el rectángulo actual del marco
Animation::Rect Animation::getFrame()
{
    int xOffset = currentFrame * frame.width;
    return { frame.left + xOffset, frame.top, frame.width, frame.height };
}