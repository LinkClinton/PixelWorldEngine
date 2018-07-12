#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 负责管理编码
	 */
	class Encoding {
	private:
	public:
		/**
		 * @brief 将UTF-8编码的字符串转换成UTF-16
		 * @param[in] str 字符串，注意必须是UTF-8编码
		 */
		static auto ToUTF16String(std::string str) -> std::u16string;
	};

}