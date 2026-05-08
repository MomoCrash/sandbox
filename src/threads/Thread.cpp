#include "Thread.h"

namespace US
{

	Thread::Thread(const char* name, bool startPaused)
		: mhThread(new ThreadHandle<char>(name, this, startPaused))
	{
		mHandle = nullptr;
	}

	Thread::~Thread()
	{
		Terminate();
	}

	HANDLE Thread::GetHandle() const
	{
		return mHandle;
	}

	uint32_t Thread::GetID() const
	{
		return mID;
	}

	ThreadStatusCode Thread::GetStatus()
	{
		return getHandlePtr()->GetStatus();
	}

	ThreadStatusCode Thread::TerminateWait(uint32_t maxDuration)
	{
		Terminate();
		WaitForSingleObject(mHandle, maxDuration);
		mHandle = nullptr;
		mID = 0;
		return GetStatus();
	}

	void Thread::Terminate()
	{
		getHandlePtr()->SafeTerminate();
	}

	void Thread::Pause()
	{
		getHandlePtr()->SafePause();
	}

	bool Thread::IsPaused() {
		return getHandlePtr()->IsPaused();
	}

	void Thread::Resume()
	{
		getHandlePtr()->SafeResume();
	}

	ThreadHandle<char>* Thread::getHandlePtr() const {
		return static_cast<ThreadHandle<char>*>(mhThread);
	}

}