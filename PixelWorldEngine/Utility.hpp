#pragma once

#include "pch.hpp"

#include "GraphicsEnum.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 常用帮助函数集合
	 */
	class Utility {
	public:
		/**
		 * @brief 将字符数组转换为字符串
		 * @param[in] str 字符数组
		 * @return 字符串
		 */
		static auto CharArrayToString(char str[])->std::string;

		/**
		 * @brief 将字符数组转换为动态数组
		 * @param[in] str 字符数组
		 * @return 动态数组
		 */
		static auto CharArrayToVector(char str[])->std::vector<byte>;

		/**
		 * @brief 计算对应的格式每个元素所占的字节数
		 * @param[in] pixelFormat 像素格式
		 * @return 字节大小
		 */
		static auto CountPixelFormatSize(Graphics::PixelFormat pixelFormat) -> int;

		/**
		 * @brief 将整形转换为字符串
		 * @param[in] number 整形
		 * @return 字符串
		 */
		static auto ToString(int number)->std::string;

		/**
		 * @brief Max函数
		 * @param[in] x 
		 * @param[in] y
		 * @return 较大的一个
		 */
		template<typename T>
		static auto Max(T x, T y)->T;

		/**
		* @brief Min函数
		* @param[in] x
		* @param[in] y
		* @return 较小的一个
		*/
		template<typename T>
		static auto Min(T x, T y)->T;

		/**
		 * @brief 将值限制在范围内
		 * @param[in] value 要被限制的值
		 * @param[in] lower 下界
		 * @param[in] upper 上界
		 * @return 最后的结果
		 */
		template<typename T>
		static auto Limit(T value, T lower, T upeer)->T;

		/**
		 * @brief 值是否超出范围
		 * @param[in] value 值
		 * @param[in] lower 下界
		 * @param[in] upper 下界
		 * @return 返回true就代表没有，否则有
		 */
		template<typename T>
		static bool IsLimit(T value, T lower, T upeer);

		/**
		 * @brief 释放资源，必须要有Release成员函数
		 * @param[in] 物体
		 */
		template<typename T>
		static void Dispose(T &object);

		/**
		 * @brief 删除资源，必须是指针并且通过new来创建
		 * @param[in] 物体
		 */
		template<typename T>
		static void Delete(T &object);
	};

	template<typename T>
	inline auto Utility::Max(T x, T y) -> T
	{
		if (x > y) return x;
		return y;
	}

	template<typename T>
	inline auto Utility::Min(T x, T y) -> T
	{
		if (x < y) return x;
		return y;
	}

	template<typename T>
	inline auto Utility::Limit(T value, T lower, T upeer) -> T
	{
		return Max(lower, Min(value, upeer));
	}

	template<typename T>
	inline bool Utility::IsLimit(T value, T lower, T upeer)
	{
		if (value >= lower && value <= upeer) return true;
		return false;
	}

	template<typename T>
	inline void Utility::Dispose(T & object)
	{
		if (object != nullptr)
			object->Release();
		object = nullptr;
	}

	template<typename T>
	inline void Utility::Delete(T & object)
	{
		if (object != nullptr)
			delete object;
		object = nullptr;
	}

}