#pragma once

#include "pch.hpp"
#include "Utility.hpp"

namespace PixelWorldEngine {

#define DEFAULT_BLOCK_SIZE 32

	/**
	 * @brief ��ͼ��
	 */
	struct MapData {
		int RenderObjectID; //��Ⱦ��š�Ϊ0�Ļ����ʹ�����Ⱦ
		float Opacity; //��͸���ȣ�Ĭ��Ϊ1

		/**
		 * @brief Ĭ�Ϲ��캯��
		 */
		MapData();
	};

	/**
	 * @brief ��ǰ����ĵ�ͼ��������������ĵ������ԣ����Ͻ�Ϊ��ͼ�� (0,0) 
	 */
	class WorldMap {
	private:
		int width; //����Ŀ��
		int height; //����ĸ߶�

		float mapBlockSize; //��ͼ���ӵĴ�С��Ĭ��Ϊ32

		std::string mapName; //����ĵ�ͼ������
	protected:
		std::vector<MapData> mapData; //�洢����ĵ�ͼ������Ϣ
	public:
		/** 
		 * @brief ���캯��
		 * @param[in] name ����ĵ�ͼ������
		 * @param[in] Width ����Ŀ��
		 * @param[in] Height ����ĸ߶�
		 * @param[in] MapBlockSize ����ĸ��Ӵ�С��Ĭ��Ϊ32
		 * @param[in] data ����ĵ�ͼ��������Ϣ�������С�뱣֤���ڵ���Width * Height
		 */
		WorldMap(std::string name, int Width, int Height, float MapBlockSize = DEFAULT_BLOCK_SIZE, MapData* data = nullptr);

		/**
		 * @brief ���õ�ͼָ��λ��������Ϣ
		 * @param[in] x ��ͼ��x����
		 * @param[in] y ��ͼ��y����
		 * @param[in] data ��ͼ������Ϣ��ע�Ᵽ�����ݵ���������
		 */
		void SetMapData(int x, int y, MapData data);

		/**
		 * @brief ���õ�ͼ���Ӵ�С
		 * @param[in] ��ͼ���Ӵ�С��Ĭ��Ϊ32
		 */
		void SetMapBlockSize(float size);

		/**
		 * @brief ��ȡ��ͼָ��λ��������Ϣ
		 * @param[in] x ��ͼ��x����
		 * @param[in] y ��ͼ��y����
		 * @return ������Ϣ
		 */
		auto GetMapData(int x, int y) -> MapData;

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
		 * @brief ��ȡ��ͼ���Ӵ�С
		 * @return ��ͼ���Ӵ�С
		 */
		auto GetMapBlockSize() -> float;

		/**
		* @brief ��ȡ��(x, y)��Ӧ��ǰ�����ͼ�ĸ��Ӷ�Ӧ���꣬������ڵ�ͼ��Χ�ڣ��򷵻� (-1, -1)
		* @param[in] x X����
		* @param[in] y Y����
		* @return ���ӵĶ�Ӧ����
		*/
		auto GetWorldMapDataIndex(float x, float y) -> std::pair<int, int>;

		/**
		* @brief ��ȡ��(x, y)��Ӧ��ǰ�����ͼ�ĸ��ӣ�������ڵ�ͼ��Χ�ڣ��򷵻�nullptr
		* @param[in] x X����
		* @param[in] y Y����
		* @return ���ӵ�����
		*/
		auto GetWorldMapData(float x, float y) -> MapData;

		/**
		 * @brief ��ȡ����ĵ�ͼ������
		 * @return ����ĵ�ͼ������
		 */
		auto GetMapName() -> std::string;

		/**
		 * @brief ���Ϸ���λ������
		 * @return (-1, -1)
		 */
		static auto InvalidLocation() -> std::pair<int, int>;
	};

}
