#include <boost/format.hpp>
#include "EnemyWave.h"
#include "Game.h"
#include "Random.h"

namespace typing
{
    // Y coordinate for enemies that spawn at the top.
    const float SPAWN_TOP_Y = 1400.0f;

    // Min X coordinate for enemies that spawn at the top.
    const float SPAWN_TOP_MIN_X = 100.0f;

    // Max X coordinate for enemies that spawn at the top.
    const float SPAWN_TOP_MAX_X = 1100.0f;


    //////////////////////////////////////////////////////////////////////////
    // BasicEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void BasicEnemyWave::Start ()
    {
        const unsigned int ENEMIES_MIN = 2; 
        const unsigned int ENEMIES_MAX = 8;
        const unsigned int ENEMIES_PER_LEVEL = 0;
        const float        SPEED_MIN = 100.0f;
        const float        SPEED_MAX = 200.0f;
        const float        SPEED_PER_LEVEL = 10.0f;

        m_enemyCount = 
            std::min(ENEMIES_MAX,
                     ENEMIES_MIN + GAME.GetLevel() * ENEMIES_PER_LEVEL);
        m_enemySpeed = 
            std::min(SPEED_MAX,
                     SPEED_MIN + GAME.GetLevel() * SPEED_PER_LEVEL);

        m_nextSpawnTime = 0;

        APP.Log(App::LOG_DEBUG,
                boost::str(boost::format(
                    "Starting Basic Enemy Wave."
                    " Level %1%, %2% enemies, %3% speed")
                    % GAME.GetLevel() % m_enemyCount % m_enemySpeed));
    }

    void BasicEnemyWave::Spawn ()
    {
        const float SPAWN_GAP = 1.0f;

        if (m_enemyCount > m_enemies.size() &&
            m_nextSpawnTime <= GAME.GetTime()) {
            float x = RAND.Range(SPAWN_TOP_MIN_X, SPAWN_TOP_MAX_X) *
                (m_enemies.size() % 2 == 0 ? 1.0f : -1.0f);

            BasicEnemyPtr enemy(
                new BasicEnemy(GAME.GetPhrase(PhraseBook::PL_MEDIUM),
                               juzutil::Vector3(x, SPAWN_TOP_Y, 0.0f),
                               m_enemySpeed));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_nextSpawnTime = GAME.GetTime() + SPAWN_GAP;
        }
    }

    bool BasicEnemyWave::IsFinished () const
    {
        return (m_enemies.size() >= m_enemyCount &&
                std::count_if(
                    m_enemies.begin(),
                    m_enemies.end(),
                    [](BasicEnemyPtr e) { return !e->Unlink(); }) == 0 );
    }


    //////////////////////////////////////////////////////////////////////////
    // AccelEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void AccelEnemyWave::Start ()
    {
        const unsigned int ENEMIES_MIN = 2; 
        const unsigned int ENEMIES_MAX = 8;
        const unsigned int ENEMIES_PER_LEVEL = 0;
        const float        SPEED_MIN = 100.0f;
        const float        SPEED_MAX = 200.0f;
        const float        SPEED_PER_LEVEL = 10.0f;

        m_enemyCount =
            std::min(ENEMIES_MAX,
                     ENEMIES_MIN + GAME.GetLevel() * ENEMIES_PER_LEVEL);
        m_enemySpeed = 
            std::min(SPEED_MAX,
                     SPEED_MIN + GAME.GetLevel() * SPEED_PER_LEVEL);

        m_nextSpawnTime = 0;

        APP.Log(App::LOG_DEBUG,
                boost::str(boost::format(
                    "Starting Accel Enemy Wave."
                    " Level %1%, %2% enemies, %3% speed")
                    % GAME.GetLevel() % m_enemyCount % m_enemySpeed));
    }

    void AccelEnemyWave::Spawn ()
    {
        const float SPAWN_GAP = 1.0f;

        if (m_enemyCount > m_enemies.size() &&
            m_nextSpawnTime <= GAME.GetTime()) {
            float x = RAND.Range(SPAWN_TOP_MIN_X, SPAWN_TOP_MAX_X) *
                (m_enemies.size() % 2 == 0 ? 1.0f : -1.0f);

            AccelEnemyPtr enemy(
                new AccelEnemy(GAME.GetPhrase(PhraseBook::PL_LONG),
                               juzutil::Vector3(x, SPAWN_TOP_Y, 0.0f),
                               m_enemySpeed));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_nextSpawnTime = GAME.GetTime() + SPAWN_GAP;
        }
    }

    bool AccelEnemyWave::IsFinished () const
    {
        return (m_enemies.size() >= m_enemyCount &&
                std::count_if(
                    m_enemies.begin(),
                    m_enemies.end(),
                    [](AccelEnemyPtr e) { return !e->Unlink(); }) == 0 );
    }


    //////////////////////////////////////////////////////////////////////////
    // MissileEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void MissileEnemyWave::Start()
    {
        const unsigned int ENEMIES_MIN = 2;
        const unsigned int ENEMIES_MAX = 6;
        const unsigned int ENEMIES_PER_LEVEL = 0;
        
        m_enemyCount =
            std::min(ENEMIES_MAX,
                     ENEMIES_MIN + GAME.GetLevel() * ENEMIES_PER_LEVEL);
        m_nextSpawnTime = 0.0f;

        APP.Log(App::LOG_DEBUG,
                boost::str(boost::format(
                    "Starting Missile Enemy Wave. Level %1%, %2% enemies")
                    % GAME.GetLevel() % m_enemyCount));
    }

    void MissileEnemyWave::Spawn()
    {
        const float SPAWN_GAP = 1.0f;
        const float SPAWN_Y_BASE = 500.0f;
        const float SPAWN_Y_GAP  = 100.0f;

        if (m_enemyCount > m_enemies.size() &&
            m_nextSpawnTime <= GAME.GetTime()) {
            float x;
            float y;
            juzutil::Vector3 dir;

            if (m_enemies.size() % 2 == 0) {
                x = Game::GAME_SCREEN_RIGHT;
                dir.Set(-1.0f, 0.0f, 0.0f);
            } else {
                x = Game::GAME_SCREEN_LEFT;
                dir.Set(1.0f, 0.0f, 0.0f);
            }

            y = SPAWN_Y_BASE + SPAWN_Y_GAP * m_enemies.size();

            MissileEnemyPtr enemy(
                new MissileEnemy(GAME.GetComboPhrase(2, PhraseBook::PL_MEDIUM),
                                 juzutil::Vector3(x, y, 0.0f),
                                 dir));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_nextSpawnTime = GAME.GetTime() + SPAWN_GAP;
        }
    }

    bool MissileEnemyWave::IsFinished() const
    {
        return (m_enemies.size() >= m_enemyCount &&
                std::count_if(
                    m_enemies.begin(),
                    m_enemies.end(),
                    [](MissileEnemyPtr e) { return !e->Unlink(); }) == 0 );
    }


    //////////////////////////////////////////////////////////////////////////
    // BombEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void BombEnemyWave::Start()
    {
        const unsigned int ENEMIES_MIN = 2;
        const unsigned int ENEMIES_MAX = 6;
        const unsigned int ENEMIES_PER_LEVEL = 0;
        
        m_enemyCount =
            std::min(ENEMIES_MAX,
                     ENEMIES_MIN + GAME.GetLevel() * ENEMIES_PER_LEVEL);
        m_nextSpawnTime = 0.0f;

        APP.Log(App::LOG_DEBUG,
                boost::str(boost::format(
                    "Starting Bomb Enemy Wave. Level %1%, %2% enemies")
                    % GAME.GetLevel() % m_enemyCount));
    }

    void BombEnemyWave::Spawn()
    {
        const BBox SPAWN_AREA(juzutil::Vector2(-500.0f, -100.0f),
                              juzutil::Vector2(500.0f, 500.0f));
        const float SPAWN_GAP = 1.0f;

        if (m_enemyCount > m_enemies.size() &&
            m_nextSpawnTime <= GAME.GetTime()) {
            juzutil::Vector2 org(RAND.Range(SPAWN_AREA.GetMin().GetX(),
                                            SPAWN_AREA.GetMax().GetX()),
                                 RAND.Range(SPAWN_AREA.GetMin().GetY(),
                                            SPAWN_AREA.GetMax().GetY()));
            BombEnemyPtr bombEnemy(
                new BombEnemy(GAME.GetPhrase(PhraseBook::PL_LONG),
                              juzutil::Vector3(org)));
            m_enemies.push_back(bombEnemy);
            GAME.AddEntity(bombEnemy);
            
            m_nextSpawnTime = GAME.GetTime() + SPAWN_GAP;
        }
    }

    bool BombEnemyWave::IsFinished() const
    {
        return (m_enemies.size() >= m_enemyCount &&
                std::count_if(
                    m_enemies.begin(),
                    m_enemies.end(),
                    [](BombEnemyPtr e) { return !e->Unlink(); }) == 0 );
    }


    //////////////////////////////////////////////////////////////////////////
    // SeekerEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void SeekerEnemyWave::Start()
    {
        const unsigned int ENEMIES_MIN = 4;
        const unsigned int ENEMIES_MAX = 12;
        const unsigned int ENEMIES_PER_LEVEL = 0;

        m_enemyCount =
            std::min(ENEMIES_MAX,
                     ENEMIES_MIN + GAME.GetLevel() * ENEMIES_PER_LEVEL);
        m_nextSpawnTime = 0.0f;

        APP.Log(App::LOG_DEBUG,
                boost::str(boost::format(
                    "Starting Missile Enemy Wave. Level %1%, %2% enemies")
                    % GAME.GetLevel() % m_enemyCount));
    }

    void SeekerEnemyWave::Spawn()
    {
        const float SPAWN_GAP = 1.0f;

        if (m_enemyCount > m_enemies.size() &&
            m_nextSpawnTime <= GAME.GetTime()) {
            float x = RAND.Range(SPAWN_TOP_MIN_X, SPAWN_TOP_MAX_X) *
                (m_enemies.size() % 2 == 0 ? 1.0f : -1.0f);

            SeekerEnemyPtr enemy(
                new SeekerEnemy(GAME.GetPhrase(PhraseBook::PL_LONG),
                                juzutil::Vector3(x, SPAWN_TOP_Y, 0.0f)));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_nextSpawnTime = GAME.GetTime() + SPAWN_GAP;
        }
    }

    bool SeekerEnemyWave::IsFinished() const
    {
        return (m_enemies.size() >= m_enemyCount &&
                std::count_if(
                    m_enemies.begin(),
                    m_enemies.end(),
                    [](SeekerEnemyPtr e) { return !e->Unlink(); }) == 0 );
    }
}
