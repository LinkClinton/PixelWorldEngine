#pragma once

#include "pch.hpp"

#include "Events.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 输入管理类型，获取一些输入信息
	 */
	class Input {
	public:
		/** 
		 * @brief 销毁构造函数
		 */
		Input() = delete; 

		/**
		 * @brief 获取鼠标按键是否按下
		 * @param[in] mouseButton 鼠标按键
		 * @return 如果按下返回true，否则返回false
		 */
		static bool GetMouseButtonDown(Events::MouseButton mouseButton);

		/**
		 * @brief 获取鼠标按键是否按下
		 * @param[in] mouseButton 鼠标按键
		 * @return 如果按下返回false，否则返回true
		 */
		static bool GetMouseButtonUp(Events::MouseButton mouseButton);

		/**
		 * @brief 获取键盘按键是否按下
		 * @param[in] keyCode 键盘按键
		 * @return 如果按下返回true，否则返回false
		 */
		static bool GetKeyCodeDown(KeyCode keyCode);

		/**
		 * @brief 获取键盘按键是否按下
		 * @param[in] keyCode 键盘按键
		 * @return 如果按下返回false，否则返回true
		 */
		static bool GetKeyCodeUp(KeyCode keyCode);

		/**
		 * @brief 检测大写锁定是否开启
		 * @return 如果开启了返回true，否则返回false
		 */
		static bool IsCapsLock();

		/**
		 * @brief 检测数字锁定是否开启
		 * @return 如果开启了返回true，否则返回false
		 */
		static bool IsNumLock();

		/**
		 * @brief 检测滚动锁定是否开启
		 * @return 如果开启了返回true，否则返回false
		 */
		static bool ScrollLock();

		/**
		 * @brief 获取鼠标的X坐标，不是鼠标真实的坐标，而是鼠标在应用程序呈现范围内的坐标，如果呈现范围和窗口大小一致，那么就是真实坐标
		 * @return 鼠标X坐标
		 */
		static int GetMousePositionX();

		/**
		* @brief 获取鼠标的Y坐标，不是鼠标真实的坐标，而是鼠标在应用程序呈现范围内的坐标，如果呈现范围和窗口大小一致，那么就是真实坐标
		* @return 鼠标Y坐标
		*/
		static int GetMousePositionY();

		/**
		* @brief 获取鼠标的坐标，不是鼠标真实的坐标，而是鼠标在应用程序呈现范围内的坐标，如果呈现范围和窗口大小一致，那么就是真实坐标
		* @return 鼠标坐标
		*/
		static auto GetMousePosition() -> std::pair<int, int>;
	};

}