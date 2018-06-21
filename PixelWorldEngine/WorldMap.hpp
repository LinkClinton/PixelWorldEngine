#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ��ͼĳһ�������
	 */
	struct MapData {
		int RenderObjectID; //��ʾ��һ����Ⱦ������ID

		/**
		 * @brief Ĭ�Ϲ��캯��
		 */
		MapData() = default;

		/**
		 * @brief ���캯��
		 * @param[in] renderObjectID ��Ⱦ������ID
		 */
		MapData(int renderObjectID);
	};

	/**
	 * @brief ��ǰ����ĵ�ͼ��������������ĵ������ԣ����Ͻ�Ϊ��ͼ�� 0,0 
	 */
	class WorldMap {
	private:
		int width; //����Ŀ��
		int height; //����ĸ߶�

		std::wstring mapName; //����ĵ�ͼ������
	protected:
		std::vector<MapData*> mapData; //�洢����ĵ�ͼ������Ϣ
	public:
		/** 
		 * @brief ���캯��
		 * @param[in] name ����ĵ�ͼ������
		 * @param[in] Width ����Ŀ��
		 * @param[in] Height ����ĸ߶�
		 * @param[in] data ����ĵ�ͼ��������Ϣ�������С�뱣֤���ڵ���Width x Height
		 */
		WorldMap(std::wstring name, int Width, int Height, MapData** data = nullptr);

		/**
		 * @brief ���õ�ͼָ��λ��������Ϣ
		 * @param[in] x ��ͼ��x����
		 * @param[in] y ��ͼ��y����
		 * @param[in] data ��ͼ������Ϣ��ע�Ᵽ�����ݵ���������
		 */
		void SetMapData(int x, int y, MapData* data);

		/**
		 * @brief ��ȡ��ͼָ��λ��������Ϣ
		 * @param[in] x ��ͼ��x����
		 * @param[in] y ��ͼ��y����
		 * @return ������Ϣ
		 */
		auto GetMapData(int x, int y)->MapData*;

		/**
		 * @brief ��ȡ������
		 * @return ����Ŀ��
		 */
		auto GetWidth() -> int;


		/** 
		 * @brief ��ȡ����ĸ߶�
		 * @return ����ĸ߶�
		 */
		auto GetHeight() -> int;

		/**
		 * @brief ��ȡ����ĵ�ͼ������
		 * @return ����ĵ�ͼ������
		 */
		auto GetMapName() -> std::wstring;
	};

}
