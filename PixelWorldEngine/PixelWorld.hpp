#pragma once

#include "pch.hpp"

#include "TextureManager.hpp"
#include "DataManager.hpp"
#include "Graphics.hpp"
#include "Events.hpp"

#include "WorldMap.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

#include "PixelObject.hpp"

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

	enum class PixelObjectLayer {
		WorldLayer,
		UILayer,
		Count
	};

	/**
	 * @brief ȫ����Ⱦ���ã�Ŀǰ��ʹ��
	 */
	struct PixelWorldRenderConfig {
		glm::vec4 unused[4];
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
		std::vector<PixelObject*> layerRoots;

		Graphics::RectangleF* renderObject; //������
		Graphics::RectangleF* renderCanvas; //����

		Graphics::StaticSampler* defaultSampler; //Ĭ�ϵĲ�����

		TextureManager* textureManager; //�������

		WorldMap* worldMap; //��ǰʹ�õĵ�ͼ��Ĭ��Ϊnullptr

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

		void RenderPixelObject(glm::mat4x4 baseTransformMatrix, float baseOpacity, PixelObject* pixelObject,
			std::vector<InstanceData>* instanceData, Camera* camera);

		void RenderPixelObjects();
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
		 * @brief ��������ʹ�õ������ͼ�������ͼû��ע����ô���ǽ���Ϊ��ע��
		 * @param[in] worldMap Ҫʹ�õ������ͼ
		 */
		void SetWorldMap(WorldMap* worldMap);

		/**
		 * @brief ������������������ڹ���ͬ����ȾIDʹ�õ�����
		 * @param[in] textureManager ���������
		 */
		void SetTextureManager(TextureManager* textureManager);

		void SetPixelObject(PixelObject* pixelObject, PixelObjectLayer layer = PixelObjectLayer::WorldLayer);

		void CancelPixelObject(std::string name, PixelObjectLayer layer = PixelObjectLayer::WorldLayer);

		/**
		 * @brief ��ȡ��ǰ�������ͼ
		 * @return �����ͼ
		 */
		auto GetWorldMap() -> WorldMap*;

		auto GetPixelObject(std::string name, PixelObjectLayer layer = PixelObjectLayer::WorldLayer) -> PixelObject*;

		/**
		 * @brief ��ȡ�ڵ�ǰ״���µ������ͼ��
		 * @return �洢��ǰ�����ͼ��������ָ��
		 */
		auto GetCurrentWorld() -> Graphics::Texture2D*;
	};
}
