#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <string>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace typing
{
    class Sound
    {
    public:
        Sound()
            : m_chunk(NULL), m_channel(-1)
        {
        }

        Sound(Mix_Chunk *chunk)
            : m_chunk(chunk), m_channel(-1)
        {
        }

        void Play(int loops);
        void FadeIn(int loops, int ms);
        void FadeOut(int ms);
        void Stop();

    private:
        Mix_Chunk *m_chunk;
        int        m_channel;
    };

    class SoundManager
    {
    public:
        // Singleton Implementation
        static SoundManager& GetSoundManager();

        // Methods
        Sound Add(const std::string& soundName);
        Sound Get(const std::string& soundName) const;
        void  Play(const std::string& soundName) const;
        void  StopAll() const;

    private:
        // Ctors/Dtors
        SoundManager()
        {
        }

        // Typedefs
        typedef std::map<std::string, Mix_Chunk*> SoundMap;

        // Methods
        Mix_Chunk * GetChunk(const std::string& soundName) const;

        // Members
        SoundMap m_soundMap;

        // Singleton Implementation
        static std::auto_ptr<SoundManager> m_singleton;
    };
    #define SOUNDS SoundManager::GetSoundManager()
}

#endif // _SOUND_MANAGER_H_
