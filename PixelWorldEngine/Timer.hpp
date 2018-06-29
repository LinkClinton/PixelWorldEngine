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
		Time::time_point nowTime;  //���ڵ�ʱ��
		std::chrono::duration<float> Time; //ǰ����������ʱ��
		bool started; //��ʱ��״̬
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
		 * @brief ������ʱ
		 */
		void  End();

		/**
		 * @brief ��ȡ��ʱ��״̬����ʾ��ʱ���Ƿ�������
		 */
		bool  GetState();

		/**
		 * @brief ��ȡ�ϴο�ʼ���ϴν���������ʱ�䣬��λ��
		 */
		float GetTime();
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
