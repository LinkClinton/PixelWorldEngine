#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 子纹理，内置使用
	 */
	struct SubTexture {
		glm::vec2 startPosition; //纹理坐标的开始位置
		glm::vec2 endPosition; //纹理坐标的结束位置
		
		int startPositionX; //纹理开始的X位置
		int startPositionY; //纹理开始的Y位置
		int width; //纹理的宽度
		int height; //纹理的高度

		glm::mat4x4 transform; //纹理坐标变换矩阵

		/**
		 * @brief 构建纹理坐标
		 * @param[in] maxWidth 最大的宽度
		 * @param[in] maxHeight 最大的高度
		 */
		void MakeTexCoordPosition(int maxWidth, int maxHeight);

		/**
		 * @brief 是否相交
		 * @param[in] texture1 第一个子纹理
		 * @param[in] texture2 第二个子纹理
		 */
		static bool IsEnter(SubTexture texture1, SubTexture texture2);
	};

	/**
	 * @brief 合并纹理类型，用于将纹理合并，注意纹理类型必须是R8G8B8A8
	 */
	class MergeTexture {
	private:
		Graphics::Graphics* graphics; //..
		
		Graphics::Texture2D* finalTexture; //最后合并的纹理

		std::map<int, SubTexture> subTextures; //子纹理
	public:
		/**
		 * @brief 构造函数
		 * @param[in] width 宽度
		 * @param[in] height 高度
		 */
		MergeTexture(Application* application, int width, int height);

		/**
		 * @brief 析构函数
		 */
		~MergeTexture();

		/**
		 * @brief 添加纹理，将纹理复制到MergeTexture中去，注意如何复制到的位置和其他纹理有交集，那么就不会复制
		 * @param[in] id 纹理的渲染ID
		 * @param[in] positionX 纹理在MergeTexture的起始X坐标
		 * @param[in] positionY 纹理在MergeTexture的起始Y坐标
		 * @param[in] texture 要复制的纹理
		 */
		void AddTexture(int id, int positionX, int positionY, Graphics::Texture2D* texture);

		/**
		 * @brief 移除纹理，注意并不会将这个纹理的区域的数据清零，但是那部分区域将会变成可用区域
		 * @param[in] id 纹理的ID
		 */
		void RemoveTexture(int id);

		/**
		 * @brief 获取纹理坐标的变换矩阵，即将对应原本纹理的纹理坐标变换到MergeTexture的纹理坐标系中去
		 * @param[in] id 纹理的渲染ID
		 * @return 变换矩阵
		 */
		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		/**
		 * @brief 获取MergeTexture的数据
		 * @return 纹理数据
		 */
		auto GetFinalTexture() -> Graphics::Texture2D*;

		/**
		 * @brief 检测这个ID是否已经被使用了
		 * @return 是否被使用
		 */
		auto IsExistID(int id) -> bool;
	};

}