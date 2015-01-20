#ifndef _GAME_H_
#define _GAME_H_

#include <memory>
#include <list>
#include <string>
#include <vector>
#include <SDL/SDL_mixer.h>
#include "Entity.h"
#include "Timer.h"
#include "Player.h"
#include "Vector.h"
#include "Colour.h"
#include "PhraseBook.h"
#include "EnemyWave.h"
#include "Powerup.h"
#include "Effect.h"
#include "Camera.h"
#include "Utils.h"
#include "SoundManager.h"

namespace typing
{
    class Game
    {
    public:
        // Consts/Enums
        static const float GAME_SCREEN_TOP;
        static const float GAME_SCREEN_BOTTOM;
        static const float GAME_SCREEN_LEFT;
        static const float GAME_SCREEN_RIGHT;

        // Methods
        void Init();
        void Update();
        void Draw();
        void OnType(const SDL_keysym sym);
        void StartNewGame();
        void Damage();
        void EndGame(float pause = 0);

        void AddEntity(const EntityPtr& ent)
        {
            ent->OnSpawn();
            m_entities.push_back(ent);
        }

        void AddEffect(const EffectPtr& effect)
        {
            effect->OnSpawn();
            m_effects.push_back(effect);
        }

        void AddEffect2d(const EffectPtr& effect)
        {
            effect->OnSpawn();
            m_effects2d.push_back(effect);
        }

        void Pause(bool pause)
        {
            m_timer.Pause(pause);
            m_paused = pause;
        }

        bool IsPaused() const
        {
            return m_paused;
        }

        void Activate(bool activate)
        {
            m_active = activate;
        }

        bool IsActive() const
        {
            return m_active;
        }

        const juzutil::Vector3& GetPlayerOrigin() const
        {
            return m_player.GetOrigin();
        }

        float GetTime() const
        {
            return m_timer.GetTime();
        }

        float GetFrameTime() const
        {
            return m_timer.GetFrameTime();
        }

        unsigned int GetLevel() const
        {
            return m_level;
        }

        const std::string& GetPhrase(PhraseBook::PhraseLength len)
        {
            return m_phrases.GetPhrase(len);
        }

        const std::string GetComboPhrase(unsigned int             words,
                                         PhraseBook::PhraseLength len)
        {
            return m_phrases.GetComboPhrase(words, len);
        }

        void MakeCharAvail(char c)
        {
            m_phrases.MakeCharAvail(c);
        }

        unsigned int GetScore() const
        {
            return m_score;
        }

        unsigned int GetMaxStreak() const
        {
            return m_maxStreak;
        }

        const Camera GetCam() const
        {
            return m_camera;
        }

        void AddExtraLife()
        {
            m_lives++;
        }

        // Singleton implementation
        static Game& GetGame();

    private:
        // Constants/Enums
        static const unsigned int GAME_START_LIVES        = 3;
        static const unsigned int END_GAME_SCREEN_PAUSE   = 1;
        static const unsigned int MAX_PROGRESS            = 5000;
        static const float        FINAL_DEATH_PAUSE;
        static const float        MIN_POWERUP_SPAWN_TIME;
        static const float        MAX_POWERUP_SPAWN_TIME;
        static const float        ORTHO_WIDTH;
        static const float        ORTHO_HEIGHT;
        static const std::string  HUD_FONT;
        static const std::string  ENDGAME_FONT;
        static const std::string  MISS_SOUND;
        static const std::string  TARGET_SOUND;
        static const std::string  GAME_MUSIC;

        // Ctors/Dtors
        // These are private in order to protect the singleton implementation.
        Game();
        Game(const Game& g);


        // Typedefs
        typedef std::list<EntityPtr>      EntityList;
        typedef std::list<EffectPtr>      EffectList;
        typedef std::vector<EnemyWavePtr> WaveVec;


        // Methods
        void                   SpawnEnemies();
        void                   SpawnPowerups();
        void                   DrawHud();
        void                   DrawBackground();
        void                   DrawEndScreen();
        void                   PhraseFinished(EntityPtr &ent);

        bool IsAlive() const
        {
            return (m_lives > 0);
        }

        bool HasGameEnded() const
        {
            return (m_gameEndTime != 0.0f && GetTime() >= m_gameEndTime);
        }

        // Members
        Camera                       m_camera;
        EntityList                   m_entities;
        EntityWeakPtr                m_targetEnt;
        Player                       m_player;
        EffectList                   m_effects;
        EffectList                   m_effects2d;
        bool                         m_streakValid;
        bool                         m_active;
        bool                         m_paused;
        bool                         m_invincible;
        Timer                        m_timer;
        unsigned int                 m_lives;
        unsigned int                 m_score;
        unsigned int                 m_streak;
        float                        m_gameEndTime;
        PhraseBook                   m_phrases;
        Mix_Music                   *m_music;
        PowerupFactory               m_powerups;
        float                        m_nextPowerupTime;
        unsigned int                 m_level;
        float                        m_nextLevelTime;
        float                        m_damageTime;

        // Enemy spawn variables
        RandomEnemyWaveFactory       m_waveCreator;
        WaveVec                      m_activeWaves;
        float                        m_nextWaveTime;

        CyclicEnemyWaveFactory       m_bossWaveCreator;
        WaveVec                      m_bossWaves;
        WaveVec::size_type           m_nextBossWaveIndex;
        bool                         m_bossWavePending;
        float                        m_bossWaveStartTime;
        bool                         m_bossWaveActive;

        // Stats
        unsigned int                 m_hits;
        unsigned int                 m_misses;
        unsigned int                 m_excellents;
        unsigned int                 m_goods;
        unsigned int                 m_oks;
        unsigned int                 m_poors;
        unsigned int                 m_bads;
        unsigned int                 m_usedLives;
        unsigned int                 m_maxStreak;

        // Singleton implementation
        static std::auto_ptr<Game> m_singleton;
    };

    #define GAME Game::GetGame()
}

#endif // _GAME_H_
