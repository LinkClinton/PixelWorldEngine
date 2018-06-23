#pragma once

#include "pch.hpp"

#include "DataManager.hpp"
#include "Graphics.hpp"
#include "Events.hpp"

#include "WorldMap.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

namespace PixelWorldEngine {

	/**
	 * ����������;��ͬ�Ļ��������ɫ���Լ������ID
	 */
	enum class BufferIndex : int {
		CameraBuffer, //��������󻺳�
		TransformBuffer, //�任���󻺳�
		RenderConfig, //��¼��ǰ��Ⱦ��ʱ�������
		Count
	};

	struct PixelWorldRenderConfig {
		int currentRenderObjectID[MAX_RENDER_OBJECT];
		glm::vec4 unused[3];
	};

	/**
	 * @brief ����ĺ�������
	 */
	class PixelWorld {
	private:
		std::wstring worldName; //��������� 

		int resolutionWidth; //�ֱ��ʿ��
		int resolutionHeight; //�ֱ��ʸ߶�

		int renderObjectSize; //ÿһ����ͼ��Ĵ�С��Ĭ��ֵΪ32

		PixelWorldRenderConfig renderConfig; //��Ⱦ��������

		Camera* camera; //�����

		Graphics::Graphics* graphics; //...

		Graphics::Texture2D* renderBuffer; //�洢���类��Ⱦ���������Ϣ
		Graphics::RenderTarget* renderTarget; //..

		Graphics::GraphicsShader* defaultShader; //Ĭ����ɫ��
		Graphics::GraphicsShader* shader; //ʹ�õ���ɫ��

		std::vector<Graphics::Buffer*> buffers; //��������

		Graphics::RectangleF* renderObject; //������
		Graphics::RectangleF* renderCanvas; //����

		Graphics::StaticSampler* defaultSampler; //Ĭ�ϵĲ�����

		WorldMap* worldMap; //��ǰʹ�õĵ�ͼ��Ĭ��Ϊ��

		std::map<int, Graphics::Texture2D*> renderObjectIDGroup; //���ڴ洢������ͬ��ID��Ӧ��ͬ������
	
		std::map<std::wstring, WorldMap*> worldMaps; //�洢����ĵ�ͼ

		friend class Application;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] WorldName ���������
		 * @param[in] Application ���������Ӧ�ó���
		 */
		PixelWorld(std::wstring WorldName, Application* Application);

		/**
		 * @brief ��������
		 */
		~PixelWorld();

		/**
		 * @brief ����ʹ�õķֱ��ʴ�С
		 * @param[in] width �ֱ��ʵĿ��
		 * @param[in] height �ֱ��ʵĸ߶�
		 */
		void SetResolution(int width, int height);

		/**
		 * @brief ��������ʹ�õ������
		 * @param[in] camera �����
		 */
		void SetCamera(Camera* camera);

		/**
		 * @brief ��������ʹ�õ���ɫ��
		 * @param[in] shader ��ɫ����ָ�룬ע���뱣��������������
		 */
		void SetShader(Graphics::GraphicsShader* shader);

		/**
		 * @brief ��������ʹ�õ���ɫ��ΪĬ����ɫ������ʼֵ����Ĭ����ɫ��
		 */
		void SetShader();

		/**
		 * @brief ��������ʹ�õ������ͼ��ʹ�õĵ�ͼ����Ҫ��ע��
		 * @param[in] worldMapName ��ͼ������
		 */
		void SetWorldMap(std::wstring worldMapName);

		/**
		 * @brief ��������ʹ�õ������ͼ�������ͼû��ע����ô���ǽ���Ϊ��ע��
		 * @param[in] worldMap Ҫʹ�õ������ͼ
		 */
		void SetWorldMap(WorldMap* worldMap);

		/**
		 * @brief ������Ⱦ����Ĵ�С
		 * @param[in] ��Ⱦ����Ĵ�С����λ������
		 */
		void SetRenderObjectSize(int size);

		/**
		 * @brief ע��һ����Ⱦ���壬��ע�Ᵽ���������������
		 * @param[in] id ����ע�����Ⱦ�����ID��ע�ⲻ��Ϊ0
		 * @param[in] fileData ��Ⱦ�����������Ϣ��ע�����ݱ��������ݸ�ʽR8G8B8A8��Ȼ��ͼ�εĳ������һ��
		 */
		void RegisterRenderObjectID(int id, Graphics::Texture2D* texture);

		/**
		 * @brief �ͷ�����ע�������Ⱦ����
		 * @param[in] id ����Ҫ�ͷŵ���Ⱦ�����ID
		 */
		void UnRegisterRenderObjectID(int id);

		/**
		 * @brief ע������ĵ�ͼ
		 * @oaram[in] worldMap ����ĵ�ͼ
		 */
		void RegisterWorldMap(WorldMap* worldMap);

		/**
		 * @brief ��ȡ��Ⱦ����Ĵ�С
		 * @return ��Ⱦ����Ĵ�С
		 */
		auto GetRenderObjectSize() -> int;

		/**
		 * @brief ��ȡ�ڵ�ǰ״���µ������ͼ��
		 * @return �洢��ǰ�����ͼ��������ָ��
		 */
		auto GetCurrentWorld() -> Graphics::Texture2D*;
	};
}
