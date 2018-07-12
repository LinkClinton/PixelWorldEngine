#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ����������
	 */
	class Encoding {
	private:
	public:
		/**
		 * @brief ��UTF-8������ַ���ת����UTF-16
		 * @param[in] str �ַ�����ע�������UTF-8����
		 * @return UTF-16������ַ���
		 */
		static auto ToUTF16String(std::string str) -> std::u16string;

#ifdef _WIN32
		/**
		* @brief ��UTF-8������ַ���ת���ɿ��ַ�
		* @param[in] str �ַ�����ע�������UTF-8����
		* @return ���ַ���
		*/
		static auto ToWideChar(std::string str) -> std::wstring;
#endif // _WIN32

	};

}