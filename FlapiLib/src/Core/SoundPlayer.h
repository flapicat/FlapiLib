#pragma once

#include "irrKlang.h"

namespace FL
{
	class SoundPlayer
	{
	public:
		static void Init();
		static void ShutDown();

		static void PlaySound(std::string_view path);
	private:
		static irrklang::ISoundEngine* m_engine;
	};

}
