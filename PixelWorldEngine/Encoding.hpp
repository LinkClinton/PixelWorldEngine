#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ¸ºÔð¹ÜÀí±àÂë
	 */
	class Encoding {
	private:
	public:
		/**
		 * @brief ½«UTF-8±àÂëµÄ×Ö·û´®×ª»»³ÉUTF-16
		 * @param[in] str ×Ö·û´®£¬×¢Òâ±ØÐëÊÇUTF-8±àÂë
		 * @return UTF-16±àÂëµÄ×Ö·û´®
		 */
		static auto ToUTF16String(std::string str) -> std::u16string;

#ifdef _WIN32
		/**
		* @brief ½«UTF-8±àÂëµÄ×Ö·û´®×ª»»³É¿í×Ö·û
		* @param[in] str ×Ö·û´®£¬×¢Òâ±ØÐëÊÇUTF-8±àÂë
		* @return ¿í×Ö·û´®
		*/
		static auto ToWideChar(std::string str) -> std::wstring;
#endif // _WIN32

	};

}