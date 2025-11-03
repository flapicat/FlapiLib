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
		if (m_engine)
			m_engine->drop();
	}


    irrklang::ISoundSource* SoundPlayer::LoadSound(std::string_view filePath)
    {
        return m_engine->addSoundSourceFromFile(filePath.data());
    }

    void SoundPlayer::PlaySound2D(irrklang::ISoundSource* source, float volume, bool loop)
    {
        if (!source) return;
        irrklang::ISound* sound = m_engine->play2D(source, loop, false, true);
        if (sound)
        {
            sound->setVolume(volume);
            sound->drop();
        }
    }
}