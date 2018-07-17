#pragma once

#include "pch.hpp"

#include "Events.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ����������ͣ���ȡһЩ������Ϣ
	 */
	class Input {
	public:
		/** 
		 * @brief ���ٹ��캯��
		 */
		Input() = delete; 

		/**
		 * @brief ��ȡ��갴���Ƿ���
		 * @param[in] mouseButton ��갴��
		 * @return ������·���true�����򷵻�false
		 */
		static bool GetMouseButtonDown(Events::MouseButton mouseButton);

		/**
		 * @brief ��ȡ��갴���Ƿ���
		 * @param[in] mouseButton ��갴��
		 * @return ������·���false�����򷵻�true
		 */
		static bool GetMouseButtonUp(Events::MouseButton mouseButton);

		/**
		 * @brief ��ȡ���̰����Ƿ���
		 * @param[in] keyCode ���̰���
		 * @return ������·���true�����򷵻�false
		 */
		static bool GetKeyCodeDown(KeyCode keyCode);

		/**
		 * @brief ��ȡ���̰����Ƿ���
		 * @param[in] keyCode ���̰���
		 * @return ������·���false�����򷵻�true
		 */
		static bool GetKeyCodeUp(KeyCode keyCode);

		/**
		 * @brief ����д�����Ƿ���
		 * @return ��������˷���true�����򷵻�false
		 */
		static bool IsCapsLock();

		/**
		 * @brief ������������Ƿ���
		 * @return ��������˷���true�����򷵻�false
		 */
		static bool IsNumLock();

		/**
		 * @brief �����������Ƿ���
		 * @return ��������˷���true�����򷵻�false
		 */
		static bool ScrollLock();

		/**
		 * @brief ��ȡ����X���꣬���������ʵ�����꣬���������Ӧ�ó�����ַ�Χ�ڵ����꣬������ַ�Χ�ʹ��ڴ�Сһ�£���ô������ʵ����
		 * @return ���X����
		 */
		static int GetMousePositionX();

		/**
		* @brief ��ȡ����Y���꣬���������ʵ�����꣬���������Ӧ�ó�����ַ�Χ�ڵ����꣬������ַ�Χ�ʹ��ڴ�Сһ�£���ô������ʵ����
		* @return ���Y����
		*/
		static int GetMousePositionY();

		/**
		* @brief ��ȡ�������꣬���������ʵ�����꣬���������Ӧ�ó�����ַ�Χ�ڵ����꣬������ַ�Χ�ʹ��ڴ�Сһ�£���ô������ʵ����
		* @return �������
		*/
		static auto GetMousePosition() -> std::pair<int, int>;
	};

}