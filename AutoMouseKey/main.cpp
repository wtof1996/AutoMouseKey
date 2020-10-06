#include <iostream>

#include "clicker.h"

using namespace std;

constexpr const char* Version = "V1.0";
constexpr int LeftKeyStartEvent = 0x1122;
constexpr int RightKeyStartEvent = 0x2211;
constexpr int StopEvent = 0x6666;
constexpr int ExitEvent = 0x8888;


void WelcomInfo()
{
	cout << "AutoMouseKey " << Version << " by wtof" << endl;
	cout << "Use at your own Risk!" << endl;
	cout << "Alt + L     Start clicking the left mouse key" << endl;
	cout << "Alt + R     Start clicking the right mouse key" << endl;
	cout << "Alt + P     Stop current clicking" << endl;
	cout << "Alt + X     Exit" << endl;
}

void EventLoop(uint32_t interval_lower = 10, uint32_t interval_upper = 100)
{
	Clicker clicker;

	//Register global hotkey
	RegisterHotKey(nullptr, LeftKeyStartEvent, MOD_ALT, 0x4c); //Alt + L
	RegisterHotKey(nullptr, RightKeyStartEvent, MOD_ALT, 0x52); //Alt + R
	RegisterHotKey(nullptr, StopEvent, MOD_ALT, 0x50); //Alt + P
	RegisterHotKey(nullptr, ExitEvent, MOD_ALT, 0x58); //Alt + X

	MSG msg{};

	//Keep pull out message and check
	while (GetMessage(&msg, nullptr, 0, 0) != 0)
	{
		if (msg.message != WM_HOTKEY) continue;

		if (msg.wParam == LeftKeyStartEvent)
		{
			clicker.StartClicking(interval_lower, interval_upper, KeyType::LeftKey);
		}
		else if (msg.wParam == RightKeyStartEvent)
		{
			clicker.StartClicking(interval_lower, interval_upper, KeyType::RightKey);

		}
		else if (msg.wParam == StopEvent)
		{
			clicker.StopClicking();
		}
		else if (msg.wParam == ExitEvent)
		{
			clicker.StopClicking();
			break;
		}

	}

	UnregisterHotKey(nullptr, LeftKeyStartEvent);
	UnregisterHotKey(nullptr, RightKeyStartEvent);
	UnregisterHotKey(nullptr, StopEvent);
	UnregisterHotKey(nullptr, ExitEvent);
}

int main(int argc, char* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	WelcomInfo();

	//Start event loop in another thread
	
	thread event_loop([]()
		{
			EventLoop();
		});

	event_loop.join();
	
	return 0;
}