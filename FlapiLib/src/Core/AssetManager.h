#pragma once
#include <unordered_map>
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Font.h"

#include "irrKlang.h"

namespace FL
{
	enum class AssetType
	{
		Font,
		Texture,
		Sound
	};

	struct Assets
	{
	public:
		friend class AssetManager;
		Ref<Font>& GetFont(std::string_view name);
		Ref<Texture2D>& GetTexture(std::string_view name);
		irrklang::ISoundSource* GetSound(std::string_view sound);
	private:
		std::unordered_map<std::string, Ref<Font>> Fonts;
		std::unordered_map<std::string, Ref<Texture2D>> Textures;
		std::unordered_map<std::string, irrklang::ISoundSource*> Sounds;
	};

	class AssetManager
	{
	public:
		static void Init();
		static void ShutDown();
		static void LoadAssetFromFile(AssetType type, const std::string& filepath, const std::string& name);
		static Assets& GetAssets() { return s_Assets; }
	private:
		static Assets s_Assets;
		static FT_Library ft_lib;
	};

}