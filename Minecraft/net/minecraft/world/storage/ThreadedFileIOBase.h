#pragma once
#include "IThreadedFileIO.h"
#include "concurrentqueue.h"

#include <atomic>
#include <memory>

class ThreadedFileIOBase
{
  public:
    static std::unique_ptr<ThreadedFileIOBase>::pointer getThreadedIOInstance();
    void run();
    ~ThreadedFileIOBase();
    void queueIO(IThreadedFileIO *fileIo);
    void waitForFinish();

  private:
    static std::unique_ptr<ThreadedFileIOBase> INSTANCE;
    std::thread thread;
    moodycamel::ConcurrentQueue<IThreadedFileIO *> threadedIOQueue;
    std::atomic<int64_t> writeQueuedCounter;
    std::atomic<int64_t> savedIOCounter;
    std::atomic<bool> isThreadWaiting;
    ThreadedFileIOBase();
    void processQueue();
};
