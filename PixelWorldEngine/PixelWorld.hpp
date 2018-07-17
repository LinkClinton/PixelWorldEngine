#pragma once

#include "pch.hpp"

#include "TextureManager.hpp"
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

#define LOW_MAX_INSTANCE_DATA 100

	/**
	 * @brief ����������;��ͬ�Ļ��������ɫ���Լ�����ı��
	 */
	enum class BufferIndex : int {
		CameraBuffer, //��������󻺳�
		RenderConfig, //��¼��ǰ��Ⱦ��ʱ�������
		LowInstanceData, //���ڵͰ汾��ʵ������
		Count
	};

	/**
	 * @brief ����������ͬ��;�Ļ������������
	 */
	enum class BufferArrayIndex :int {
		WorldMapInstanceData, //��Ⱦ��ͼʵ������
		PixelObjectInstanceData, //��ȾPixelObjectʵ������
		UIObjectInstanceData, //��ȾUIObjectʵ������
		Count
	};

	/**
	 * @brief ȫ����Ⱦ���ã�Ŀǰ��ʹ��
	 */
	struct PixelWorldRenderConfig {
		Graphics::PixelFormat mergeTextureFormat[MAX_MERGETEXTURE_COUNT]; //ʹ�õĺϲ�����ĸ�ʽ
		glm::vec4 unused[3];
	};

	 /**
	 * @brief ʵ������
	 */
	struct InstanceData {
		int setting[4]; //���ã���һ��Ԫ����ʹ�õ���Ⱦ��ţ��ڶ���Ԫ����ʹ�õ�����ҳ���
		glm::mat4x4 worldTransform; //����任����
		glm::mat4x4 texcoordTransform; //����任����
		glm::vec4 renderCoor; //��Ⱦ����ɫ�����ĸ������ǲ�͸����

		InstanceData();
	};

	/**
	 * @brief ����ĺ�������
	 */
	class PixelWorld {
	private:
		std::string worldName; //��������� 

		int resolutionWidth; //�ֱ��ʿ��
		int resolutionHeight; //�ֱ��ʸ߶�

		float ssaaLevel = 1.0f; //SSAA�ȼ�

		PixelWorldRenderConfig renderConfig; //��Ⱦ��������

		Camera* camera; //�����
		Camera UICamera; //UI�����

		float backGroundColor[4]; //������ɫ��Ĭ��Ϊ(0, 0, 0, 1)

		Graphics::Graphics* graphics; //...

		Graphics::Texture2D* renderBuffer; //�洢���类��Ⱦ���������Ϣ
		Graphics::RenderTarget* renderTarget; //..

		Graphics::GraphicsShader* defaultShader; //Ĭ����ɫ��
		Graphics::GraphicsShader* shader; //ʹ�õ���ɫ��

		std::vector<Graphics::Buffer*> buffers; //��������
		std::vector<Graphics::BufferArray*> bufferArrays; //��������

		Graphics::RectangleF* renderObject; //������
		Graphics::RectangleF* renderCanvas; //����

		Graphics::StaticSampler* defaultSampler; //Ĭ�ϵĲ�����

		TextureManager* textureManager; //�������

		WorldMap* worldMap; //��ǰʹ�õĵ�ͼ��Ĭ��Ϊnullptr

		std::map<std::string, WorldMap*> worldMaps; //�洢����ĵ�ͼ
		std::map<std::string, PixelObject*> pixelObjects; //�洢���������
		std::map<std::string, UIObject*> UIObjects; //�洢UI����
		
		std::multiset<PixelObject*, PixelObjectCompare> pixelObjectLayer;//�洢��ͬ���PixelObject
		std::multiset<UIObject*, UIObjectCompare> UIObjectLayer; //�洢��ͬ���UIObject

		UIObject* focusUIObject; //�õ������UIObject��Ĭ��Ϊnullptr

		friend class UIObject;
		friend class PixelObject;
		friend class Application;
	private:
		/**
		* @brief ������ƶ���ʱ�򴥷�
		* @param[in] sender ˭��������Ϣ
		* @param[in] eventArg ��Ϣ��Ϣ
		*/
		void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg);

		/**
		* @brief ����갴�µ�ʱ�򴥷�
		* @param[in] sender ˭��������Ϣ
		* @param[in] eventArg ��Ϣ��Ϣ
		*/
		void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg);

		/**
		* @brief ����껬�ֹ�����ʱ�򴥷�
		* @param[in] sender ˭��������Ϣ
		* @param[in] eventArg ��Ϣ��Ϣ
		*/
		void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg);

		/**
		* @brief �����̰�������ȥ��ʱ�򴥷�
		* @param[in] sender ˭��������Ϣ
		* @param[in] eventArg ��Ϣ��Ϣ
		*/
		void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg);

		/**
		 * @brief �������磬��Ӧ�ó�����µ�ʱ������Ҳ�����
		 * @param[in] deltaTime ������һ�θ��µ�ʱ��
		 */
		void OnUpdate(float deltaTime);

		/**
		 * @brief ���ڵͰ汾����Ⱦ
		 * @param[in] instanceData ʵ������
		 */
		void LowDrawObject(std::vector<InstanceData>* instanceData);

		/**
		 * @brief ���ڸ߰汾����Ⱦ
		 * @param[in] instanceData ʵ������
		 * @param[in] arrayIndex ����
		 */
		void HighDrawObject(std::vector<InstanceData>* instanceData, BufferArrayIndex arrayIndex);

		/**
		 * @brief ��Ⱦ����ĵ�ͼ����ΪGetCurrentWorld���Ӳ���
		 */
		void RenderWorldMap();

		/**
		 * @brief ��Ⱦ�������壬��ΪGetCurrentWorld���Ӳ���
		 */
		void RenderPixelObjects();

		/**
		 * @brief ��ȾUI����
		 * @param[in] baseTransform ���׵�λ��
		 * @param[in] baseOpacity ���׵Ĳ�͸����
		 * @param[in] object ����
		 * @param[in] instanceData ������ʵ������
		 */
		void RenderUIObject(glm::mat4x4 baseTransform, float baseOpacity, UIObject* object, std::vector<InstanceData>* instanceData);

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
		 * @param[in] ssaa ���û��屶�������ڿ���ݣ�Ĭ��Ϊ1.0f
		 */
		void SetResolution(int width, int height, float ssaa = 1.0f);

		/**
		 * @brief ���ñ�����ɫ
		 * @param[in] red ������Ĭ��Ϊ0
		 * @param[in] green ������Ĭ��Ϊ0
		 * @param[in] blue ������Ĭ��Ϊ0
		 * @param[in] alpha ������Ĭ��Ϊ1
		 */
		void SetBackGroundColor(float red, float green, float blue, float alpha);

		/**
		 * @brief ��������ʹ�õ������
		 * @param[in] camera �������Ĭ��Ϊnullptr
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
		 * @brief ������������������ڹ���ͬ����ȾIDʹ�õ�����
		 * @param[in] textureManager ���������
		 */
		void SetTextureManager(TextureManager* textureManager);

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
		 * @brief ע��UI���壬��ע������彫����뵽������ȥ���������֮ǰ������һ���������ô�����ȴ��Ƴ�Ȼ���ټ���
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
