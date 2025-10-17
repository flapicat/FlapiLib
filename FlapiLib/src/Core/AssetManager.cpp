#include "pch.h"
#include "AssetManager.h"

namespace FL
{
	Assets AssetManager::s_Assets;

	void AssetManager::LoadAssetFromFile(const std::string& name, const std::string& filepath, AssetType type)
	{
		switch (type)
		{
		case AssetType::Texture:
			auto tex = Texture2D::Create(filepath);
			s_Assets.Textures[name] = tex;
			if(tex->Loaded == true) LOG_TRACE("Asset loaded: {0} - {1}", name, filepath);
			break;
		}
	}

	void AssetManager::LoadAssets()
	{
		LoadAssetFromFile("container", "Assets/Textures/container.png", AssetType::Texture);
	}

	Ref<Texture2D>& Assets::GetTexture(const std::string& name)
	{
		auto it = Textures.find(name);
		if (it != Textures.end())
		{
			return it->second;
		}

		LOG_WARN("Trying to get texture that don't exist: {0}", name);
		static Ref<Texture2D> nullTexture = nullptr;
		return nullTexture;
	}
}