#pragma once

#include "pch.hpp"

#include "Timer.hpp"
#include "Utility.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief �ؼ�֡
	 */
	class KeyFrame {
	private:
		void* data; //�洢����

		float timePos; //ʱ���

		/**
		 * @brief ���캯��
		 * @param[in] data ������Ϣ
		 * @param[in] timePos ʱ���
		 */
		template<typename T>
		KeyFrame(T data, float timePos);

		/**
		 * @brief �������ݣ������ͷ��ڴ�
		 */
		void Destory();

		friend class Animator;
		friend class Animation;
		friend class AnimatorItem;
	public:
		/**
		 * @brief ���캯������
		 */
		KeyFrame() = delete;

		/**
		 * @brief ����������Ϣ
		 * @param[in] data ������Ϣ
		 */
		template<typename T>
		void SetData(T data);

		/**
		 * @brief ����ʱ���
		 * @param[in] timePos ʱ���
		 */
		void SetTimePos(float timePos);

		/**
		 * @brief ��ȡ������Ϣ
		 * @return ������Ϣ
		 */
		template<typename T>
		auto GetData() -> T;

		/**
		 * @brief ��ȡʱ���
		 * @return ʱ���
		 */
		auto GetTimePos() -> float;

		/**
		 * @brief ����С�ں�
		 */
		bool operator < (KeyFrame keyFrame) const;
	};

	/**
	 * @brief ���ڶ��嶯����֮֡����δ���
	 * @param[in] float ʱ���
	 * @param[in] KeyFrame ����ʱ�������ұ���С�Ĺؼ�֡
	 * @param[in] KeyFrame ����ʱ�������ұ�����Ĺؼ�֡
	 */
	typedef std::function<KeyFrame(float, KeyFrame, KeyFrame)> FrameProcessUnit;

	/**
	 * @brief �������ͣ���ʹ��ָ�빹�������뱣֤�����ж�����ʱ�������������ؼ�֡
	 */
	class Animation {
	private:
		std::vector<KeyFrame> keyFrames; //�洢�ؼ�֡������

		std::string name; //����

		FrameProcessUnit frameProcessUnit; //������֮֡��Ĵ�����

		/**
		 * @brief Ĭ�ϵĴ�����
		 * @param[in] timePos ʱ���
		 * @param[in] lastFrame ����ʱ�������ұ���С�Ĺؼ�֡
		 * @param[in] nextFrame ����ʱ�������ұ�����Ĺؼ�֡
		 */
		static auto DefaultProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame)->KeyFrame;

		friend class Animator;
		friend class AnimatorItem;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] name ����
		 */
		Animation(std::string name);

		/**
		 * @brief ���캯��
		 * @param[in] name ����
		 * @param[in] frameProcessUnit ʹ�õĴ����� 
		 */
		Animation(std::string name, FrameProcessUnit frameProcessUnit);

		/**
		 * @brief ��������
		 */
		~Animation();

		/**
		 * @brief ���ùؼ�֡�����õ�ʱ�����ǵĹؼ�֡�������ź����
		 * @param[in] data ������Ϣ
		 * @param[in] timePos ʱ���
		 */
		template<typename T>
		void SetKeyFrame(T data, float timePos);

		/**
		 * @brief ���ؼ�֡��������ʹ�ö���֮ǰ��֤������
		 */
		void Sort();

		/**
		 * @brief ����ʹ�õĴ�����
		 * @param[in] ����ʹ�õĴ�����
		 */
		void SetFrameProcessUnit(FrameProcessUnit frameProcessUnit);

		/**
		 * @brief ��ȡ�����Ľ���ʱ�䣬������Ϊ������ʼ��ʱ��Ϊ0
		 * @return ����������ʱ��
		 */
		auto GetEndTime() -> float;

		/**
		 * @brief ��ȡ����
		 * @return ����
		 */
		auto GetName()->std::string;
	};

	/**
	 * @brief ���ú����������������ж�����ʱ�򽫻��������Ϊ���ǵ�Ŀ���޸�
	 * @param void* ���ǵ�Ŀ�꣬����Ӧ��targetֵ
	 * @param void* ������Ϣ
	 */
	typedef std::function<void(void*, void*)> AnimationSetFunction;

	/**
	 * ���ŵĶ���
	 */
	class AnimatorItem {
	private:
		void* target; //�������õ�Ŀ��

		AnimationSetFunction function; //���ú���

		Animation* animation; //����

		float startTime; //��ʼ��ʱ��

		int lastFrame; //���뵱ǰʱ�������ұ���С��һ���ؼ�֡
		int nextFrame; //���뵱ǰʱ�������ұ�����һ���ؼ�֡
	private:
		/**
		 * @brief ���캯��
		 * @param[in] target �������õ�Ŀ��
		 * @param[in] function ���ú���
		 * @param[in] animation ����
		 * @param[in] startTime ��ʼ��ʱ��
		 */
		AnimatorItem(void* target, AnimationSetFunction function, Animation* animation, float startTime);

		/**
		 * @brief ������
		 * @param[in] ��ǰ��ʱ���
		 */
		void ProcessAnimation(float timePos);

		/**
		 * @brief ����
		 */
		void Reset();

		friend class Animator;
	public:
		/**
		 * @brief ���캯������
		 */
		AnimatorItem() = delete;

		/**
		 * @brief ����С�ں�
		 */
		bool operator < (AnimatorItem item) const;
	};

	/**
	 * @brief �����������ͣ��Ƽ�ʹ��ָ��
	 */
	class Animator {
	private:
		std::string name; //����

		bool isRun; //�Ƿ�������
		bool isRepeat; //�Ƿ��ظ����Ŷ���

		float endTime; //����ʱ��
		float speed; //�����ٶ�

		TimerExt timer; //��ʱ��

		std::set<AnimatorItem> items; //�洢Ҫ���ŵĶ���

		/**
		 * @brief �����¼�
		 * @param[in] deltaTime �����ϴθ��µ�ʱ��
		 */
		void OnUpdate(float deltaTime); 

		friend class Application;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] name ����
		 */
		Animator(std::string name);

		/**
		 * @brief ��Ӷ���
		 * @param[in] target �������õ�Ŀ��
		 * @param[in] function ���ú���
		 * @param[in] animation ����
		 * @param[in] startTime ��ʼ��ʱ��
		 */
		void AddAnimation(void* target, AnimationSetFunction function, Animation* animation, float startTime = 0);

		/**
		 * @brief ���Ŷ���
		 * @param[in] startTime ��ʲôʱ��ʼ����
		 */
		void Run(float startTime = 0);

		/**
		 * @brief ֹͣ����
		 */
		void Stop();

		/**
		 * @brief �Ƿ������ظ�����
		 * @param[in] �Ƿ�����
		 */
		void EnableRepeat(bool enable);

		/**
		 * @brief ���ò����ٶ�
		 * @param[in] speed �����ٶ�
		 */
		void SetSpeed(float speed);

		/**
		 * @brief �Ƿ������ظ�����
		 * @return ���
		 */
		auto IsRepeat() -> bool;

		/**
		 * @brief �Ƿ��ڲ��Ŷ���
		 * @return ���
		 */
		auto IsRun() -> bool;

		/**
		 * @brief ��ȡ�����ٶ�
		 * @return �����ٶ�
		 */
		auto GetSpeed() -> float;

		/**
		 * @brief ��ȡ����
		 * @return ����
		 */
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

	template<typename T>
	inline void Animation::SetKeyFrame(T data, float timePos)
	{
		keyFrames.push_back(KeyFrame(data, timePos));
	}

}