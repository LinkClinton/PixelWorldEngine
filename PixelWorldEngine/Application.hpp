#pragma once

#include "pch.hpp"

#include "KeyCode.hpp"
#include "Events.hpp"

#include "Graphics.hpp"
#include "GraphicsRenderTarget.hpp"

#include "Timer.hpp"
#include "PixelWorld.hpp"

#include "Animation.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ��ҪӦ�ó���
	 */
	class Application {
	private:
		std::string applicationName; //Ӧ�ó��������

		int width = 0; //���ڵĿ��
		int height = 0; //���ڵĸ߶�

		std::string windowName; //���ڵ�����
		std::string iconName; //����ʹ�õ�ͼ���ļ�·��

		bool isWindowCreated = false; //�Ƿ��Ѿ������˴���
		bool isFocused = false; //�����Ƿ��ȡ�˽���

		int mousePositionX; //���λ��Y
		int mousePositionY; //���λ��X

		int mousePositionXRelative; //���λ��X
		int mousePositionYRelative; //���λ��Y

		float deltaTime; //ÿ�θ��¾�����ʱ��

		Timer timer; //��ʱ��
		FpsCounter fpsCounter; //fps������

		Graphics::Graphics* graphics; //ͼ�νӿ�
		Graphics::RenderTarget* renderTarget; //��ȾĿ��

		Graphics::Buffer* cameraBuffer; //��������󻺳�
		Graphics::GraphicsShader* defaultShader; //Ĭ�ϵ���ɫ��
		Graphics::StaticSampler* defaultSampler; //Ĭ�ϵĲ�����

		std::map<std::string, Animator*> animators; //����������

		PixelWorld* pixelWorld; //ʹ�õ�����

#ifdef _WIN32
		HWND hwnd = nullptr; //���ھ��

		std::wstring wideWindowName; //���ڿ��ַ��µ�����
		std::wstring wideIconName; //���ַ��µ�ͼ���ļ�·��

		static LRESULT CALLBACK DefaultWindowProc(HWND hWnd, UINT message,
			WPARAM wParam, LPARAM lParam); //Ĭ����Ϣ������
#endif // _WIN32

	public:

#ifdef _WIN32
		DXGI_SWAP_CHAIN_DESC swapDesc; //������������Ϣ

		IDXGISwapChain* swapChain; //������


#endif //_WIN32

	private:
		/**
		 * @brief ������ƶ���ʱ�򴥷�
		 * @param[in] sender ˭������
		 * @param[in] eventArg ����ƶ�����Ϣ��Ϣ����������λ������ʵλ��
		 */
		void OnMouseMove(void* sender, PixelWorldEngine::Events::MouseMoveEvent* eventArg);

		/**
		* @brief �������ֻ�����ʱ�򴥷�
		* @param[in] sender ˭������
		* @param[in] eventArg �����ֻ�������Ϣ��Ϣ����������λ������ʵλ��
		*/
		void OnMouseWheel(void* sender, PixelWorldEngine::Events::MouseWheelEvent* eventArg);

		/**
		* @brief ��갴����Ϣ����
		* @param[in] sender ˭������
		* @param[in] eventArg ��갴�µ���Ϣ��Ϣ����������λ������ʵλ��
		*/
		void OnMouseClick(void* sender, PixelWorldEngine::Events::MouseClickEvent* eventArg);

		/**
		* @brief ���̰�����Ϣ����
		* @param[in] sender ˭������
		* @param[in] eventArg ���̰�����Ϣ
		*/
		void OnKeyClick(void* sender, PixelWorldEngine::Events::KeyClickEvent* eventArg);

		/**
		 * @brief �����ڴ�С�ı��ʱ�򴥷�
		 * @param[in] sender ˭������
		 * @param[in] eventArg ���ڴ�С�ı���Ϣ
		 */
		void OnSizeChange(void* sender, PixelWorldEngine::Events::SizeChangeEvent* eventArg);

		/**
		 * @brief ���º���
		 * @param[in] sender ˭������
		 */
		void OnUpdate(void* sender);

		/**
		 * @brief ��Ⱦ����
		 * @param[in] sender ˭������
		 */
		void OnRender(void* sender);

#ifdef _WIN32
		/**
		 * @brief Windows��Ϣ������
		 * @param[in] message ��Ϣ
		 */
		void OnProcessMessage(MSG message);
#endif // _WIN32

		/**
		 * @brief �������λ��ת�Ƶ���ǰ����ʹ�õķֱ����µ�λ��
		 * @param[in] x ���λ��X
		 * @param[in] y ���λ��Y
		 * @return ת��������λ��
		 */
		auto ComputeMousePosition(int x, int y) -> std::pair<int, int>;

		/**
		 * @brief ���ݷֱ����Լ����ڴ�С�������ǵĿ��ӷ�Χ
		 * @param[in] windowWidth ���ڿ��
		 * @param[in] windowHeight ���ڸ߶�
		 * @param[in] resolutionWidth �ֱ��ʿ��
		 * @param[in] resolutionHeight �ֱ��ʸ߶�
		 * @return ���ؿ��ӷ�Χ
		 */
		static auto ComputeViewPort(int width, int height, int resolutionWidth, int resolutionHeight) -> RectangleF;

		/**
		* @brief �������λ��ת�Ƶ���ǰ����ʹ�õķֱ����µ�λ��
		* @param[in] ViewPort �ӽǷ�Χ
		* @param[in] resolutionWidth �ֱ��ʿ��
		* @param[in] resolutionHeight �ֱ��ʸ߶�
		* @param[in] x ���λ��X
		* @param[in] y ���λ��Y
		* @return ת��������λ��
		*/
		static auto ComputeMousePosition(RectangleF ViewPort, int resolutionWidth, int resolutionHeight, int x, int y) -> std::pair<int, int>;
		
		friend class Input;
		friend class Animator;
	public:
		PixelWorldEngine::Events::MouseMoveHandlers MouseMove; //����ƶ��¼�
		PixelWorldEngine::Events::MouseClickHandlers MouseClick; //��갴���¼�
		PixelWorldEngine::Events::MouseWheelHandlers MouseWheel; //�������¼�
		PixelWorldEngine::Events::KeyClickEventHandlers KeyClick; //���̰����¼�
		PixelWorldEngine::Events::UpdateEventHandlers Update; //�����¼�
	public:
		/**
		 * @brief ���캯��
		 * @param[in] ApplicationName Ӧ�ó�������
		 */
		Application(std::string ApplicationName);

		/**
		 * @brief ��������
		 */
		~Application();

		/**
		 * @brief �������ڣ�ͬʱ���ֻ�ܴ���һ������
		 * @param[in] WindowName ��������
		 * @param[in] Width ���ڿ��
		 * @param[in] Height ���ڸ߶�
		 * @param[in] IconName ͼ��·��
		 */
		void MakeWindow(std::string WindowName, int Width, int Height, std::string IconName = "");

		/**
		 * @brief ���ô�������
		 * @param[in] WindowName ���ڵ�����
		 * @param[in] Width ���ڵĿ��
		 * @param[in] Height ���ڵĸ߶�
		 */
		void SetWindow(std::string WindowName, int Width, int Height);

		/**
		 * @brief ����ȫ��
		 * @param[in] state �Ƿ�ȫ��
		 */
		void MakeFullScreen(bool state);

		/**
		 * @brief ��ʾ����
		 */
		void ShowWindow();

		/**
		 * @brief ���ش���
		 */
		void HideWindow();

		/**
		 * @brief ��ѭ��
		 */
		void RunLoop();

		/**
		 * @brief ����ʹ�õ�����
		 * @param[in] pixelWorld ����
		 */
		void SetWorld(PixelWorld* pixelWorld);

		/**
		 * @brief ע�ᶯ����������
		 * @param[in] animator ����������
		 */
		void RegisterAnimator(Animator* animator);

		/**
		 * @brief ж�ض�����������
		 * @param[in] animator ����������
		 */
		void UnRegisterAnimator(Animator* animator);

		/**
		 * @brief ж�ض���������
		 * @param[in] name ���������������
		 */
		void UnRegisterAnimator(std::string name);

		/**
		 * @brief ��ȡ���ڿ��
		 * @return ���ڿ��
		 */
		auto GetWindowWidth() -> int;

		/**
		 * @brief ��ȡ���ڸ߶�
		 * @return ���ڿ��
		 */
		auto GetWindowHeight() -> int;

		/**
		 * @brief ��ȡfps
		 * @return fps
		 */
		auto GetFramePerSecond() -> int;

		/**
		 * @brief ��ȡ���µ�ʱ����
		 * @return ʱ�䣬��λ��
		 */
		auto GetDeltaTime() -> float;

		/**
		 * @brief ��ȡͼ�νӿ�
		 * @return ͼ�νӿڵ�ָ��
		 */
		auto GetGraphics() -> Graphics::Graphics*;

		/**
		 * @brief �����Ƿ��ȡ���˽���
		 * @return �Ƿ�õ�����
		 */
		auto IsFocused() -> bool;
	};

}
