#include "Animation.hpp"

#include "DebugLayer.hpp"
#include "Application.hpp"

PixelWorldEngine::KeyFrame::KeyFrame()
{
	data = nullptr;

	timePos = 0;
}

void PixelWorldEngine::KeyFrame::SetTimePos(float TimePos)
{
	timePos = TimePos;
}

auto PixelWorldEngine::KeyFrame::GetTimePos() -> float
{
	return timePos;
}

bool PixelWorldEngine::KeyFrame::Compare(KeyFrame frame1, KeyFrame frame2)
{
	return frame1.timePos < frame2.timePos;
}

auto PixelWorldEngine::Animation::DefaultProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame) -> KeyFrame
{
	if (timePos == nextFrame.timePos) return nextFrame;
	return lastFrame;
}

PixelWorldEngine::Animation::Animation(std::string Name, std::vector<KeyFrame> KeyFrames)
{
	name = Name;

	keyFrames = KeyFrames;

	DebugLayer::Assert(keyFrames.size() < 2, Error::NeedMoreThanOneFrame, name);

	std::sort(keyFrames.begin(), keyFrames.end(), KeyFrame::Compare);

	frameProcessUnit = DefaultProcessUnit;
}

PixelWorldEngine::Animation::Animation(std::string Name, std::vector<KeyFrame> KeyFrames, FrameProcessUnit FrameProcessUnit)
	:Animation(Name, KeyFrames)
{
	frameProcessUnit = FrameProcessUnit;
}

void PixelWorldEngine::Animation::SetFrameProcessUnit(FrameProcessUnit FrameProcessUnit)
{
	frameProcessUnit = FrameProcessUnit;
}

auto PixelWorldEngine::Animation::GetEndTime() -> float
{
	return keyFrames[keyFrames.size() - 1].timePos;
}

auto PixelWorldEngine::Animation::GetName() -> std::string
{
	return name;
}

void PixelWorldEngine::AnimatorItem::ProcessAnimation(float timePos)
{
	if (timePos > animation->GetEndTime()) return;

	while (animation->keyFrames[nextFrame].timePos <= timePos) {
		lastFrame = nextFrame;

		nextFrame++;

		if (nextFrame == animation->keyFrames.size()) {
			function(target, animation->keyFrames[lastFrame].data);
			Reset(); return;
		}
	}

	auto frame = animation->frameProcessUnit(timePos, animation->keyFrames[lastFrame],
		animation->keyFrames[nextFrame]);

	function(target, frame.data);
}

PixelWorldEngine::AnimatorItem::AnimatorItem(void* Target, AnimationSetFunction Function, Animation* Animation, float StartTime) {
	target = Target;
	function = Function;
	animation = Animation;
	startTime = StartTime;

	lastFrame = 0;
	nextFrame = 1;
}

void PixelWorldEngine::AnimatorItem::Reset()
{
	lastFrame = 0;
	nextFrame = 1;
}

bool PixelWorldEngine::AnimatorItem::operator<(AnimatorItem item) const
{
	return startTime < item.startTime;
}

void PixelWorldEngine::Animator::OnUpdate(float deltaTime)
{
	if (isRun == false) return;

	for (auto it = items.begin(); it != items.end(); it++) {
		auto item = *it;

		if (timer.GetPassTime() > item.animation->GetEndTime() + item.startTime) {
			if (timer.GetLastPassTime() <= item.animation->GetEndTime() + item.startTime)
				item.ProcessAnimation(item.animation->GetEndTime());

			continue;
		}

		if (timer.GetPassTime() < item.startTime) break;

		item.ProcessAnimation(timer.GetPassTime() - item.startTime);
	}

	if (timer.GetPassTime() > endTime) {
		if (isRepeat == true) 
			timer.Reset(timer.GetPassTime() - endTime);
		else {
			Stop(); return;
		}
	}

	timer.Pass(deltaTime * speed);
}

PixelWorldEngine::Animator::Animator(std::string Name)
{
	name = Name;

	isRepeat = false;
	isRun = false;

	speed = 1;
	endTime = 0;
}

void PixelWorldEngine::Animator::AddAnimation(void * target, AnimationSetFunction function, Animation * animation, float startTime)
{
	DebugLayer::Assert(isRun, Error::CanNotAddAnimationWhenAnimatorRun, animation->name, name);

	items.insert(AnimatorItem(target, function, animation, startTime));

	endTime = Utility::Max(endTime, startTime + animation->GetEndTime());
}

void PixelWorldEngine::Animator::Run(float startTime)
{
	isRun = true;

	timer.Reset(startTime);
}

void PixelWorldEngine::Animator::Stop()
{
	isRun = false;
}

void PixelWorldEngine::Animator::EnableRepeat(bool enable)
{
	isRepeat = enable;
}

void PixelWorldEngine::Animator::SetSpeed(float Speed)
{
	speed = Speed;
}

auto PixelWorldEngine::Animator::IsRepeat() -> bool
{
	return isRepeat;
}

auto PixelWorldEngine::Animator::IsRun() -> bool
{
	return isRun;
}

auto PixelWorldEngine::Animator::GetSpeed() -> float
{
	return speed;
}

auto PixelWorldEngine::Animator::GetName() -> std::string
{
	return name;
}