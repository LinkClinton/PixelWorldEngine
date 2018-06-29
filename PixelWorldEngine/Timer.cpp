#include "Timer.hpp"


PixelWorldEngine::Timer::Timer() {
	started = false;
}


void PixelWorldEngine::Timer::Start() {
	lastTime = Time::now();
	started = true;
}

void PixelWorldEngine::Timer::End() {
	nowTime = Time::now();
	Time = std::chrono::duration_cast<std::chrono::duration<float>>(nowTime - lastTime);
	
	started = false;
}


bool PixelWorldEngine::Timer::GetState() {
	return started;
}


float PixelWorldEngine::Timer::GetTime() {
	return Time.count();
}

PixelWorldEngine::FpsCounter::FpsCounter()
{
	currentTime = 0;
	currentFps = 0;
	fps = 0;
}

void PixelWorldEngine::FpsCounter::Update(float deltaTime)
{
	currentFps++;

	currentTime += deltaTime;

	if (currentTime > 1.0f) {
		fps = currentFps;

		currentTime = currentTime - 1.0f;
		currentFps = 0;
	}
}

auto PixelWorldEngine::FpsCounter::GetFramePerSecond() -> int
{
	return fps;
}


