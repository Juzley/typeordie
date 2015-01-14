#ifndef _BOSS_H_
#define _BOSS_H_

#include "EnemyWave.h"
#include "Entity.h"
#include "Phrase.h"
#include "BBox.h"
#include "Game.h"

namespace typing
{
    class ColourRGBA;

    // Charge Boss charges up and fires if the phrase isn't typed before the
    // charge is ready
    class ChargeBoss : public Entity
    {
    public:
        ChargeBoss()
        {
        }

        void OnSpawn();
        void Update();
        void Draw2D();
        void Draw3D();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        bool IsSolid() const
        {
            return (false);
        }

        const BBox GetBounds() const
        {
            return BBox();
        }

        char GetStartChar() const
        {
            return m_phrase.GetStartChar();
        }

        bool StartsWith(const char c) const
        {
            return (GetStartChar() == c);
        }

        bool IsPhraseSingle() const
        {
            return (m_phrase.Length() == 1);
        }

        float GetTypingSpeed() const
        {
            return (m_phrase.GetTypingSpeed());
        }

        bool Unlink() const
        {
            return (m_health == 0);
        }

        unsigned int GetScore() const
        {
            return (CHARGEBOSS_HEALTH);
        }

    private:
        void CalcNextChargeTime()
        {
            m_nextChargeFinishTime = GAME.GetTime() +
                                     CHARGEBOSS_BASE_CHARGE_TIME +
                                     CHARGEBOSS_CHARGE_TIME_SCALE * m_health;
        }

        static const unsigned int   CHARGEBOSS_SCORE = 50;
        static const unsigned int   CHARGEBOSS_HEALTH = 10;
        static const float          CHARGEBOSS_BASE_CHARGE_TIME;
        static const float          CHARGEBOSS_CHARGE_TIME_SCALE;

        unsigned int     m_health;
        juzutil::Vector3 m_origin;
        Phrase           m_phrase;
        bool             m_moving;
        float            m_nextChargeFinishTime;
        ColourRGBA       m_colour;
    };

    class ChargeBossEnemyWave : public EnemyWave
    {
    public:
        void Spawn()
        {
            if (!m_spawned) {
                m_ent.reset(new ChargeBoss);
                GAME.AddEntity(m_ent);
                m_spawned = true;
            }
        }

        void Start()
        {
            m_spawned = false;
        }

        bool IsFinished() const
        {
            return (m_spawned && m_ent->Unlink());
        }

        void OnFinished()
        {
            m_ent.reset();
        }

        float MinProgress() const
        {
            return (0.0001f);
        }

    private:
        EntityPtr m_ent;
        bool      m_spawned;
    };


    // Missile Boss fires missiles at the player
    class MissileBoss : public Entity
    {
    public:
        MissileBoss()
        {
        }

        void OnSpawn();
        void Update();
        void Draw2D();
        void Draw3D();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        bool IsSolid() const
        {
            return (false);
        }

        const BBox GetBounds() const
        {
            return BBox();
        }

        char GetStartChar() const
        {
            return m_phrase.GetStartChar();
        }

        bool StartsWith(const char c) const
        {
            return (GetStartChar() == c);
        }

        bool IsPhraseSingle() const
        {
            return (m_phrase.Length() == 1);
        }

        float GetTypingSpeed() const
        {
            return (m_phrase.GetTypingSpeed());
        }

        bool Unlink() const
        {
            return (m_health == 0);
        }

        unsigned int GetScore() const
        {
            return (MISSILEBOSS_SCORE);
        }

    private:
        static const unsigned int     MISSILEBOSS_SCORE  = 50;
        static const unsigned int     MISSILEBOSS_HEALTH = 10;
        static const unsigned int     MISSILEBOSS_WAVE_MISSILE_COUNT = 8;            

        static const float            MISSILEBOSS_WAVE_GAP;
        static const float            MISSILEBOSS_MISSILE_GAP;   

        unsigned int     m_health;
        juzutil::Vector3 m_origin;
        Phrase           m_phrase;
        float            m_nextMissileFireTime;
        float            m_currentWaveMissilesFired;
        bool             m_moving;
    };

    class MissileBossEnemyWave : public EnemyWave
    {
    public:
        void Spawn()
        {
            if (!m_spawned) {
                m_ent.reset(new MissileBoss);
                GAME.AddEntity(m_ent);
                m_spawned = true;
            }
        }

        void Start()
        {
            m_spawned = false;
        }

        bool IsFinished() const
        {
            return (m_spawned && m_ent->Unlink());
        }

        void OnFinished()
        {
            m_ent.reset();
        }

        float MinProgress() const
        {
            return (0.0001f);
        }

    private:
        EntityPtr m_ent;
        bool      m_spawned;
    };
}

#endif // _BOSS_H_
