
#include "Filtro/PNGImage.h"
#include "Filtro/Filtro.h"
#include <vector>
#include "Thread/Thread.h"

class Processor
{
    PNGImage* source;
    PNGImage* resultPNG;
    public:
        Processor(PNGImage* src)
        {
            if (src == NULL) throw "no foto to process";
            source = src;
            resultPNG = new PNGImage(source->getWidth(), source->getHeight());
        }
        ~Processor()
        {
            delete resultPNG;
        }
        void foo();
        void doCut(int x, int y, int w, int h, int filterNumber);
        void saveResult(const char* path)
        {
            resultPNG->save(path);
        }
        std::vector<FiltroExemplo*> filtroList;
};

void Processor::doCut(int x, int y, int w, int h, int i)
{
    PNGImage* cutted = NULL;
    PNGImage* cuttedResult = NULL;
    PPMImage* ppmResult = NULL;
    PPMImage* image = NULL;

        cutted = source->cut(x, y, w, h);
        image = cutted->convertToPPM();

        ppmResult = (PPMImage*)filtroList.at(i)->applyTo(image);

        cuttedResult = new PNGImage(ppmResult);
        resultPNG->surface->draw(*cuttedResult->surface, x, y);

    delete cutted;
    delete image;
    delete ppmResult;
    delete cuttedResult;

    printf("Aplicado filtro #%d!\n", i);
}

class ProcessorThread : public Thread
{
    Processor& processor;
    int x;
    int y;
    int w;
    int h;
    int i;

    public:
        ProcessorThread(Processor& p, int x_, int y_, int w_, int h_, int filterIndex);
        void run();
};

ProcessorThread::ProcessorThread(Processor& p, int x_, int y_, int w_, int h_, int filterIndex)
: processor(p), x(x_), y(y_), w(w_), h(h_), i(filterIndex)
{}

void ProcessorThread::run()
{
    processor.doCut(x, y, w, h, i);
}

#define USE_THREADS true

int main(int argc, char**argv)
{
    PNGImage* source = NULL;

    try
    {
        source = new PNGImage("imgs/gf.png");
        printf("Imagem carregada...\n");
        Processor proc(source);

        int numFilters = 10;
        for (int i=0; i < numFilters; i++)
        {
            proc.filtroList.push_back(new FiltroExemplo(5, 8));
        }

        #if USE_THREADS
        ProcessorThread* threadList[numFilters];
        #endif

        int w = source->getWidth()/numFilters;
        int h = source->getHeight();
        for (int i=0; i < numFilters; i++)
        {
            #if USE_THREADS
            threadList[i] = new ProcessorThread(proc, w*i, 0, w, h, i);
            threadList[i]->start();
            #else
            proc.doCut(w*i, 0, w, h, i);
            #endif
        }

        #if USE_THREADS
        for (int i=0; i < numFilters; i++)
        {
            while (threadList[i]->getStatus() == THREAD_RUNNING);
            delete threadList[i];
        }
        #endif

        while(proc.filtroList.empty() == false)
        {
            delete proc.filtroList.back();
            proc.filtroList.pop_back();
        }

        proc.saveResult("imgs/resultgf.png");
    }
    catch (const char* err)
    {
        printf("ERROR: %s\n", err);
    }

    delete source;
    return 0;
}
