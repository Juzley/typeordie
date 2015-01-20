#ifndef _ENEMY_WAVE_H_
#define _ENEMY_WAVE_H_

#include <tuple>
#include <memory>
#include <algorithm>
#include "Enemy.h"
#include "Random.h"

namespace typing
{
    class EnemyWave
    {
    public:        
        virtual bool         IsFinished() const = 0;

        virtual void Start()
        {
        }

        virtual void Spawn()
        {
        }
    };
    typedef std::shared_ptr<EnemyWave> EnemyWavePtr;
    typedef std::weak_ptr<EnemyWave>   EnemyWaveWeakPtr;

    typedef EnemyWavePtr (*WaveCreateFn)();

    template <typename T> static EnemyWavePtr CreateEnemyWave()
    {
        return EnemyWavePtr(new T);
    }

    class RandomEnemyWaveFactory
    {
        public:
            template <typename T> void AddWave(unsigned int minLevel)
            {
                m_waves.push_back(std::make_tuple(minLevel,
                                                  &CreateEnemyWave<T>));
            }

            EnemyWavePtr CreateWave(unsigned int level)
            {
                // Find the list of waves that are valid for the current level.
                WaveCreateVec validWaves(m_waves.size());
                auto iter = std::copy_if(
                    m_waves.begin(), m_waves.end(), validWaves.begin(),
                    [=](WaveMapping m) -> bool { return std::get<0>(m) <= level; });
                validWaves.resize(std::distance(validWaves.begin(), iter));

                // Pick a random wave from the valid waves.
                auto createTup =
                    validWaves[RAND.Range(
                        static_cast<unsigned int>(0),
                        static_cast<unsigned int>(validWaves.size() - 1))];
                WaveCreateFn createFn = std::get<1>(createTup);
                return createFn();
            }
        private:
            typedef std::tuple<unsigned int, WaveCreateFn> WaveMapping;
            typedef std::vector<WaveMapping> WaveCreateVec;

            WaveCreateVec m_waves;
    };

    class CyclicEnemyWaveFactory
    {
        public:
            template <typename T> void AddWave()
            {
                m_waves.push_back(&CreateEnemyWave<T>);
            }
            
            EnemyWavePtr CreateWave()
            {
                WaveCreateFn createFn = m_waves[m_index++ % m_waves.size()];
                return createFn();
            }

            void Reset()
            {
                m_index = 0;
            }

        private:
            typedef std::vector<WaveCreateFn> WaveCreateVec;

            WaveCreateVec m_waves;
            unsigned int  m_index;
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

    private:
        BasicEnemyVec m_enemies;
        unsigned int  m_enemyCount;
        float         m_enemySpeed;
        float         m_nextSpawnTime;
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

    private:
        AccelEnemyVec m_enemies;
        unsigned int  m_enemyCount;
        float         m_enemySpeed;
        float         m_nextSpawnTime;
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

    private:
        MissileEnemyVec m_enemies;
        unsigned int    m_enemyCount;
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

    private:
        BombEnemyVec m_enemies;
        unsigned int m_enemyCount;
        float        m_nextSpawnTime;
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

    private:
        static const unsigned int SEEKERENEMYWAVE_MIN_ENEMIES = 6;
        static const unsigned int SEEKERENEMYWAVE_MAX_ENEMIES = 12;
        static const float        SEEKERENEMYWAVE_SPAWN_GAP;
        static const float        SEEKERENEMYWAVE_SPAWN_Y;
        static const float        SEEKERENEMYWAVE_MAX_SPAWN_X;

        SeekerEnemyVec  m_enemies;
        unsigned int    m_enemyCount;
        float           m_nextSpawnTime;
    };
}

#endif // _ENEMY_WAVE_H_
