#pragma once

#include "irrKlang.h"

namespace FL
{
	class SoundPlayer
	{
	public:
		static void Init();
		static void ShutDown();

		static irrklang::ISoundSource* LoadSound(std::string_view filePath);
		static void PlaySound2D(irrklang::ISoundSource* source, float volume = 1.0f, bool loop = false);

	private:
		static irrklang::ISoundEngine* m_engine;
	};

}
