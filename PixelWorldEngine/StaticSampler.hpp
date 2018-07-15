#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief ������ַģʽ
		 */
		enum class TextureAddressMode: int {
			Wrap = 1,
			Mirror = 2,
			Clamp = 3,
			Border = 4,
			MirrorOnce = 5
		};

		/**
		 * @brief ��������
		 */
		enum class TextureFilter: int {
			MinMagMipPoint = 0,
			MinMagPointMipLinear = 1,
			MinPointMagLinearMipPoint = 4,
			MinPointMagMipLinear = 5,
			MinLinearMagMipPoint = 16,
			MinLinearMagPointMipLinear = 17,
			MinMagLinearMipPoint = 20,
			MinMagMipLinear = 21,
			Anisotropic = 85,
			ComparisonMinMagMipPoint = 128,
			ComparisonMinMagPointMipLinear = 129,
			ComparisonMinPointMagLinearMipPoint = 132,
			ComparisonMinPointMagMipLinear = 133,
			ComparisonMinLinearMagMipPoint = 144,
			ComparisonMinLinearMagPointMipLinear = 145,
			ComparisonMinMagLinearMipPoint = 148,
			ComparisonMinMagMipLinear = 149,
			ComparisonAnisotropic = 213,
			MinimumMinMagMipPoint = 256,
			MinimumMinMagPointMipLinear = 257,
			MinimumMinPointMagLinearMipPoint = 260,
			MinimumMinPointMagMipLinear = 261,
			MinimumMinLinearMagMipPoint = 272,
			MinimumMinLinearMagPointMipLinear = 273,
			MinimumMinMagLinearMipPoint = 276,
			MinimumMinMagMipLinear = 277,
			MinimumAnisotropic = 341,
			MaximumMinMagMipPoint = 384,
			MaximumMinMagPointMipLinear = 385,
			MaximumMinPointMagLinearMipPoint = 388,
			MaximumMinPointMagMipLinear = 389,
			MaximumMinLinearMagMipPoint = 400,
			MaximumMinLinearMagPointMipLinear = 401,
			MaximumMinMagLinearMipPoint = 404,
			MaximumMinMagMipLinear = 405,
			MaximumAnisotropic = 469
		};

		/**
		 * @brief ���������
		 */
		class StaticSampler {
		private:
			Graphics* graphics; //..

			TextureAddressMode AddressU; //U��ַ����ģʽ
			TextureAddressMode AddressV; //V��ַ����ģʽ
			TextureAddressMode AddressW; //W��ַ����ģʽ

			TextureFilter Filter; //������

#ifdef _WIN32
		public:
			ID3D11SamplerState* sampler; //Direct3D �ӿ�
#endif // _WIN32

		public:
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] addressU U��ַ����ģʽ
			 * @param[in] addressV V��ַ����ģʽ
			 * @param[in] addressW W��ַ����ģʽ
			 * @param[in] filter �������
			 */
			StaticSampler(Graphics* graphics, TextureAddressMode addressU, TextureAddressMode addressV,
				TextureAddressMode addressW, TextureFilter filter = TextureFilter::MinMagMipLinear);
			
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] addressUVM UVM��ַ����ģʽ
			 * @param[in] filter �������
			 */
			StaticSampler(Graphics* graphics, TextureAddressMode addressUVW = TextureAddressMode::Clamp,
				TextureFilter filter = TextureFilter::MinMagMipLinear);

			/**
			 * @brief ��������
			 */
			~StaticSampler();
		};

	}

}