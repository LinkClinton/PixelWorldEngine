#pragma once

#include "pch.hpp"

#include "Transform.hpp"

namespace PixelWorldEngine {

	class PixelObject;

	class PixelObjectCompare {
	public:
		bool operator () (PixelObject* object1, PixelObject* object2)const;
	};
	
	class PixelObject {
	private:
		std::string name;
		
		PixelObject* parent;

		int depth;
	
		std::map<std::string, PixelObject*> childrenNameIndex;
		std::multiset<PixelObject*, PixelObjectCompare> childrenDepthSort;

		friend class PixelWorld;
	public:
		Transform Transform;

		int RenderObjectID;

		float Opacity;
	public:
		PixelObject(std::string name, PixelWorldEngine::Transform transform = PixelWorldEngine::Transform());

		void SetDepth(int depth);

		void SetParent(PixelObject* parent);

		void SetChild(PixelObject* child);

		void CancelChild(std::string name);

		auto GetName() -> std::string;

		auto GetDepth() -> int;

		auto GetParent() -> PixelObject*;

		auto GetChildren(std::string name) -> PixelObject*;
	};

}