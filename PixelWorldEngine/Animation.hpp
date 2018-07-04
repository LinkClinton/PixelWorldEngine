#pragma once

#include "pch.hpp"

#include "Timer.hpp"
#include "Utility.hpp"

namespace PixelWorldEngine {

	class KeyFrame {
	private:
		void* data;

		float timePos;

		friend class Animator;
		friend class Animation;
		friend class AnimatorItem;
	public:
		KeyFrame();

		template<typename T>
		KeyFrame(T data, float timePos);

		template<typename T>
		void SetData(T data);

		void SetTimePos(float timePos);

		template<typename T>
		auto GetData()->T;

		auto GetTimePos() -> float;

		static bool Compare(KeyFrame frame1, KeyFrame frame2);
	};

	typedef std::function<KeyFrame(float, KeyFrame, KeyFrame)> FrameProcessUnit;

	class Animation {
	private:
		std::vector<KeyFrame> keyFrames;

		std::string name;

		FrameProcessUnit frameProcessUnit;

		static auto DefaultProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame)->KeyFrame;

		friend class Animator;
		friend class AnimatorItem;
	public:
		Animation(std::string name, std::vector<KeyFrame> keyFrames);

		Animation(std::string name, std::vector<KeyFrame> keyFrames, FrameProcessUnit frameProcessUnit);

		void SetFrameProcessUnit(FrameProcessUnit frameProcessUnit);

		auto GetEndTime() -> float;

		auto GetName()->std::string;
	};

	typedef std::function<void(void*, void*)> AnimationSetFunction;

	class AnimatorItem {
	private:
		void* target;

		AnimationSetFunction function;

		Animation* animation;

		float startTime;

		int lastFrame;
		int nextFrame;
	private:
		AnimatorItem(void* target, AnimationSetFunction function, Animation* animation, float startTime);

		void ProcessAnimation(float timePos);

		void Reset();

		friend class Animator;
	public:
		AnimatorItem() = delete;

		bool operator < (AnimatorItem item) const;
	};

	class Animator {
	private:
		std::string name;

		bool isRun;
		bool isRepeat;

		float endTime;
		float speed;

		TimerExt timer;

		std::set<AnimatorItem> items;

		void OnUpdate(float deltaTime);

		friend class Application;
	public:
		Animator(std::string name);

		void AddAnimation(void* target, AnimationSetFunction function, Animation* animation, float startTime = 0);

		void Run(float startTime = 0);

		void Stop();

		void EnableRepeat(bool enable);

		void SetSpeed(float speed);

		auto IsRepeat() -> bool;

		auto IsRun() -> bool;

		auto GetSpeed() -> float;

		auto GetName()->std::string;
	};

	template<typename T>
	inline KeyFrame::KeyFrame(T Data, float TimePos)
	{
		data = new T(Data);

		timePos = TimePos;
	}

	template<typename T>
	inline void PixelWorldEngine::KeyFrame::SetData(T Data)
	{
		Utility::Delete(data);

		data = new T(Data);
	}

	template<typename T>
	inline auto PixelWorldEngine::KeyFrame::GetData() -> T
	{
		return T(*data);
	}

}