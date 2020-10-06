#include "clicker.h"

#include <iostream>
#include <random>

#include <Windows.h>

using namespace std;

Clicker::~Clicker()
{
	//Stop clicking graceful
	if (ClickingThread.joinable())
	{
		IsClicking_ = false;
		ClickingThread.join();
	}
}

void Clicker::StartClicking(uint32_t interval_ms_lower, uint32_t interval_ms_upper, KeyType key)
{
	concurrency::critical_section::scoped_lock lock(StartCS_);

	//If clicking thread is active, stop first
	if (IsClicking_ || ClickingThread.joinable())
	{
		StopClicking();
	}

	IsClicking_ = true;
	//Start new thread
	ClickingThread = thread(&Clicker::clickThreadFunction, this, interval_ms_lower, interval_ms_upper, key);
}

void Clicker::StopClicking()
{
	IsClicking_ = false;

	if (ClickingThread.joinable())
	{
		ClickingThread.join();
	}
}

void Clicker::clickThreadFunction(uint32_t interval_ms_lower, uint32_t interval_ms_upper, KeyType key)
{
	//In case for some unknown problem
	this_thread::sleep_for(30ms);

	std::random_device random_device{};
	mt19937 random_engine(random_device());
	uniform_int_distribution<> interval_generator(interval_ms_lower, interval_ms_upper);


	while (IsClicking_)
	{
		//Random generate interval
		auto interval = chrono::milliseconds(interval_generator(random_engine));
		sendClickKey(key);
		this_thread::sleep_for(interval);
	}
}

void Clicker::sendClickKey(KeyType key)
{
	DWORD downFlags = 0, upFlags = 0;

	switch (key)
	{
		case KeyType::RightKey:
		{
			downFlags = MOUSEEVENTF_RIGHTDOWN;
			upFlags = MOUSEEVENTF_RIGHTUP;
			break;
		}
		case KeyType::MiddleKey:
		{
			downFlags = MOUSEEVENTF_MIDDLEDOWN;
			upFlags = MOUSEEVENTF_MIDDLEUP;
			break;
		}
		case KeyType::LeftKey:
		default:
		{
			downFlags = MOUSEEVENTF_LEFTDOWN;
			upFlags = MOUSEEVENTF_LEFTUP;
			break;
		}
	}

	//DNF must use mouse_event here
	
	mouse_event(downFlags, 0, 0, 0, 0);
	this_thread::sleep_for(10ms); //Need a little sleep or it will no effect in DNF
	mouse_event(upFlags, 0, 0, 0, 0);
}
