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
		Time::time_point nowTime;  //现在的时间
		std::chrono::duration<float> Time; //前面两者相差的时间
		bool started; //计时器状态
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
		 * @brief 结束计时
		 */
		void  End();

		/**
		 * @brief 获取计时器状态，表示计时器是否在运行
		 */
		bool  GetState();

		/**
		 * @brief 获取上次开始到上次结束经过的时间，单位秒
		 */
		float GetTime();
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
