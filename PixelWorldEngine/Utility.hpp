#pragma once

#include "pch.hpp"

#include "GraphicsEnum.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ���ð�����������
	 */
	class Utility {
	public:
		/**
		 * @brief ���ַ�����ת��Ϊ�ַ���
		 * @param[in] str �ַ�����
		 * @return �ַ���
		 */
		static auto CharArrayToString(char str[])->std::string;

		/**
		 * @brief ���ַ�����ת��Ϊ��̬����
		 * @param[in] str �ַ�����
		 * @return ��̬����
		 */
		static auto CharArrayToVector(char str[])->std::vector<byte>;

		/**
		 * @brief �����Ӧ�ĸ�ʽÿ��Ԫ����ռ���ֽ���
		 * @param[in] pixelFormat ���ظ�ʽ
		 * @return �ֽڴ�С
		 */
		static auto CountPixelFormatSize(Graphics::PixelFormat pixelFormat) -> int;

		/**
		 * @brief ������ת��Ϊ�ַ���
		 * @param[in] number ����
		 * @return �ַ���
		 */
		static auto ToString(int number)->std::string;

		/**
		 * @brief Max����
		 * @param[in] x 
		 * @param[in] y
		 * @return �ϴ��һ��
		 */
		template<typename T>
		static auto Max(T x, T y)->T;

		/**
		* @brief Min����
		* @param[in] x
		* @param[in] y
		* @return ��С��һ��
		*/
		template<typename T>
		static auto Min(T x, T y)->T;

		/**
		 * @brief ��ֵ�����ڷ�Χ��
		 * @param[in] value Ҫ�����Ƶ�ֵ
		 * @param[in] lower �½�
		 * @param[in] upper �Ͻ�
		 * @return ���Ľ��
		 */
		template<typename T>
		static auto Limit(T value, T lower, T upeer)->T;

		/**
		 * @brief ֵ�Ƿ񳬳���Χ
		 * @param[in] value ֵ
		 * @param[in] lower �½�
		 * @param[in] upper �½�
		 * @return ����true�ʹ���û�У�������
		 */
		template<typename T>
		static bool IsLimit(T value, T lower, T upeer);

		/**
		 * @brief �ͷ���Դ������Ҫ��Release��Ա����
		 * @param[in] ����
		 */
		template<typename T>
		static void Dispose(T &object);

		/**
		 * @brief ɾ����Դ��������ָ�벢��ͨ��new������
		 * @param[in] ����
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