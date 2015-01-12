#ifndef _BOSS_H_
#define _BOSS_H_

#include "EnemyWave.h"
#include "Entity.h"
#include "Phrase.h"
#include "BBox.h"

namespace typing
{
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
        static const unsigned int   CHARGEBOSS_SCORE = 50;
        static const unsigned int   CHARGEBOSS_HEALTH = 10;

        unsigned int     m_health;
        juzutil::Vector3 m_origin;
        Phrase           m_phrase;
    };

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

        static const juzutil::Vector3 MISSILEBOSS_START_ORIGIN;
        static const juzutil::Vector3 MISSILEBOSS_DEST_ORIGIN;
        static const float            MISSILEBOSS_DEST_EPSILON;
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
        void Spawn();

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
