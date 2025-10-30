#include "pch.h"
#include "SoundPlayer.h"

namespace FL
{
	irrklang::ISoundEngine* SoundPlayer::m_engine = nullptr;

	void SoundPlayer::Init()
	{
		m_engine = irrklang::createIrrKlangDevice();
		if (!m_engine) LOG_ERROR("Error while seting up irrclang");
	}

	void SoundPlayer::ShutDown()
	{
		m_engine->drop();
	}

	void SoundPlayer::PlaySound(std::string_view path)
	{
		m_engine->play2D(path.data());
	}
}