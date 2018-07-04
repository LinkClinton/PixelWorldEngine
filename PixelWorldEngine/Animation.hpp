#pragma once

#include "pch.hpp"

#include "Timer.hpp"
#include "Utility.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 关键帧
	 */
	class KeyFrame {
	private:
		void* data; //存储数据

		float timePos; //时间点

		/**
		 * @brief 构造函数
		 * @param[in] data 数据信息
		 * @param[in] timePos 时间点
		 */
		template<typename T>
		KeyFrame(T data, float timePos);

		/**
		 * @brief 销毁数据，用于释放内存
		 */
		void Destory();

		friend class Animator;
		friend class Animation;
		friend class AnimatorItem;
	public:
		/**
		 * @brief 构造函数销毁
		 */
		KeyFrame() = delete;

		/**
		 * @brief 设置数据信息
		 * @param[in] data 数据信息
		 */
		template<typename T>
		void SetData(T data);

		/**
		 * @brief 设置时间点
		 * @param[in] timePos 时间点
		 */
		void SetTimePos(float timePos);

		/**
		 * @brief 获取数据信息
		 * @return 数据信息
		 */
		template<typename T>
		auto GetData() -> T;

		/**
		 * @brief 获取时间点
		 * @return 时间点
		 */
		auto GetTimePos() -> float;

		/**
		 * @brief 重载小于号
		 */
		bool operator < (KeyFrame keyFrame) const;
	};

	/**
	 * @brief 用于定义动画两帧之间如何处理
	 * @param[in] float 时间点
	 * @param[in] KeyFrame 距离时间点最近且比它小的关键帧
	 * @param[in] KeyFrame 距离时间点最近且比他大的关键帧
	 */
	typedef std::function<KeyFrame(float, KeyFrame, KeyFrame)> FrameProcessUnit;

	/**
	 * @brief 动画类型，请使用指针构建，且请保证在运行动画的时候至少有两个关键帧
	 */
	class Animation {
	private:
		std::vector<KeyFrame> keyFrames; //存储关键帧的数组

		std::string name; //名字

		FrameProcessUnit frameProcessUnit; //处理两帧之间的处理器

		/**
		 * @brief 默认的处理器
		 * @param[in] timePos 时间点
		 * @param[in] lastFrame 距离时间点最近且比它小的关键帧
		 * @param[in] nextFrame 距离时间点最近且比他大的关键帧
		 */
		static auto DefaultProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame)->KeyFrame;

		friend class Animator;
		friend class AnimatorItem;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] name 名字
		 */
		Animation(std::string name);

		/**
		 * @brief 构造函数
		 * @param[in] name 名字
		 * @param[in] frameProcessUnit 使用的处理器 
		 */
		Animation(std::string name, FrameProcessUnit frameProcessUnit);

		/**
		 * @brief 析构函数
		 */
		~Animation();

		/**
		 * @brief 设置关键帧，设置的时候我们的关键帧并不是排好序的
		 * @param[in] data 数据信息
		 * @param[in] timePos 时间点
		 */
		template<typename T>
		void SetKeyFrame(T data, float timePos);

		/**
		 * @brief 将关键帧排序，请在使用动画之前保证其有序
		 */
		void Sort();

		/**
		 * @brief 设置使用的处理器
		 * @param[in] 我们使用的处理器
		 */
		void SetFrameProcessUnit(FrameProcessUnit frameProcessUnit);

		/**
		 * @brief 获取动画的结束时间，我们认为动画开始的时间为0
		 * @return 动画结束的时间
		 */
		auto GetEndTime() -> float;

		/**
		 * @brief 获取名字
		 * @return 名字
		 */
		auto GetName()->std::string;
	};

	/**
	 * @brief 设置函数，即当我们运行动画的时候将会调用它来为我们的目标修改
	 * @param void* 我们的目标，即对应的target值
	 * @param void* 数据信息
	 */
	typedef std::function<void(void*, void*)> AnimationSetFunction;

	/**
	 * 播放的动画
	 */
	class AnimatorItem {
	private:
		void* target; //动画作用的目标

		AnimationSetFunction function; //设置函数

		Animation* animation; //动画

		float startTime; //开始的时间

		int lastFrame; //距离当前时间点最近且比其小的一个关键帧
		int nextFrame; //距离当前时间点最近且比其大的一个关键帧
	private:
		/**
		 * @brief 构造函数
		 * @param[in] target 动画作用的目标
		 * @param[in] function 设置函数
		 * @param[in] animation 动画
		 * @param[in] startTime 开始的时间
		 */
		AnimatorItem(void* target, AnimationSetFunction function, Animation* animation, float startTime);

		/**
		 * @brief 处理动画
		 * @param[in] 当前的时间点
		 */
		void ProcessAnimation(float timePos);

		/**
		 * @brief 重置
		 */
		void Reset();

		friend class Animator;
	public:
		/**
		 * @brief 构造函数销毁
		 */
		AnimatorItem() = delete;

		/**
		 * @brief 重载小于号
		 */
		bool operator < (AnimatorItem item) const;
	};

	/**
	 * @brief 动画管理类型，推荐使用指针
	 */
	class Animator {
	private:
		std::string name; //名字

		bool isRun; //是否在运行
		bool isRepeat; //是否重复播放动画

		float endTime; //结束时间
		float speed; //播放速度

		TimerExt timer; //计时器

		std::set<AnimatorItem> items; //存储要播放的动画

		/**
		 * @brief 更新事件
		 * @param[in] deltaTime 距离上次更新的时间
		 */
		void OnUpdate(float deltaTime); 

		friend class Application;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] name 名字
		 */
		Animator(std::string name);

		/**
		 * @brief 添加动画
		 * @param[in] target 动画作用的目标
		 * @param[in] function 设置函数
		 * @param[in] animation 动画
		 * @param[in] startTime 开始的时间
		 */
		void AddAnimation(void* target, AnimationSetFunction function, Animation* animation, float startTime = 0);

		/**
		 * @brief 播放动画
		 * @param[in] startTime 从什么时候开始播放
		 */
		void Run(float startTime = 0);

		/**
		 * @brief 停止播放
		 */
		void Stop();

		/**
		 * @brief 是否允许重复播放
		 * @param[in] 是否允许
		 */
		void EnableRepeat(bool enable);

		/**
		 * @brief 设置播放速度
		 * @param[in] speed 播放速度
		 */
		void SetSpeed(float speed);

		/**
		 * @brief 是否允许重复播放
		 * @return 结果
		 */
		auto IsRepeat() -> bool;

		/**
		 * @brief 是否在播放动画
		 * @return 结果
		 */
		auto IsRun() -> bool;

		/**
		 * @brief 获取播放速度
		 * @return 播放速度
		 */
		auto GetSpeed() -> float;

		/**
		 * @brief 获取名字
		 * @return 名字
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