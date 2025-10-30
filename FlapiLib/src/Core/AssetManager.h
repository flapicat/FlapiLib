#pragma once
#include <unordered_map>
#include "Render/Texture.h"
#include "Render/Shader.h"

namespace FL
{
	enum class AssetType
	{
		Texture,
		Sound
	};

	struct Assets
	{
	public:
		friend class AssetManager;
		Ref<Texture2D>& GetTexture(const std::string& name);
		std::string_view GetSound(std::string_view sound);
	private:
		std::unordered_map<std::string, Ref<Texture2D>> Textures;
		std::unordered_map<std::string, std::string> SoundsPath;
	};

	class AssetManager
	{
	public:
		static void LoadAssetFromFile(const std::string& name, const std::string& filepath, AssetType type);
		static Assets& GetAssets() { return s_Assets; }
	private:
		static Assets s_Assets;

	};

}