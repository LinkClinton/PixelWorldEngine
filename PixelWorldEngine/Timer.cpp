#include "Timer.hpp"


PixelWorldEngine::Timer::Timer() {
	lastTime = Time::now();
}


void PixelWorldEngine::Timer::Start() {
	lastTime = Time::now();
}

float PixelWorldEngine::Timer::GetTime() {	
	auto Time = std::chrono::duration_cast<std::chrono::duration<float>>(Time::now() - lastTime);
	
	return Time.count();
}

PixelWorldEngine::TimerExt::TimerExt(float startTime)
{
	lastPassTime = 0;
	passTime = startTime;
}

void PixelWorldEngine::TimerExt::Reset(float startTime)
{
	lastPassTime = 0;
	passTime = startTime;
}

void PixelWorldEngine::TimerExt::Pass(float PassTime)
{
	lastPassTime = passTime;
	passTime += PassTime;
}

auto PixelWorldEngine::TimerExt::GetPassTime() -> float
{
	return passTime;
}

auto PixelWorldEngine::TimerExt::GetLastPassTime() -> float
{
	return lastPassTime;
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


