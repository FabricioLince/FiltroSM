#include "thread.h"
#include <stdexcept>

Thread::Thread()
{
    this->handle = NULL;
    this->id = 0;
    this->status = THREAD_NOT_STARTED;
}

Thread::~Thread()
{
    this->stop();
}

LPTHREAD_START_ROUTINE runThread(void*s)
{
    ((Thread*)s)->setStatus(THREAD_RUNNING);
    ((Thread*)s)->run();
    ((Thread*)s)->setStatus(THREAD_FINISHED);
    return 0;
}

void Thread::start()
{
    if (this->status == THREAD_RUNNING) throw new std::exception();

    // Create the thread to begin execution on its own.

    this->handle = CreateThread
    (
        NULL,                        // default security attributes
        0,                              // use default stack size
        (LPTHREAD_START_ROUTINE)runThread,                      // thread function name
        (void*)this,                       // argument to thread function
        0,                           // use default creation flags
        &this->id                          // returns the thread identifier
    );

    if (this->handle == NULL) throw new std::runtime_error("Falha na criacao da Thread");
}

void Thread::stop()
{
    if (this->handle)
        CloseHandle(this->handle);
    this->handle = NULL;
}
