#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	typedef std::chrono::high_resolution_clock Time;

	/**
	 * @bfief ��ʱ��
	 */
	class Timer {
	private:
		Time::time_point lastTime; //�ϴμ�ʱ��ʱ��
	public:
		/**
		 * @brief ���캯��
		 */
		Timer();

		/**
		 * @brief ��ʼ��ʱ
		 */
		void  Start();

		/**
		 * @brief ��ȡ���ڵ���ʼ��ʱ��������λ��
		 */
		float GetTime();
	};

	/**
	 * @brief ��һ�ּ�ʱ��
	 */
	class TimerExt {
	private:
		float passTime; //������ʱ��
		float lastPassTime; //�ϴ�ʹ��Pass������ʱ��
	public:
		/**
		 * @brief ���캯��
		 * @param[in] startTime ��ʼ��ʱ��
		 */
		TimerExt(float startTime = 0);

		/**
		 * @brief ����
		 * @param[in] startTime ��ʼ��ʱ��
		 */
		void Reset(float startTime = 0);

		/**
		 * @brief ����һ��ʱ��
		 * @param[in] passTime ������ʱ��
		 */
		void Pass(float passTime);

		/**
		 * @brief ��ȡ������ʱ��
		 * @return ������ʱ��
		 */
		auto GetPassTime() -> float;
		
		/**
		 * @brief ��ȡ�ϴξ�����ʱ��
		 * @return �ϴ�ʹ��Pass������ʱ��
		 */
		auto GetLastPassTime() -> float;
	};

	/**
	 * @brief �򵥵�Fps������
	 */
	class FpsCounter {
	private:
		float currentTime; //��¼��ǰ������ʱ�䣬����1��󽫻��ȥ1��
		int currentFps; //��ǰ״̬�µ�֡��
		int fps; //��һ�뾭����֡��
	public:
		/**
		 * @brief ���캯��
		 */
		FpsCounter();

		/**
		 * @brief �������ݣ���ÿһ֡�������
		 * @param[in] deltaTime ��һ֮֡�侭����ʱ�䣬��λ��
		 */
		void Update(float deltaTime);

		/**
		 * @brief ��ȡ��һ���ڵ�֡��
		 * @return ֡��
		 */
		auto GetFramePerSecond() -> int;
	};

}
