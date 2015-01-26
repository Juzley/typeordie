#include "SoundManager.h"
#include "Exceptions.h"

namespace typing
{
    void Sound::Play(int loops)
    {
        m_channel = Mix_PlayChannel(-1, m_chunk, loops);
        printf("%d\n",m_channel);
    }

    void Sound::FadeIn(int loops, int ms)
    {
        m_channel = Mix_FadeInChannel(-1, m_chunk, loops, ms);
    }

    void Sound::FadeOut(int ms)
    {
        if (m_channel != -1)
        {
            (void)Mix_FadeOutChannel(m_channel, ms);
        }
    }

    void Sound::Stop()
    {
        if (m_channel != -1)
        {
            Mix_HaltChannel(m_channel);
        }
    }

    std::auto_ptr<SoundManager> SoundManager::m_singleton(new SoundManager);
    SoundManager& SoundManager::GetSoundManager()
    {
        return *(m_singleton.get());
    }

    Sound SoundManager::Add(const std::string& soundName)
    {
        if(m_soundMap.find(soundName) == m_soundMap.end())
        {
            Mix_Chunk * sound = Mix_LoadWAV(soundName.c_str());
            if (!sound)
            {
                throw FileNotFoundException(soundName);
            }
            else
            {
                m_soundMap[soundName] = sound;
            }

            return Sound(sound);
        } else {
            return Get(soundName);
        }
    }

    Mix_Chunk * SoundManager::GetChunk(const std::string& soundName) const
    {
        SoundMap::const_iterator iter = m_soundMap.find(soundName);
        if (iter == m_soundMap.end())
        {
            throw MediaNotLoadedException(soundName);
        }
        else
        {
            return iter->second;
        }
    }

    Sound SoundManager::Get(const std::string& soundName) const
    {
        return Sound(GetChunk(soundName));
    }

    // SoundManager::Play can be used for 'fire and forget' sound playing.
    // The underlying Sound is not returned so there is no further control of the sound
    // after it has been started
    void SoundManager::Play(const std::string& soundName) const
    {
        Get(soundName).Play(0);
    }

    void SoundManager::StopAll() const
    {
        Mix_HaltChannel(-1);
    }
}
