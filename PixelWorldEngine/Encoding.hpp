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
		 */
		static auto ToUTF16String(std::string str) -> std::u16string;
	};

}