#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief �ļ��������ͣ����ڴ洢�ļ�����
	 */
	class FileData {
	private:
		byte* data; //ָ�������ڴ��ָ�룬���ݰ����ֽڴ洢
	public:
		/**
		 * @brief ���캯��
		 * @param[in] Data �ļ�����
		 */
		FileData(byte* Data);

		/**
		 * @brief �ͷŴ洢���ݵ��ڴ�
		*/
		void Dipose();

		/**
		 * @brief ��ȡ����
		 * @return ���������ڴ��ͷָ��
		 */
		auto GetData() -> byte*;
	};

	/**
	 * @brief �ļ����ݹ������ͣ����ڶ�ȡ����Լ���������
	 */
	class DataManager {
	private:
	public:
		/**
		 * @brief ��ȡ�ļ�
		 * @param[in] fileName �ļ���
		 * @return �����ļ�����
		 */
		auto ReadFile(std::wstring fileName) -> FileData;

		/**
		 * @brief д���ļ�
		 * @param[in] fileName �ļ���
		 * @param[in] fileData Ҫд�������
		 */
		void WriteFile(std::wstring fileName, FileData fileData);
	};

}
