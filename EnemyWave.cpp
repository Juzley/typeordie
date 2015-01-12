#include "EnemyWave.h"
#include "Game.h"
#include "Random.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // BasicEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void BasicEnemyWave::Start ()
    {
        m_enemyCount = RAND.Range(BASICENEMYWAVE_MIN_ENEMIES, BASICENEMYWAVE_MAX_ENEMIES);
        m_nextSpawnTime = 0;
        m_enemiesSpawned = 0;
    }

    void BasicEnemyWave::Spawn ()
	{
		const float MIN_SPAWN_X = 200.0f;
		const float MAX_SPAWN_X = 800.0f;
		const float SPAWN_Y = 1000.0f;

		const float MAX_SPAWN_GAP = 3.0f;
		const float MIN_SPAWN_GAP = 1.0f;
		const float MIN_SPEED = 50.0f;
		const float MAX_SPEED = 150.0f;
		const float PROGRESS_CEILING = 0.75f;

        if (m_enemiesSpawned < m_enemyCount && m_nextSpawnTime <= GAME.GetTime()) {
			const float progress = std::min(PROGRESS_CEILING, GAME.GetProgress()) / PROGRESS_CEILING;
			const float speed = RangeFromRatio(MIN_SPEED, MAX_SPEED, progress);
			const float gap = RangeFromRatioInverse(MIN_SPAWN_GAP, MAX_SPAWN_GAP, progress);

            float x = RAND.Range(MIN_SPAWN_X, MAX_SPAWN_X);

            if (m_enemiesSpawned++ % 2 == 0) {
                x *= -1;
            }

            BasicEnemyPtr enemy(new BasicEnemy(GAME.GetPhrase(PhraseBook::PL_TINY),
				                               juzutil::Vector3(x, SPAWN_Y, 0.0f),
											   speed));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_nextSpawnTime = GAME.GetTime() + gap;
        }
    }

    bool BasicEnemyWave::IsFinished () const
    {
        bool finished = true;

        if (m_enemiesSpawned >= m_enemyCount) {
            for (BasicEnemyVec::const_iterator iter = m_enemies.begin(); iter != m_enemies.end(); ++iter) {
                if (!(*iter)->Unlink()) {
                    finished = false;
                    break;
                }
            }
        } else {
            finished = false;
        }

        return (finished);
    }

    void BasicEnemyWave::OnFinished ()
    {
        m_enemies.clear();
    }


    //////////////////////////////////////////////////////////////////////////
    // AccelEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void AccelEnemyWave::Start ()
    {
        m_enemyCount = RAND.Range(ACCELENEMYWAVE_MIN_ENEMIES, ACCELENEMYWAVE_MAX_ENEMIES);
        m_nextSpawnTime = 0;
        m_enemiesSpawned = 0;
    }

    void AccelEnemyWave::Spawn ()
	{
		const float MIN_SPAWN_X = 200.0f;
		const float MAX_SPAWN_X = 800.0f;
		const float SPAWN_Y = 1000.0f;

		const float MAX_SPAWN_GAP = 3.0f;
		const float MIN_SPAWN_GAP = 1.0f;
		const float MIN_SPEED = 50.0f;
		const float MAX_SPEED = 150.0f;
		const float PROGRESS_CEILING = 0.75f;

        if (m_enemiesSpawned < m_enemyCount && m_nextSpawnTime <= GAME.GetTime()) {
			const float progress = std::min(PROGRESS_CEILING, GAME.GetProgress()) / PROGRESS_CEILING;
			const float speed = RangeFromRatio(MIN_SPEED, MAX_SPEED, progress);
			const float gap = RangeFromRatioInverse(MIN_SPAWN_GAP, MAX_SPAWN_GAP, progress);

            float x = RAND.Range(MIN_SPAWN_X, MAX_SPAWN_X);

            if (m_enemiesSpawned++ % 2 == 0) {
                x *= -1;
            }

            AccelEnemyPtr enemy(new AccelEnemy(GAME.GetPhrase(PhraseBook::PL_SHORT),
				                               juzutil::Vector3(x, SPAWN_Y, 0.0f),
											   speed));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_nextSpawnTime = GAME.GetTime() + gap;
        }
    }

    bool AccelEnemyWave::IsFinished () const
    {
        bool finished = true;

        if (m_enemiesSpawned >= m_enemyCount) {
            for (AccelEnemyVec::const_iterator iter = m_enemies.begin(); iter != m_enemies.end(); ++iter) {
                if (!(*iter)->Unlink()) {
                    finished = false;
                    break;
                }
            }
        } else {
            finished = false;
        }

        return (finished);
    }

    void AccelEnemyWave::OnFinished ()
    {
        m_enemies.clear();
    }


    //////////////////////////////////////////////////////////////////////////
    // MissileEnemyWave
    //////////////////////////////////////////////////////////////////////////

    void MissileEnemyWave::Start()
    {
        const int   MIN_ENEMIES = 2;
        const int   MAX_ENEMIES = 4;
        const float PROGRESS_CEILING = 0.75f;
        
        const float progress = std::min(PROGRESS_CEILING, GAME.GetProgress()) / PROGRESS_CEILING;
        m_enemyCount = RangeFromRatio(MIN_ENEMIES, MAX_ENEMIES, progress);
        m_enemiesSpawned = 0;
        m_nextSpawnTime  = 0;
    }

    void MissileEnemyWave::Spawn()
    {
        const float MIN_SPAWN_GAP = 0.3f;
        const float MAX_SPAWN_GAP = 0.6f;
        const float SPAWN_Y_BASE = 500.0f;
        const float SPAWN_Y_GAP  = 100.0f;

        if (m_enemiesSpawned < m_enemyCount && m_nextSpawnTime <= GAME.GetTime()) {
            float x;
            float y;
            juzutil::Vector3 dir;

            if (m_enemiesSpawned % 2 == 0) {
                x = Game::GAME_SCREEN_RIGHT;
                dir.Set(-1.0f, 0.0f, 0.0f);
            } else {
                x = Game::GAME_SCREEN_LEFT;
                dir.Set(1.0f, 0.0f, 0.0f);
            }

            y = SPAWN_Y_BASE + SPAWN_Y_GAP * m_enemiesSpawned;

            MissileEnemyPtr enemy(new MissileEnemy(GAME.GetPhrase(PhraseBook::PL_MEDIUM),
                                                   juzutil::Vector3(x, y, 0.0f),
                                                   dir));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            ++m_enemiesSpawned;
            m_nextSpawnTime = RAND.Range(MIN_SPAWN_GAP, MAX_SPAWN_GAP) + GAME.GetTime();
        }
    }

    bool MissileEnemyWave::IsFinished() const
    {
        bool finished = true;

        if (m_enemiesSpawned >= m_enemyCount) {
            for (MissileEnemyVec::const_iterator iter = m_enemies.begin(); iter != m_enemies.end(); ++iter) {
                if (!(*iter)->Unlink()) {
                    finished = false;
                    break;
                }
            }
        } else {
            finished = false;
        }

        return (finished);
    }

    void MissileEnemyWave::OnFinished()
    {
        m_enemies.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    // BombEnemyWave
    //////////////////////////////////////////////////////////////////////////

    const BBox  BombEnemyWave::INNER_BOUNDS(juzutil::Vector2(-20.0f, -20.0f), juzutil::Vector2(20.0f, 20.0f));
    const BBox  BombEnemyWave::OUTER_BOUNDS(juzutil::Vector2(-500.0f, -100.0f), juzutil::Vector2(500.0f, 500.0f));

    void BombEnemyWave::Start()
    {
        m_spawned = false;
    }

    void BombEnemyWave::Spawn()
    {
        if (!m_spawned) {
            const juzutil::Vector2 innerRange = INNER_BOUNDS.GetMax() - INNER_BOUNDS.GetMin();
            const juzutil::Vector2 range = (OUTER_BOUNDS.GetMax() - OUTER_BOUNDS.GetMin()) - innerRange;

            const unsigned int enemies =
                static_cast<unsigned int>(MIN_ENEMIES +
                (MAX_ENEMIES - MIN_ENEMIES) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));

            for (unsigned int i = 0; i < enemies; i++) {
                juzutil::Vector2 spawnOrg(RAND.Range(0.0f, range.GetX()),
                                          RAND.Range(0.0f, range.GetY()));
                spawnOrg += OUTER_BOUNDS.GetMin();

                if (spawnOrg.GetX() > INNER_BOUNDS.GetMinX() &&
                    spawnOrg.GetY() > INNER_BOUNDS.GetMinY()) {
                    spawnOrg += INNER_BOUNDS.GetMin();
                }

                BombEnemyPtr bombEnemy(
                    new BombEnemy(GAME.GetPhrase(PhraseBook::PL_MEDIUM), juzutil::Vector3(spawnOrg)));
                m_enemies.push_back(bombEnemy);
                GAME.AddEntity(bombEnemy);
            }

            m_spawned = true;
        }
    }

    bool BombEnemyWave::IsFinished() const
    {
        bool finished = true;

        if (m_spawned) {
            for (BombEnemyVec::const_iterator iter = m_enemies.begin(); iter != m_enemies.end(); ++iter) {
                if (!(*iter)->Unlink()) {
                    finished = false;
                    break;
                }
            }
        } else {
            finished = false;
        }

        return (finished);
    }

    void BombEnemyWave::OnFinished()
    {
        m_enemies.clear();
    }


    //////////////////////////////////////////////////////////////////////////
    // SeekerEnemyWave
    //////////////////////////////////////////////////////////////////////////

    const float SeekerEnemyWave::SEEKERENEMYWAVE_SPAWN_GAP      = 3.0f;
    const float SeekerEnemyWave::SEEKERENEMYWAVE_MAX_SPAWN_X    = 500.0f;
    const float SeekerEnemyWave::SEEKERENEMYWAVE_SPAWN_Y        = 1000.0f;

    void SeekerEnemyWave::Start()
    {
        // TODO: Different numbers of enemies based on progress
        m_enemyCount = SEEKERENEMYWAVE_MIN_ENEMIES;
        m_enemiesSpawned = 0;
        m_nextSpawnTime = GAME.GetTime() + SEEKERENEMYWAVE_SPAWN_GAP;
    }

    void SeekerEnemyWave::Spawn()
    {
        if (m_enemiesSpawned < m_enemyCount && GAME.GetTime() >= m_nextSpawnTime) {
            const float xGap = SEEKERENEMYWAVE_MAX_SPAWN_X / static_cast<float>(m_enemyCount - 1);
            const float x = xGap * (m_enemyCount - m_enemiesSpawned) + xGap / 2.0f;

            SeekerEnemyPtr enemy(new SeekerEnemy(GAME.GetPhrase(PhraseBook::PL_MEDIUM),
                                                 juzutil::Vector3(x, SEEKERENEMYWAVE_SPAWN_Y, 0.0f)));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            enemy.reset(new SeekerEnemy(GAME.GetPhrase(PhraseBook::PL_MEDIUM),
                                        juzutil::Vector3(-x, SEEKERENEMYWAVE_SPAWN_Y, 0.0f)));
            m_enemies.push_back(enemy);
            GAME.AddEntity(enemy);

            m_enemiesSpawned += 2;
            m_nextSpawnTime = GAME.GetTime() + SEEKERENEMYWAVE_SPAWN_GAP;
        }
    }

    bool SeekerEnemyWave::IsFinished() const
    {
        bool finished = true;

        if (m_enemiesSpawned >= m_enemyCount) {
            for (SeekerEnemyVec::const_iterator iter = m_enemies.begin(); iter != m_enemies.end(); ++iter) {
                if (!(*iter)->Unlink()) {
                    finished = false;
                    break;
                }
            }
        } else {
            finished = false;
        }

        return (finished);
    }

    void SeekerEnemyWave::OnFinished()
    {
        m_enemies.clear();
    }
}
