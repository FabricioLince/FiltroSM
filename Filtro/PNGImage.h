#pragma once

#include "Filtro.h"
#include "../SDL/Surface.h"

class PNGImage : public Image
{
    public:
    Surface* surface;
    const char* ext(){return "png";}
    PNGImage(int width, int height)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            throw "Couldn't Init SDL";
        }
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        {
            throw ((const char*)IMG_GetError());
        }
        surface = new Surface(width, height);
    }
    public:
        PNGImage(const char* path)
        {
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                throw "Couldn't Init SDL";
            }
            if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            {
                throw ((const char*)IMG_GetError());
            }
            surface = new Surface(path);
        }
        virtual ~PNGImage()
        {
            delete surface;
        }
        PNGImage(PPMImage* ppmImage);
        PPMImage* convertToPPM();

        PNGImage* cut(int x, int y, int width, int height);

        bool save(const char* path);

        int getWidth() {return surface->getWidth();}
        int getHeight() {return surface->getHeight();}
};

