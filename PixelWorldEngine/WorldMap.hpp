#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

#define MAX_RENDER_OBJECT 4

	/**
	 * @brief ��ͼĳһ�������
	 */
	struct MapData {
		int RenderObjectID[MAX_RENDER_OBJECT]; //��ʾ��һ����Ⱦ������ID�����ǽ��ᰴ��˳��(0 -> MAX_RENDER_OBJECT - 1)������ϵ��µ��ӣ�������ע�������Alphaֵ��Ϊ0�Ļ����ʹ���ʹ��
		int MoveEnable; //�Ƿ����������ƶ�����һ��Ĭ��Ϊfasle

		/**
		 * @brief Ĭ�Ϲ��캯��
		 */
		MapData() = default;
	};

	/**
	 * @brief ��ǰ����ĵ�ͼ��������������ĵ������ԣ����Ͻ�Ϊ��ͼ�� 0,0 
	 */
	class WorldMap {
	private:
		int width; //����Ŀ��
		int height; //����ĸ߶�

		std::string mapName; //����ĵ�ͼ������
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
		WorldMap(std::string name, int Width, int Height, MapData** data = nullptr);

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
		auto GetMapName() -> std::string;

		/**
		 * @brief ���Ϸ���λ������
		 * @return (-1, -1)
		 */
		static auto InvalidLocation() -> std::pair<int, int>;
	};

}
