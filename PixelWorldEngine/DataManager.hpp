#pragma once

#include "pch.hpp"

#include "Graphics.hpp"

namespace PixelWorldEngine {

	class Application;

	/**
	 * @brief 文件数据类型，用于存储文件数据
	 */
	class FileData {
	private:
		std::vector<byte> data; //数据
		
		friend class DataManager;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] Data 文件数据
		 */
		FileData(byte* Data = nullptr);

		/**
		 * @brief 获取数据大小，按字节
		 * @return 数据大小
		 */
		auto GetSize() -> size_t;

		/**
		 * @brief 获取数据
		 * @return 数据所在内存的头指针
		 */
		auto GetData() -> byte*;

		/**
		 * @brief 获取数据
		 * @return 存放数据的动态数组
		 */
		auto GetVector() -> std::vector<byte>;
	};

	/**
	 * @brief 文件数据管理类型，用于读取输出以及加密数据
	 */
	class DataManager {
	private:
		Graphics::Graphics* graphics;

		std::map<std::string, Graphics::Texture2D*> textures;
	public:
		/**
		 * @breif 构造函数
		 * @param[in] application
		 */
		DataManager(Application* application);

		/**
		 * @brief 析构函数
		 */
		~DataManager();
		
		/**
		 * @brief 读取文件
		 * @param[in] fileName 文件名
		 * @return 返回文件数据
		 */
		auto ReadFile(std::string fileName) -> FileData;

		/**
		 * @brief 读取纹理资源，并且注册，如果文件已经读取那么不会重复读取而是直接返回纹理
		 * @param[in] fileName 纹理文件路径，目前只支持bmp, jpeg, png 
		 * @return 纹理文件，纹理格式为R8G8B8A8
		 */
		auto RegisterTexture(std::string fileName) -> Graphics::Texture2D*;

		/**
		 * @brief 释放我们注册的纹理资源，如果一个纹理不需要使用的话建议释放以节约内存
		 * @param[in] 文件名
		 */
		auto UnRegisterTexture(std::string fileName);

		/**
		 * @brief 写入文件
		 * @param[in] fileName 文件名
		 * @param[in] fileData 要写入的数据
		 */
		void WriteFile(std::string fileName, FileData fileData);
	};

}
