#include "Surface.h"
#include "Color.h"
#include <stdexcept>
#include "SDL2_gfx-1.0.1/SDL2_gfxPrimitives.h"

Surface::Surface(int w, int h, int color)
{
    image = NULL;
    if (w > 0 && h > 0)
    {
        create(w, h, color);
    }
}

Surface::Surface(const char* path)
{
    image = NULL;
    if (!load(path)) throw "bad load";
}

Surface::Surface(Surface& source)
{
    image = NULL;
    (*this) = source;
}

bool Surface::create(int w, int h, int color)
{
    destroy();
    image = SDL_CreateRGBSurface(0, w, h, 32,
                                 0xff0000,
                                 0xff00,
                                 0xff,
                                 0xff000000);
    if (image == NULL) return false;

    clear(color);

    return true;
}

bool Surface::load(const char* path)
{
    image = IMG_Load(path);

    return (image);
}

void Surface::destroy()
{
    if (!image) return;
    SDL_FreeSurface(image);
    image = NULL;
}

Surface::~Surface()
{
    destroy();
}

SDL_Surface* Surface::get()
{
    return image;
}

void Surface::clear(int color)
{
    if (!image) return;

    SDL_FillRect(image, NULL,
                 SDL_MapRGB(image->format,
                    color%0x1000000/0x10000,
                    color%0x10000/0x100,
                    color%0x100));
}

void Surface::clear(Color color)
{
    if (!image) return;

    Uint32 pixel = createUint32Color(image->format, color.r, color.g, color.b, color.a);

    repeat(xA, (unsigned)getWidth())
    {
        repeat(yA, (unsigned)getHeight())
        {
            low_level_putPixelColor(xA, yA, pixel);
        }
    }
}

void Surface::putPixelColor(int x, int y, Color color)
{
    putPixel(x, y, SDL_MapRGBA(image->format, color.r, color.g, color.b, color.a));
}
void Surface::putPixel(int x, int y, Uint32 color)
{
    if (!image) return;

    if (x >= 0 && x < image->w && y >= 0 && y < image->h)
//    SDL_LockSurface(image);
    ((Uint32*)image->pixels)[y * image->w + x] = color;
//    SDL_UnlockSurface(image);
}

int Surface::getWidth()
{
    return (image?image->w:0);
}
int Surface::getHeight()
{
    return (image?image->h:0);
}

void Surface::setColorKey(bool enabled, int color)
{
    if (!image) return;
    SDL_SetColorKey(image, enabled, color);
}

Color Surface::getPixelColor(int x, int y)
{
    if (!image) return Color(0, 0, 0, 0);
    return Color::getColor(image->format, getPixel(x, y));
}

int Surface::getPixel(int x, int y)
{
    if (!image) return -1;
    return ((Uint32*)image->pixels)[y * image->w + x];
//    SDL_UnlockSurface(image);
}

void Surface::savePNG(const char* path)
{
    if (!image) return;
    IMG_SavePNG(image, path);
}

void Surface::low_level_putPixelColor(int x, int y, Uint32 color)
{
    ((Uint32*)image->pixels)[y * image->w + x] = color;
}

Surface& Surface::operator=(Surface& source)
{
    throw std::runtime_error("Surface::operator= not implemented");
}

void Surface::draw(Surface& src, int x, int y)
{
    if (!image) return;
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;

    SDL_BlitSurface(
                    src.get(), NULL,
                    image, &dstrect
                    );
}

void Surface::hack()
{
     /***
    ATENÇÃO, GAMBIARRA A FRENTE
    Por algum motivo algumas imagens estão sendo carregadas com a mascara invertida
    ou seja, imagem que foram salvas ARGB, estão sendo carregadas ABGR
    isso está causando as imagens serem mostradas com as "cores invertidas"
    sendo assim, eu verifico se a mascara está invertida, e se estiver eu inverto
    a posição dos componentes cores
    ***/

    // se não possui mascara retorna
    if (image->format->Rmask == 0) return;
    // se a mascara para AZUL estiver correta retorna
    if (image->format->Bmask == 0xFF) return;

    // faz uma varredura na imagem invertendo os componentes das cores
    int w = getWidth();
    int h = getHeight();
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            Color color = getPixelColor(x, y);

            int temp = color.r;
            color.r = color.b;
            color.b = temp;

            putPixelColor(x, y, color);
        }
    }

    /***
    FIM DA GAMBIARRA
    ***/

}
