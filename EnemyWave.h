#ifndef _ENEMY_WAVE_H_
#define _ENEMY_WAVE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "Enemy.h"

namespace typing
{
    class EnemyWave
    {
    public:        
        virtual bool  IsFinished() const = 0;
        virtual float MinProgress() const = 0;

        virtual void Start()
        {
        }

        virtual void Spawn()
        {
        }

        virtual void OnFinished()
        {
        }
    };
    typedef boost::shared_ptr<EnemyWave> EnemyWavePtr;
    typedef boost::weak_ptr<EnemyWave>   EnemyWaveWeakPtr;

    class EnemyWaveSort
    {
    public:
        bool operator()(const EnemyWavePtr& a, const EnemyWavePtr& b) const
        {
            return (a->MinProgress() < b->MinProgress());
        }
    };


    //////////////////////////////////////////////////////////////////////////
    // BasicEnemyWave
    //////////////////////////////////////////////////////////////////////////
    class BasicEnemyWave : public EnemyWave
    {
    public:
        void Start();
        void Spawn();
        bool IsFinished() const;
        void OnFinished();

        float MinProgress() const
        {
            return 0.0f;
        }

    private:
        static const unsigned int BASICENEMYWAVE_MIN_ENEMIES = 5;
        static const unsigned int BASICENEMYWAVE_MAX_ENEMIES = 10;

        BasicEnemyVec m_enemies;
        unsigned int  m_enemyCount;
        unsigned int  m_enemiesSpawned;
        float         m_nextSpawnTime;
        float         m_waveEndTime;
    };


    //////////////////////////////////////////////////////////////////////////
    // AccelEnemyWave
    //////////////////////////////////////////////////////////////////////////
    class AccelEnemyWave : public EnemyWave
    {
    public:
        void Start();
        void Spawn();
        bool IsFinished() const;
        void OnFinished();

        float MinProgress() const
        {
            return 0.1f;
        }

    private:
        static const unsigned int ACCELENEMYWAVE_MIN_ENEMIES = 5;
        static const unsigned int ACCELENEMYWAVE_MAX_ENEMIES = 10;

        AccelEnemyVec m_enemies;
        unsigned int  m_enemyCount;
        unsigned int  m_enemiesSpawned;
        float         m_nextSpawnTime;
        float         m_waveEndTime;
    };


    //////////////////////////////////////////////////////////////////////////
    // MissileEnemyWave
    //////////////////////////////////////////////////////////////////////////
    class MissileEnemyWave : public EnemyWave
    {
    public:
        void Start();
        void Spawn();
        bool IsFinished() const;
        void OnFinished();

        float MinProgress() const
        {
            return (0.01f);
        }

    private:
        static const unsigned int MISSILEENEMYWAVE_MIN_ENEMIES = 6;
        static const unsigned int MISSILEENEMYWAVE_MAX_ENEMIES = 10;

        MissileEnemyVec m_enemies;
        unsigned int    m_enemyCount;
        unsigned int    m_enemiesSpawned;
        float           m_nextSpawnTime;
    };


    //////////////////////////////////////////////////////////////////////////
    // BombEnemyWave
    //////////////////////////////////////////////////////////////////////////
    class BombEnemyWave : public EnemyWave
    {
    public:
        BombEnemyWave()
        {
        }

        void Start();
        void Spawn();
        bool IsFinished() const;
        void OnFinished();

        float MinProgress() const
        {
            return (0.75f);
        }

    private:
        static const unsigned int MIN_ENEMIES = 2;
        static const unsigned int MAX_ENEMIES = 4;
        static const float        WAVE_TIME;
        static const BBox         OUTER_BOUNDS;
        static const BBox         INNER_BOUNDS;

        BombEnemyVec m_enemies;
        bool         m_spawned;
        float        m_spawnTime;
    };


    //////////////////////////////////////////////////////////////////////////
    // SeekerEnemyWave
    //////////////////////////////////////////////////////////////////////////
    class SeekerEnemyWave : public EnemyWave
    {
    public:
        void Start();
        void Spawn();
        bool IsFinished() const;
        void OnFinished();

        float MinProgress() const
        {
            return (0.50f);
        }

    private:
        static const unsigned int SEEKERENEMYWAVE_MIN_ENEMIES = 6;
        static const unsigned int SEEKERENEMYWAVE_MAX_ENEMIES = 12;
        static const float        SEEKERENEMYWAVE_SPAWN_GAP;
        static const float        SEEKERENEMYWAVE_SPAWN_Y;
        static const float        SEEKERENEMYWAVE_MAX_SPAWN_X;

        SeekerEnemyVec  m_enemies;
        unsigned int    m_enemyCount;
        unsigned int    m_enemiesSpawned;
        float           m_nextSpawnTime;
    };
}

#endif // _ENEMY_WAVE_H_