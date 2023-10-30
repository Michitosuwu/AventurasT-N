#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"



class Animation {
public:
    Animation(SDL_Texture* texture, int startX, int startY, int frameWidth, int frameHeight, int frameCount, float switchTime);

    // Destructor
    virtual ~Animation();

    void update(float deltaTime);
    struct Rect {
        int left;
        int top;
        int width;
        int height;
    };
    Rect getFrame();

private:
    Rect frame;
    int frameCount;
    int currentFrame;
    float totalTime;
    float switchTime;

    // En lugar de sf::Texture, podrías usar un puntero a textura u otro enfoque según tus necesidades
    // Ejemplo: sf::Texture* texturePtr;
    // También podrías usar una matriz de texturas o un enfoque que se adapte a tus necesidades.
};

#endif 
