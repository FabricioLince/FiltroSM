#ifndef IMAGE_H
#define IMAGE_H
#include <SDL.h>
#include <SDL_image.h>
#include "SDL2_gfx-1.0.1/SDL2_gfxPrimitives.h"
#include <stdio.h>

#include "Color.h"

#define repeat(i, t) for(unsigned i = 0; i < t; ++i)

class Surface
{
    void hack();

    public:

        Surface(int w = 0, int h = 0, int color = 0);
        Surface(const char* path);
        Surface(Surface&);
        Surface(SDL_Surface* src)
        {
            this->image = src;
        }
        ~Surface();

        Surface& operator=(Surface&);

        void destroy();
        bool load(const char* path);
        bool create(int w, int h, int color = 0);
        void save(const char* path);

        void setColorKey(bool enabled, int color = 0xFF00FF);

        void clear(int color);
        void clear(Color color);

        SDL_Surface* get();
        SDL_Surface* image;

        int getWidth();
        int getHeight();

        void putPixelColor(int x, int y, Color color);
        void putPixel(int x, int y, Uint32 color);
        int getPixel(int x, int y);
        Color getPixelColor(int x, int y);

        void savePNG(const char* path);

        void draw(Surface& src, int x = 0, int y = 0);

    protected:

        void low_level_putPixelColor(int x, int y, Uint32 color);

};

#endif // IMAGE_H
