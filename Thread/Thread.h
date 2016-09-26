#ifndef THREAD_H
#define THREAD_H
#include <windows.h>

#define THREAD_FINISHED 0
#define THREAD_RUNNING 1
#define THREAD_NOT_STARTED -1

class Thread
{
    public:
        Thread();
        virtual ~Thread();

        void start();

        virtual void run()=0;

        int getStatus() {return this->status;};
        void setStatus(int i) {this->status = i;};

        DWORD getId() {return this->id;};
        HANDLE getHandle() {return this->handle;};

    protected:
        int status;
    private:
        HANDLE handle;
        DWORD id;

        void stop();

};
#endif // THREAD_H
