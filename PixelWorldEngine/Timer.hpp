#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	typedef std::chrono::high_resolution_clock Time;

	/**
	 * @bfief 计时器
	 */
	class Timer {
	private:
		Time::time_point lastTime; //上次计时的时间
	public:
		/**
		 * @brief 构造函数
		 */
		Timer();

		/**
		 * @brief 开始计时
		 */
		void  Start();

		/**
		 * @brief 获取现在到开始的时间间隔，单位秒
		 */
		float GetTime();
	};

	/**
	 * @brief 另一种计时器
	 */
	class TimerExt {
	private:
		float passTime; //经过的时间
		float lastPassTime; //上次使用Pass经过的时间
	public:
		/**
		 * @brief 构造函数
		 * @param[in] startTime 开始的时间
		 */
		TimerExt(float startTime = 0);

		/**
		 * @brief 重置
		 * @param[in] startTime 开始的时间
		 */
		void Reset(float startTime = 0);

		/**
		 * @brief 经过一段时间
		 * @param[in] passTime 经过的时间
		 */
		void Pass(float passTime);

		/**
		 * @brief 获取经过的时间
		 * @return 经过的时间
		 */
		auto GetPassTime() -> float;
		
		/**
		 * @brief 获取上次经过的时间
		 * @return 上次使用Pass经过的时间
		 */
		auto GetLastPassTime() -> float;
	};

	/**
	 * @brief 简单的Fps计算器
	 */
	class FpsCounter {
	private:
		float currentTime; //记录当前经过的时间，超过1秒后将会减去1秒
		int currentFps; //当前状态下的帧数
		int fps; //上一秒经过的帧数
	public:
		/**
		 * @brief 构造函数
		 */
		FpsCounter();

		/**
		 * @brief 更新数据，在每一帧过后更新
		 * @param[in] deltaTime 这一帧之间经过的时间，单位秒
		 */
		void Update(float deltaTime);

		/**
		 * @brief 获取上一秒内的帧数
		 * @return 帧数
		 */
		auto GetFramePerSecond() -> int;
	};

}
