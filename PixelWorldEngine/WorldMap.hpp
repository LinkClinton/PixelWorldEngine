#pragma once

#include "pch.hpp"
#include "Utility.hpp"
#include "Collider.hpp"

namespace PixelWorldEngine {

#define MAX_RENDER_OBJECT 4
#define DEFAULT_BLOCK_SIZE 32

	/**
	 * @brief ��ͼĳһ�������
	 */
	struct MapData {
		int RenderObjectID[MAX_RENDER_OBJECT]; //��ʾ��һ����Ⱦ������ID�����ǽ��ᰴ��˳��(0 -> MAX_RENDER_OBJECT - 1)������ϵ��µ��ӣ�������ע�������Alphaֵ��Ϊ0�Ļ����ʹ���ʹ��
		
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

		float blockSize; //���Ӵ�С

		std::string mapName; //����ĵ�ͼ������

		std::map<std::pair<int, int>, Collider> colliders; //��ײ��
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
		WorldMap(std::string name, int Width, int Height, float BlockSize = DEFAULT_BLOCK_SIZE, MapData** data = nullptr);

		/**
		 * @brief ���õ�ͼָ��λ��������Ϣ
		 * @param[in] x ��ͼ��x����
		 * @param[in] y ��ͼ��y����
		 * @param[in] data ��ͼ������Ϣ��ע�Ᵽ�����ݵ���������
		 */
		void SetMapData(int x, int y, MapData* data);
	
		/**
		 * @brief ���õ�ͼ���Ӵ�С
		 * @param[in] size ���Ӵ�С
		 */
		void SetBlockSize(float size);

		/**
		 * @brief �Ƿ��ֹ�ƶ���(x, y)����
		 * @param[in] x ���ӵ�X����
		 * @param[in] y ���ӵ�Y����
		 */
		void EnableMovement(bool enable, int x, int y);

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
		 * @brief ��ȡ��ͼ���Ӵ�С
		 * @return ���Ӵ�С
		 */
		auto GetBlockSize() -> float;

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
		auto GetWorldMapData(float x, float y) -> MapData*;

		/**
		 * @brief �����ײ���Ƿ��ڽ�ֹ�ƶ���Χ�ڣ����ཻ
		 * @param[in] ��ײ��
		 * @return �Ƿ��ڷ�Χ��
		 */
		auto Intersect(Collider collider) -> bool;

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
