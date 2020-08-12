#include "FPSCounter.h"

void tge::FPSCounter::Update()
{
	_sec += TimeManager::GetDeltaTimeF();
	_frames++;
	if (_sec >= 1.f) {
		if (is_active)
			printf("FPS: %u\n", _frames);
		_sec = 0;
		_frames = 0;
	}
}