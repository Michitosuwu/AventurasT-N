#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"
#include <vector>



class Animation {
public:

    Animation();

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

    void pushBack(int startX, int startY, int frameWidth, int frameHeight, int frameCount, float switchTime) {
        animation.push_back(Animation(texture, startX, startY, frameWidth, frameHeight, frameCount, switchTime));
    }

private:
    Rect frame;
    int frameCount;
    int currentFrame;
    float totalTime;
    float switchTime;
    SDL_Texture* texture;
    std::vector<Animation> animation;

    // En lugar de sf::Texture, podrías usar un puntero a textura u otro enfoque según tus necesidades
    // Ejemplo: sf::Texture* texturePtr;
    // También podrías usar una matriz de texturas o un enfoque que se adapte a tus necesidades.
};

#endif 
