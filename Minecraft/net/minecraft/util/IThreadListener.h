#pragma once
#include <future>

class IThreadListener
{
  public:
    virtual ~IThreadListener()                                = default;
    virtual std::future<void> addScheduledTask(Runnable var1) = 0;

    virtual bool isCallingFromMinecraftThread() = 0;
};
