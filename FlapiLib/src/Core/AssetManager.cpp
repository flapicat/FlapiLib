#include "pch.h"
#include "AssetManager.h"

#include "SoundPlayer.h"

namespace FL
{
	Assets AssetManager::s_Assets;

	void AssetManager::LoadAssetFromFile(const std::string& name, const std::string& filepath, AssetType type)
	{
		switch (type)
		{
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