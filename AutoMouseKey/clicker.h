#pragma once

#include <cstdint>

#include <functional>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <concrt.h>
#include <Windows.h>


enum class KeyType
{
	LeftKey = 0,
	RightKey = 1,
	MiddleKey = 2
};

class Clicker
{
	
public:

	
	Clicker() = default;
	//Copy is forbidden
	Clicker(const Clicker&) = delete;
	bool operator =(const Clicker&) = delete;
	~Clicker();

	void StartClicking(uint32_t interval_ms_lower = 10, uint32_t interval_ms_upper = 50, KeyType key = KeyType::LeftKey);
	void StopClicking();
private:

	
	void clickThreadFunction(uint32_t uinterval_ms_lower, uint32_t uinterval_ms_upper, KeyType key);

	static void sendClickKey(KeyType key);

	concurrency::critical_section StartCS_;
	
	std::thread ClickingThread;
	
	std::atomic<bool> IsClicking_ { false } ;
};