#pragma once
#include <unordered_map>
#include "Render/Texture.h"
#include "Render/Shader.h"

namespace FL
{
	enum class AssetType
	{
		Texture
	};

	struct Assets
	{
	public:
		friend class AssetManager;
		Ref<Texture2D>& GetTexture(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Texture2D>>Textures;
	};

	class AssetManager
	{
	public:
		static void LoadAssetFromFile(const std::string& name, const std::string& filepath, AssetType type);
		static void LoadAssets();
		static Assets& GetAssets() { return s_Assets; }
	private:
		static Assets s_Assets;

	};

}