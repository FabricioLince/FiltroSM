#pragma once

class Image
{
    virtual const char* ext()=0;
    public:
        virtual bool save(const char*path){return false;}
};

class Filtro
{
    public:
        virtual Image* applyTo(Image*)=0;
};

class PPMImage;
class FiltroExemplo;
class Color;

class PPMImage : public Image
{
    friend class FiltroExemplo;
    public:
        typedef struct pixel Pixel;

        PPMImage(char* path);
        PPMImage(int width_, int height_);
        virtual ~PPMImage();
        bool save(const char* path);
        int getWidth() {return width;}
        int getHeight() {return height;}

        Color getPixel(int x, int y);
        void putPixel(int x, int y, Color color);

    private:
        int width;
        int height;
        Pixel** pixel;
        const char* ext(){return "ppm";}
};

class FiltroExemplo
{
    int steps;
    int brushRadio;

    public:

        FiltroExemplo(int steps_=5, int radio_=8);

        Image* applyTo(Image* img);
};
