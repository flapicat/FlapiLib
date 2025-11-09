#include "pch.h"
#include "AssetManager.h"

#include "SoundPlayer.h"

namespace FL
{
	Assets AssetManager::s_Assets;
	FT_Library AssetManager::ft_lib;

	void AssetManager::Init()
	{
		if (FT_Init_FreeType(&ft_lib)) {
			LOG_ERROR("ERROR::FREETYPE: Could not init FreeType");
		}
	}
	
	void AssetManager::ShutDown()
	{
		if (ft_lib)
		{
			FT_Done_FreeType(ft_lib);
		}
	}

	void AssetManager::LoadAssetFromFile(AssetType type, const std::string& filepath ,const std::string& name)
	{
		switch (type)
		{
			case AssetType::Font:
			{
				auto font = Font::Create(ft_lib, filepath);
				s_Assets.Fonts[name] = font;
				LOG_TRACE("Asset loaded: {0} - {1}", name, filepath);
				break;
			}

			case AssetType::Model:
			{
				auto model = Model::Create(filepath);
				s_Assets.Models[name] = model;
				LOG_TRACE("Asset loaded: {0} - {1}", name, filepath);
				break;
			}

			case AssetType::Texture:
			{
				auto tex = Texture2D::Create(filepath);
				s_Assets.Textures[name] = tex;
				if (tex->Loaded == true) LOG_TRACE("Asset loaded: {0} - {1}", name, filepath);
				break;
			}

			case AssetType::Sound:
			{
				auto source = SoundPlayer::LoadSound(filepath);
				s_Assets.Sounds[name] = source;
				LOG_TRACE("Asset loaded: {0} - {1}", name, filepath);
				break;
			}
		}
	}

	Ref<Font>& Assets::GetFont(std::string_view name)
	{
		auto it = Fonts.find(name.data());
		if (it != Fonts.end())
		{
			return it->second;
		}

		LOG_WARN("Trying to get Font that don't exist: {0}", name);
		static Ref<Font> nullFont = nullptr;
		return nullFont;
	}

	Ref<FL::Model>& Assets::GetModel(std::string_view name)
	{
		auto it = Models.find(name.data());
		if (it != Models.end())
		{
			return it->second;
		}

		LOG_WARN("Trying to get texture that don't exist: {0}", name);
		static Ref<Model> nullModel = nullptr;
		return nullModel;
	}

	Ref<Texture2D>& Assets::GetTexture(std::string_view name)
	{
		auto it = Textures.find(name.data());
		if (it != Textures.end())
		{
			return it->second;
		}

		LOG_WARN("Trying to get texture that don't exist: {0}", name);
		static Ref<Texture2D> nullTexture = nullptr;
		return nullTexture;
	}

	irrklang::ISoundSource* Assets::GetSound(std::string_view sound)
	{
		auto it = Sounds.find(std::string(sound));
		if (it != Sounds.end())
		{
			return it->second;
		}

		LOG_WARN("Trying to get Sound that don't exist: {0}", sound);
		return nullptr;
	}
}