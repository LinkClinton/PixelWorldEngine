#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief 采样地址模式
		 */
		enum class TextureAddressMode: int {
			Wrap = 1,
			Mirror = 2,
			Clamp = 3,
			Border = 4,
			MirrorOnce = 5
		};

		/**
		 * @brief 采样过滤
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
		 * @brief 纹理采样器
		 */
		class StaticSampler {
		private:
			Graphics* graphics; //..

			TextureAddressMode AddressU; //U地址采样模式
			TextureAddressMode AddressV; //V地址采样模式
			TextureAddressMode AddressW; //W地址采样模式

			TextureFilter Filter; //过滤器

#ifdef _WIN32
		public:
			ID3D11SamplerState* sampler; //Direct3D 接口
#endif // _WIN32

		public:
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] addressU U地址采样模式
			 * @param[in] addressV V地址采样模式
			 * @param[in] addressW W地址采样模式
			 * @param[in] filter 纹理过滤
			 */
			StaticSampler(Graphics* graphics, TextureAddressMode addressU, TextureAddressMode addressV,
				TextureAddressMode addressW, TextureFilter filter = TextureFilter::MinMagMipLinear);
			
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] addressUVM UVM地址采样模式
			 * @param[in] filter 纹理过滤
			 */
			StaticSampler(Graphics* graphics, TextureAddressMode addressUVW = TextureAddressMode::Clamp,
				TextureFilter filter = TextureFilter::MinMagMipLinear);

			/**
			 * @brief 析构函数
			 */
			~StaticSampler();
		};

	}

}