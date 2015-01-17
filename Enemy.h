#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <string>
#include <vector>
#include "Entity.h"
#include "Phrase.h"

namespace typing
{
    class ColourRGBA;

    //////////////////////////////////////////////////////////////////////////
    // BasicEnemy
    //////////////////////////////////////////////////////////////////////////

    class BasicEnemy : public Entity
    {
    public:
        BasicEnemy(const std::string& phrase, const juzutil::Vector3& origin, float speed)
            : m_phrase(phrase), m_origin(origin), m_speed(speed), m_unlink(false)
        {
        }

        void Update();
        void Draw2D();
        void Draw3D();
        void OnSpawn();
        void OnCollide();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        const BBox GetBounds() const
        {
            return (BBox() + m_origin);
        }

        bool IsSolid() const
        {
            return (true);
        }

        char GetStartChar() const
        {
            return (m_phrase.GetStartChar());
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
            return m_unlink;
        }

        unsigned int GetScore() const
        {
            return BASICENEMY_SCORE;
        }

    private:
        static const unsigned int BASICENEMY_SCORE = 20;
        static const BBox         BASICENEMY_BOUNDS;
        static const ColourRGBA   BASICENEMY_COLOUR;
        static const ColourRGBA   BASICENEMY_OUTLINECOLOUR;

        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
		float            m_speed;
        bool             m_unlink;
        juzutil::Vector3 m_dir;
        float            m_angle;
    };
    typedef boost::shared_ptr<BasicEnemy> BasicEnemyPtr;
    typedef std::vector<BasicEnemyPtr> BasicEnemyVec;


    //////////////////////////////////////////////////////////////////////////
    // AccelEnemy
    //////////////////////////////////////////////////////////////////////////

    class AccelEnemy : public Entity
    {
    public:
        AccelEnemy(const std::string& phrase, const juzutil::Vector3& origin, float startSpeed)
            : m_phrase(phrase), m_origin(origin), m_speed(startSpeed), m_unlink(false)
        {
        }

        void Update();
        void Draw2D();
        void Draw3D();
        void OnSpawn();
        void OnCollide();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        const BBox GetBounds() const
        {
            return (BBox() + m_origin);
        }

        bool IsSolid() const
        {
            return (true);
        }

        char GetStartChar() const
        {
            return (m_phrase.GetStartChar());
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
            return m_unlink;
        }

        unsigned int GetScore() const
        {
            return ACCELENEMY_SCORE;
        }

    private:
        static const unsigned int ACCELENEMY_SCORE = 30;
        static const BBox         ACCELENEMY_BOUNDS;
        static const ColourRGBA   ACCELENEMY_COLOUR;
        static const ColourRGBA   ACCELENEMY_OUTLINECOLOUR;
        static const float        ACCELENEMY_ACCEL;

        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
		float            m_speed;
        bool             m_unlink;
        juzutil::Vector3 m_dir;
        float            m_angle;
    };
    typedef boost::shared_ptr<AccelEnemy> AccelEnemyPtr;
    typedef std::vector<AccelEnemyPtr> AccelEnemyVec;


    //////////////////////////////////////////////////////////////////////////
    // Missile
    //////////////////////////////////////////////////////////////////////////

    class Missile : public Entity
    {
    public:
        Missile(const std::string& phrase, const juzutil::Vector3& origin)
            : m_phrase(phrase), m_origin(origin), m_unlink(false)
        {
        }

        void Update();
        void Draw2D();
        void Draw3D();
        void OnSpawn();
        void OnCollide();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        const BBox GetBounds() const
        {
            return (BBox() + m_origin);
        }

        bool IsSolid() const
        {
            return (true);
        }

        char GetStartChar() const
        {
            return (m_phrase.GetStartChar());
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
            return m_unlink;
        }

        unsigned int GetScore() const
        {
            return MISSILE_SCORE;
        }

    private:
        static const unsigned int MISSILE_SCORE = 10;
        static const float        MISSILE_SPEED;
        static const ColourRGBA   MISSILE_COLOUR;
        static const ColourRGBA   MISSILE_OUTLINECOLOUR;

        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
        bool             m_unlink;
        juzutil::Vector3 m_dir;
        float            m_angle;
        
    };
    typedef boost::shared_ptr<Missile> MissilePtr;


    //////////////////////////////////////////////////////////////////////////
    // MissileEnemy
    //////////////////////////////////////////////////////////////////////////

    class MissileEnemy : public Entity
    {
    public:
        MissileEnemy(const std::string& phrase, const juzutil::Vector3& origin, const juzutil::Vector3& dir)
            : m_phrase(phrase), m_origin(origin), m_dir(dir), m_unlink(false)
        {
        }

        void Update();
        void Draw2D();
        void Draw3D();
        void OnSpawn();
        void OnFinished();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        bool IsSolid() const
        {
            return (false);
        }

        char GetStartChar() const
        {
            return (m_phrase.GetStartChar());
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
            return m_unlink;
        }

        unsigned int GetScore() const
        {
            return (MISSILEENEMY_SCORE);
        }

    private:
        // Constants/Enums
        static const unsigned int     MISSILEENEMY_SCORE = 40;
        static const float            MISSILEENEMY_SPEED;
        static const float            MISSILEENEMY_FIREPAUSE;
        static const ColourRGBA       MISSILEENEMY_COLOUR;
        static const ColourRGBA       MISSILEENEMY_OUTLINECOLOUR;

        // Members
        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
        juzutil::Vector3 m_dir;
        bool             m_unlink;
        float            m_angle;
        float            m_lastFireTime;
    };
    typedef boost::shared_ptr<MissileEnemy> MissileEnemyPtr;
    typedef std::vector<MissileEnemyPtr> MissileEnemyVec;


    //////////////////////////////////////////////////////////////////////////
    // BombEnemy
    //////////////////////////////////////////////////////////////////////////

    class BombEnemy : public Entity
    {
    public:
        BombEnemy(const std::string& phrase, const juzutil::Vector3& origin)
            : m_phrase(phrase), m_origin(origin), m_unlink(false)
        {
        }

        void Update();
        void Draw2D();
        void Draw3D();
        void OnSpawn();
        void OnType(char c, bool *hit, bool *phraseFinished);

        const juzutil::Vector3& GetOrigin() const
        {
            return (m_origin);
        }

        bool IsSolid() const
        {
            return (false);
        }

        char GetStartChar() const
        {
            return (m_phrase.GetStartChar());
        }

        bool StartsWith(const char c) const
        {
            return (GetStartChar() == c);
        }

        float GetTypingSpeed() const
        {
            return (m_phrase.GetTypingSpeed());
        }

        bool IsPhraseSingle() const
        {
            return (m_phrase.Length() == 1);
        }

        bool Unlink() const
        {
            return (m_unlink);
        }

        unsigned int GetScore() const
        {
            return (BOMB_SCORE);
        }

    private:
        // Constants/Enums
        static const unsigned int BOMB_SCORE = 50;
        static const ColourRGBA   BOMB_COLOUR;
        static const ColourRGBA   BOMB_OUTLINECOLOUR;
        static const float        BOMB_MAX_ROTATE_SPEED;
        static const float        BOMB_DETONATE_TIME;
        static const float        BOMB_BLINK_TIME;
        static const float        BOMB_BLINK_SPEED;

        // Methods
        void Detonate();

        // Members
        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
        bool             m_unlink;
        float            m_spawnTime;
        juzutil::Vector3 m_angles;
        juzutil::Vector3 m_angleSpeed;
    };
    typedef boost::shared_ptr<BombEnemy> BombEnemyPtr;
    typedef std::vector<BombEnemyPtr> BombEnemyVec;


    //////////////////////////////////////////////////////////////////////////
    // SeekerEnemy
    //////////////////////////////////////////////////////////////////////////

    class SeekerEnemy : public Entity
    {
    public:
        SeekerEnemy(const std::string& phrase, const juzutil::Vector3& origin)
            : m_phrase(phrase), m_origin(origin), m_unlink(false)
        {
        }

        void Update();
        void Draw2D();
        void Draw3D();
        void OnSpawn();
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
            return (BBox() + m_origin);
        }

        char GetStartChar() const
        {
            return (m_phrase.GetStartChar());
        }

        bool StartsWith(const char c) const
        {
            return (GetStartChar() == c);
        }

        float GetTypingSpeed() const
        {
            return (m_phrase.GetTypingSpeed());
        }

        bool IsPhraseSingle() const
        {
            return (m_phrase.Length() == 1);
        }

        bool Unlink() const
        {
            return (m_unlink);
        }

        unsigned int GetScore() const
        {
            return (SEEKER_SCORE);
        }

    private:
        // Constants/Enums
        static const unsigned int SEEKER_SCORE = 50;
        static const ColourRGBA   SEEKER_COLOUR;
        static const ColourRGBA   SEEKER_OUTLINECOLOUR;
        static const float        SEEKER_SEEK_TIME;
        static const float        SEEKER_TURN_SPEED;
        static const float        SEEKER_SEEK_MOVE_SPEED;
        static const float        SEEKER_ATTACK_MOVE_SPEED;

        // Methods
        void StartAttack();

        // Members
        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
        juzutil::Vector3 m_dir;
        bool             m_unlink;
        float            m_spawnTime;
        float            m_angle;
        float            m_destAngle;
        bool             m_seeking;
        bool             m_turning;
    };
    typedef boost::shared_ptr<SeekerEnemy> SeekerEnemyPtr;
    typedef std::vector<SeekerEnemyPtr> SeekerEnemyVec;
}

#endif // _ENEMY_H_
