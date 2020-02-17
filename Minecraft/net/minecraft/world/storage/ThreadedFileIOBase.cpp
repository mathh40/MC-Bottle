#include "ThreadedFileIOBase.h"
#include "IThreadedFileIO.h"
std::unique_ptr<ThreadedFileIOBase> ThreadedFileIOBase::INSTANCE = std::make_unique<ThreadedFileIOBase>();

std::unique_ptr<ThreadedFileIOBase>::pointer ThreadedFileIOBase::getThreadedIOInstance()
{
	return INSTANCE.get();
}

void ThreadedFileIOBase::run()
{
	while (true) 
	{
		processQueue();
	}
}

ThreadedFileIOBase::~ThreadedFileIOBase()
{
	thread.join();
}

void ThreadedFileIOBase::queueIO(IThreadedFileIO* fileIo)
{
	++writeQueuedCounter;
	threadedIOQueue.enqueue(fileIo);
}

void ThreadedFileIOBase::waitForFinish()
{
	isThreadWaiting = true;

	while (writeQueuedCounter != savedIOCounter) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10L));
	}

	isThreadWaiting = false;
}

ThreadedFileIOBase::ThreadedFileIOBase()
	: thread(&ThreadedFileIOBase::run,this), writeQueuedCounter(0), savedIOCounter(0), isThreadWaiting(false)
{
}

void ThreadedFileIOBase::processQueue()
{
	IThreadedFileIO* ithreadedfileio = nullptr;
	while (threadedIOQueue.try_dequeue(ithreadedfileio))
	{
		bool flag = ithreadedfileio->writeNextIO();
		if (!flag) 
		{
			++savedIOCounter;
		}

		try 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(isThreadWaiting ? 0L : 10L));
		}
		catch (std::exception& var6) 
		{
			var6.printStackTrace();
		}
	}

	if (threadedIOQueue.size_approx() == 0) 
	{
		try 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(25L));
		}
		catch (std::exception& var5) {
			var5.printStackTrace();
		}
	}
}
