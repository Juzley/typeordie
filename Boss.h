#ifndef _BOSS_H_
#define _BOSS_H_

#include "EnemyWave.h"
#include "Entity.h"
#include "Phrase.h"
#include "BBox.h"
#include "Game.h"
#include "SoundManager.h"

namespace typing
{
    class ColourRGBA;

    // The Knockback boss keeps moving towards the player but gets knocked
    // back by the player completing the phrase.
    class KnockbackBoss : public Entity
    {
    public:
        KnockbackBoss(float speed = KNOCKBACKBOSS_ADVANCE_SPEED)
            : m_speed(speed)
        {
        }

        void OnSpawn();
        void Update();
        virtual void Draw2D();
        void Draw3D();
        void OnType(char c, bool *hit, bool *phraseFinished);
        void OnCollide();

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        bool IsSolid() const
        {
            return (true);
        }

        const BBox GetBounds() const
        {
            // TODO: Proper hitbox
            return BBox() + m_origin;
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
            return m_phrase.GetTypingSpeed();
        }

        bool Unlink() const
        {
            return (m_health == 0);
        }

        unsigned int GetScore() const
        {
            return KNOCKBACKBOSS_SCORE;
        }

    protected:
        static const unsigned int KNOCKBACKBOSS_SCORE = 50;
        static const unsigned int KNOCKBACKBOSS_HEALTH = 8;
        static const float        KNOCKBACKBOSS_ADVANCE_SPEED;

        float            m_speed;
        unsigned int     m_health;
        juzutil::Vector3 m_origin;
        Phrase           m_phrase;
        bool             m_moving;
    };


    // Backwards knockback boss is like the knockback boss but the phrase is
    // backwards
    class BackwardsKnockbackBoss : public KnockbackBoss
    {
        public:
            BackwardsKnockbackBoss(
                        float speed = BACKWARDSKNOCKBACKBOSS_ADVANCE_SPEED)
                : KnockbackBoss(speed)
            {
            }

            void Draw2D();

        private:
            static const float BACKWARDSKNOCKBACKBOSS_ADVANCE_SPEED;
    };


    // Charge Boss charges up and fires if the phrase isn't typed before the
    // charge is ready
    class ChargeBoss : public Entity
    {
    public:
        ChargeBoss()
        {
        }

        static void Init();

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
            return (CHARGEBOSS_SCORE);
        }

    private:
        void CalcNextChargeTime()
        {
            m_nextChargeFinishTime = GAME.GetTime() +
                                     CHARGEBOSS_BASE_CHARGE_TIME +
                                     CHARGEBOSS_CHARGE_TIME_SCALE * m_health;
        }

        static const unsigned int   CHARGEBOSS_SCORE = 50;
        static const unsigned int   CHARGEBOSS_HEALTH = 8;
        static const float          CHARGEBOSS_BASE_CHARGE_TIME;
        static const float          CHARGEBOSS_CHARGE_TIME_SCALE;

        unsigned int     m_health;
        juzutil::Vector3 m_origin;
        Phrase           m_phrase;
        bool             m_moving;
        float            m_nextChargeFinishTime;
        ColourRGBA       m_colour;
        bool             m_chargeSoundPlaying;
        Sound            m_chargeSound;
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
        static const unsigned int     MISSILEBOSS_HEALTH = 8;
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


    template<typename T> class BossEnemyWave : public EnemyWave
    {
    public:
        void Spawn()
        {
            if (!m_spawned) {
                m_ent.reset(new T);
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
            return (0.0f);
        }

    private:
        EntityPtr m_ent;
        bool      m_spawned;
    };

    typedef BossEnemyWave<MissileBoss> MissileBossEnemyWave;
    typedef BossEnemyWave<ChargeBoss> ChargeBossEnemyWave;
    typedef BossEnemyWave<KnockbackBoss> KnockbackBossEnemyWave;
    typedef BossEnemyWave<BackwardsKnockbackBoss>
                                        BackwardsKnockbackBossEnemyWave;


}

#endif // _BOSS_H_
