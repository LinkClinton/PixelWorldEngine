#pragma once

#include "pch.hpp"

#include "DataManager.hpp"
#include "Graphics.hpp"
#include "Events.hpp"

#include "WorldMap.hpp"
#include "PixelObject.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

#include "UIObject.hpp"

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
		float renderOpacity;
		glm::vec3 unused0;
		glm::vec4 unused1[2];
	};

	/**
	 * @brief ����ĺ�������
	 */
	class PixelWorld {
	private:
		std::string worldName; //��������� 

		int resolutionWidth; //�ֱ��ʿ��
		int resolutionHeight; //�ֱ��ʸ߶�

		PixelWorldRenderConfig renderConfig; //��Ⱦ��������

		Camera* camera; //�����

		float backGroundColor[4]; //������ɫ

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
	
		std::map<std::string, WorldMap*> worldMaps; //�洢����ĵ�ͼ
		std::map<std::string, PixelObject*> pixelObjects; //�洢���������
		std::map<std::string, UIObject*> UIObjects; //�洢UI����
		
		std::multiset<PixelObject*, PixelObjectCompare> pixelObjectLayer;//�洢��ͬ���PixelObject
		std::multiset<UIObject*, UIObjectCompare> UIObjectLayer; //�洢��ͬ���UIObject

		friend class PixelObject;
		friend class Application;
	private:
		/**
		 * @brief �������磬��Ӧ�ó�����µ�ʱ������Ҳ�����
		 * @param[in] deltaTime ������һ�θ��µ�ʱ��
		 */
		void OnUpdate(float deltaTime);

		/**
		 * @brief ��Ⱦ����ĵ�ͼ����ΪGetCurrentWorld���Ӳ���
		 */
		void RenderWorldMap();

		/**
		 * @brief ��Ⱦ�������壬��ΪGetCurrentWorld���Ӳ���
		 */
		void RenderPixelObjects();

		/**
		 * @brief ��ȾUI���壬��ΪGetCurrentWorld���Ӳ���
		 */
		void RenderUIObjects();
	public:
		/**
		 * @brief ���캯��
		 * @param[in] WorldName ���������
		 * @param[in] Application ���������Ӧ�ó���
		 */
		PixelWorld(std::string WorldName, Application* Application);

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
		 * @brief ���ñ�����ɫ
		 * @param[in] red ����
		 * @param[in] green ����
		 * @param[in] blue ����
		 * @param[in] alpha ����
		 */
		void SetBackGroundColor(float red, float green, float blue, float alpha);

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
		void SetWorldMap(std::string worldMapName);

		/**
		 * @brief ��������ʹ�õ������ͼ�������ͼû��ע����ô���ǽ���Ϊ��ע��
		 * @param[in] worldMap Ҫʹ�õ������ͼ
		 */
		void SetWorldMap(WorldMap* worldMap);

		/**
		 * @brief ע��һ����Ⱦ���壬��ע�Ᵽ���������������
		 * @param[in] id ����ע�����Ⱦ�����ID��ע�ⲻ��Ϊ0
		 * @param[in] fileData ��Ⱦ�����������Ϣ��ע�����ݱ��������ݸ�ʽR8G8B8A8
		 */
		void RegisterRenderObjectID(int id, Graphics::Texture2D* texture);

		/**
		 * @brief �ͷ�����ע�������Ⱦ����
		 * @param[in] id ����Ҫ�ͷŵ���Ⱦ�����ID
		 */
		void UnRegisterRenderObjectID(int id);

		/**
		 * @brief ע������ĵ�ͼ
		 * @param[in] worldMap ����ĵ�ͼ
		 */
		void RegisterWorldMap(WorldMap* worldMap);

		/**
		 * @brief ע����������壬��ע������彫����뵽������ȥ���������֮ǰ������һ���������ô�����ȴ��Ƴ�Ȼ���ټ���
		 * @param[in] pixelObject Ҫ��ע�������
		 */
		void RegisterPixelObject(PixelObject* pixelObject);

		/**
		 * @brief ȡ��ע�����壬���彫����������Ƴ�
		 * @param[in] pixelObject Ҫ��ȡ��ע�������
		 */
		void UnRegisterPixelObject(PixelObject* pixelObject);

		/**
		 * @brief ȡ��ע�����壬���彫����������Ƴ�
		 * @param[in] objectName Ҫ��ȡ��ע������������
		 */
		void UnRegisterPixelObject(std::string objectName);

		/**
		 * @brief ע��UI���壬����ע����뵽�������
		 * @param[in] object ����
		 */
		void RegisterUIObject(UIObject* object);

		/**
		 * @brief ȡ��ע������
		 * @param[in] object ����
		 */
		void UnRegisterUIObject(UIObject* object);

		/**
		 * @brief ȡ��ע������
		 * @param[in] object ���������
		 */
		void UnRegisterUIObject(std::string name);

		/**
		 * @brief ��ȡ��ǰ�������ͼ
		 * @return �����ͼ
		 */
		auto GetWorldMap() -> WorldMap*;

		/**
		 * @brief ��ȡ�ڵ�ǰ״���µ������ͼ��
		 * @return �洢��ǰ�����ͼ��������ָ��
		 */
		auto GetCurrentWorld() -> Graphics::Texture2D*;
	};
}
