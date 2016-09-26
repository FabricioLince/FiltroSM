
#include "imagenes.c"
#include "Filtro.h"
#include "PNGImage.h"
#include <vector>
#define DEBUG 0


PPMImage::PPMImage(char* path)
{
    pixel = NULL;
    if (lee_ppm(path, &pixel, &width, &height) == -1)
    {
        pixel = NULL;
    }
}
PPMImage::PPMImage(int width_, int height_)
: width(width_), height(height_)
{
    pixel = (struct pixel **)malloc(height * sizeof(struct pixel *));
    pixel[0] = (struct pixel *)malloc(width * height * sizeof(struct pixel));
    for (int i = 1; i < height; i++)
        pixel[i] = pixel[i - 1] + width;
}
PPMImage::~PPMImage()
{
    if (pixel != NULL)
    {
        free(pixel[0]);
        free(pixel);
    }
}
bool PPMImage::save(const char* path)
{
    return escribe_ppm((char*)path, pixel, height, width)==0;
}
Color PPMImage::getPixel(int x, int y)
{
    if (pixel == NULL) throw "No PPM Image";

    Pixel& p = pixel[y][x];

    return Color(p.r, p.g, p.b);
}
void PPMImage::putPixel(int x, int y, Color color)
{
    if (pixel == NULL) throw "No PPM Image";

    Pixel p;
    p.r = color.r;
    p.g = color.g;
    p.b = color.b;

    pixel[y][x] = p;
}

FiltroExemplo::FiltroExemplo(int steps_, int radio_)
: steps(steps_), brushRadio(radio_)
{}

Image* FiltroExemplo::applyTo(Image* img)
{
    PPMImage* image = dynamic_cast<PPMImage*>(img);
    if (image == NULL) return NULL;

    PPMImage* result = new PPMImage(image->width, image->height);

    Filtro(steps, brushRadio, image->pixel, result->pixel, result->height, result->width);

    return result;
}


PNGImage::PNGImage(PPMImage* ppmImage)
{
    if (ppmImage == NULL) throw "error";
    if (DEBUG) printf("Converting PPM to PNG\n");
    int w = ppmImage->getWidth();
    int h = ppmImage->getHeight();


    surface = new Surface(w, h);
    if (surface == NULL) throw "Couldn't load";
    if (surface->get() == NULL) throw "Couldn't load";
    if (DEBUG) printf("w: %d - %d\nh: %d - %d\n", w, surface->getWidth(), h, surface->getHeight());


    if (DEBUG) printf("pitch: %u\n", ((Uint32*)surface->get()->pixels)[0 * w + 277]);

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
        {
            Color color = ppmImage->getPixel(x, y);

            surface->putPixelColor(x, y, color);
        }
    if (DEBUG) printf("DONE\n\n");
}
PPMImage* PNGImage::convertToPPM()
{
    if (surface == NULL) throw "no PNG Surface";
    if (DEBUG) printf("Converting PNG to PPM\n");
    int w = surface->getWidth();
    int h = surface->getHeight();
    if (DEBUG) printf("w: %d\nh: %d\n", w, h);
    PPMImage* image = new PPMImage(w, h);

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
        {
            Color color = surface->getPixelColor(x, y);
            image->putPixel(x, y, color);
        }

    if (DEBUG) printf("w: %d\nh: %d\n", image->getWidth(), image->getHeight());
    if (DEBUG) printf("DONE\n\n");

    return image;
}


bool PNGImage::save(const char* path)
{
    if (surface)
        surface->savePNG(path);
    else return false;
    return true;
}
PNGImage* PNGImage::cut(int x, int y, int width, int height)
{
    PNGImage* png = new PNGImage(width, height);

    png->surface->draw(*surface, -x, -y);

    return png;
}


int main1(int argc, char**argv)
{
    PNGImage* source = NULL;
    PPMImage* image = NULL;
    Image* result = NULL;
    FiltroExemplo filtro(5, 8);

    try
    {
        source = new PNGImage("imgs/foto.png");
        printf("source carregada...\n");

        image = source->convertToPPM();
        if (image == NULL) throw "bad conversion to ppm";
        printf("Conversao bem sucedida...\n");
        image->save("imgs/original.ppm");

        delete source;

        printf("aplicando filtro...");
        result = filtro.applyTo(image);
        printf(" Pronto\n");

        if (result == NULL)
        {
            printf("Deu errado\n");
        }
        else
        {
            printf("OK\n");
            result->save("imgs/result.ppm");
            printf("Resultado ppm salvo...\n");
            source = new PNGImage((PPMImage*)result);
            source->save("imgs/result1.png");
            printf("Resultado png salvo...\n");
            delete source;
        }
    }
    catch (const char* err)
    {
        printf("ERROR: %s\n", err);
    }

    delete image;
    delete result;

    return 0;
}

int main1_simplified(int argc, char**argv)
{
    PNGImage* source = NULL;
    PPMImage* image = NULL;
    Image* result = NULL;
    FiltroExemplo filtro(5, 8);

    try
    {
        source = new PNGImage("imgs/foto.png");

        image = source->convertToPPM();
        if (image == NULL) throw "bad conversion to ppm";
        image->save("imgs/original.ppm");

        delete source;

        result = filtro.applyTo(image);

        if (result == NULL)
        {
            printf("Error\n");
        }
        else
        {
            result->save("imgs/result.ppm");
            source = new PNGImage((PPMImage*)result);
            source->save("imgs/result1.png");
            delete source;
        }
    }
    catch (const char* err)
    {
        printf("ERROR: %s\n", err);
    }

    delete image;
    delete result;

    return 0;
}










