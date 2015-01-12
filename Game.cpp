#include <ctime>
#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include "Game.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Utils.h"
#include "Menu.h"
#include "MenuPause.h"
#include "MenuMain.h"
#include "MenuNewHighScore.h"
#include "Award.h"
#include "Laser.h"
#include "Powerup.h"
#include "HighScores.h"
#include "Explosion.h"
#include "Phrase.h"
#include "Exceptions.h"
#include "Boss.h"
#include "Random.h"

namespace typing
{
    const float       Game::GAME_SCREEN_TOP = 1000.0f;
    const float       Game::GAME_SCREEN_BOTTOM = -100.0f;
    const float       Game::GAME_SCREEN_LEFT = -775.0f;
    const float       Game::GAME_SCREEN_RIGHT = 775.0f;
    const float       Game::FINAL_DEATH_PAUSE = 2.0f;
    const float       Game::BOSS_WAVE_WAIT_TIME = 5.0f;
    const float       Game::MIN_POWERUP_SPAWN_TIME = 30.0f;
    const float       Game::MAX_POWERUP_SPAWN_TIME = 180.0f;
    const float       Game::ORTHO_WIDTH       = 800.0f;
    const float       Game::ORTHO_HEIGHT      = 600.0f;
    const std::string Game::HUD_FONT("fonts/hudfont.fnt");
    const std::string Game::ENDGAME_FONT("fonts/menufont.fnt");
    const std::string Game::MISS_SOUND("sounds/miss.wav");
    const std::string Game::TARGET_SOUND("sounds/target.wav");
    const std::string Game::GAME_MUSIC("music/music.mp3");

    std::auto_ptr<Game> Game::m_singleton(new Game);
    Game& Game::GetGame ()
    {
        return *(m_singleton.get());
    }

    Game::Game()
        : m_camera(juzutil::Vector3(0.0f, -200.0f, 500.0f), juzutil::Vector3(0.0f, 200.0f, 0.0f)), m_active(false)
    {
    }


    void Game::Init ()
    {
        FONTS.Add(HUD_FONT);
        FONTS.Add(ENDGAME_FONT);
        SOUNDS.Add(MISS_SOUND);
        SOUNDS.Add(TARGET_SOUND);

        // Load the music
        m_music = Mix_LoadMUS(GAME_MUSIC.c_str());
        if(!m_music)
        {
            throw FileNotFoundException(GAME_MUSIC);
        }

        m_phrases.Init();

        // Initialise entities needed by the game.
        // Makes sure all required media is loaded.
        Player::Init();
        Phrase::Init();
        Award::Init();
        Explosion::Init();
        Laser::Init();

        // Initialise waves
        m_waves.push_back(EnemyWavePtr(new BasicEnemyWave));
        m_waves.push_back(EnemyWavePtr(new MissileEnemyWave));
        m_waves.push_back(EnemyWavePtr(new AccelEnemyWave));
        m_waves.push_back(EnemyWavePtr(new BombEnemyWave));
        m_waves.push_back(EnemyWavePtr(new SeekerEnemyWave));
        std::sort(m_waves.begin(), m_waves.end(), EnemyWaveSort());

        m_bossWaves.push_back(EnemyWavePtr(new MissileBossEnemyWave));
        std::sort(m_bossWaves.begin(), m_bossWaves.end(), EnemyWaveSort());

        // Initialise powerups
        m_powerups.Register(CreatePowerup<ExtraLife>);
    }


    void Game::StartNewGame ()
    {
        const int MUSIC_FADE_IN_TIME = 2000;

        m_phrases.MakeAllCharsAvail();

        m_entities.clear();
        m_effects.clear();
        m_effects2d.clear();
        m_lives = GAME_START_LIVES;

        Pause(false);
        Activate(true);

        m_player.Reset();
        m_timer.Reset();

        m_gameEndTime       = 0.0f;
        m_score             = 0;
        m_streakValid       = false;
        m_streak            = 0;
        m_invincible        = false;

        for (int i = 0; i < FLOW_CONTRIB_COUNT; i++) {
            m_flowContribs[i] = 0;
        }
        m_flow                   = 0;
        m_oldestFlowContribIndex = 0;

        m_hits        = 0;
        m_misses      = 0;
        m_excellents  = 0;
        m_goods       = 0;
        m_oks         = 0;
        m_poors       = 0;
        m_bads        = 0;
        m_usedLives   = 0;
        m_maxStreak   = 0;
        
        RAND.Seed(static_cast<unsigned int>(std::time(0)));

        m_currentWave.reset();
        m_targetEnt.reset();

        m_progress           = 0;
        m_lastValidWaveIndex = 0;
        m_bossWavePending   = false;
        m_bossWaveStartTime = 0.0f;
        m_nextBossWaveIndex = 0;

        m_nextPowerupTime = RAND.Range(MIN_POWERUP_SPAWN_TIME, MAX_POWERUP_SPAWN_TIME);

        Mix_FadeInMusic(m_music, -1, MUSIC_FADE_IN_TIME);
    }


    void Game::SpawnEnemies()
    {
        EnemyWavePtr wave = m_currentWave.lock();

        if (wave) {
            // If we're waiting to spawn a boss wave, check whether we've
            // waited long enough to spawn it.
            if (m_bossWavePending) {
                if (m_bossWaveStartTime <= GetTime()) {
                    m_bossWavePending = false;
                    wave->Start();
                }
            } else {
                wave->Spawn();

                if (wave->IsFinished())
                {
                    m_currentWave.reset();
                    wave.reset();

                    m_progress += m_flow;
                }
            }
        }

        // There is no active wave, find a new one
        // First check if we should spawn a boss wave.
        if (!wave && !m_bossWaves.empty()) {
            if (m_nextBossWaveIndex < m_bossWaves.size()) {
                const float nextBossProgress = m_bossWaves[m_nextBossWaveIndex]->MinProgress();
                if (m_lastBossCheckedProgress <= nextBossProgress && GetProgress() >= nextBossProgress) {
                    m_currentWave = m_bossWaves[m_nextBossWaveIndex++];
                    wave = m_currentWave.lock();

                    m_bossWavePending = true;
                    m_bossWaveStartTime = GetTime() + BOSS_WAVE_WAIT_TIME;
                }
            }

            m_lastBossCheckedProgress = GetProgress();
        }

        // We didn't spawn a boss wave, spawn a normal wave of enemies.
        if (!wave && !m_waves.empty()) {
            // Find the range of waves that are valid for the current game progress
            // The wave collection is sorted by increasing progress, so we can start
            // the search at the last known valid wave.
            while (m_lastValidWaveIndex < m_waves.size() &&
                   GetProgress() >= m_waves[m_lastValidWaveIndex]->MinProgress())
            {
                ++m_lastValidWaveIndex;
            }

            // Pick a random wave from the currently valid wave types to spawn.
            const unsigned int index = RAND.Range(static_cast<unsigned int>(0), static_cast<unsigned int>(m_lastValidWaveIndex - 1));
            m_currentWave = m_waves[index];
            m_waves[index]->Start();
        }
    }


    void Game::SpawnPowerups()
    {
        const float MIN_X = -200.0f;
        const float MAX_X = 200.0f;
        const float MIN_Y = 100.0f;
        const float MAX_Y = 500.0f;

        if (GetTime() >= m_nextPowerupTime) {
            AddEntity(m_powerups.Create(juzutil::Vector2(RAND.Range(MIN_X, MAX_X), RAND.Range(MIN_Y, MAX_Y))));
            m_nextPowerupTime = GetTime() + RAND.Range(MIN_POWERUP_SPAWN_TIME, MAX_POWERUP_SPAWN_TIME);
        }
    }


    void Game::Update()
    {
        if (!IsActive())
        {
            return;
        }

        // If the game is paused or has ended
        if (IsPaused() || HasGameEnded())
        {
            m_timer.Update();
            return;
        }

        // A wave is active, update the entities
        for (EntityList::iterator iter = m_entities.begin(); iter != m_entities.end(); ++iter)
        {
            (*iter)->Update();

            // Check if the entity hit the player
            if ((*iter)->IsSolid()) {
                if ((*iter)->GetBounds().Intersects(m_player.GetBounds()))
                {
                    (*iter)->OnCollide();
                }
            }

            // If the entity is finished, remove it from the list
            if ((*iter)->Unlink())
            {
                // Check if the entity we've just unlinked was the current target
                if ((*iter) == m_targetEnt.lock()) {
                    m_targetEnt.reset();
                }

                iter = m_entities.erase(iter);

                // If we've erased the last element in the list, we need to stop here.
                if (iter == m_entities.end())
                {
                    break;
                }
            }
        }

        for (EffectList::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter)
        {
            (*iter)->Update();

            if ((*iter)->Unlink())
            {
                iter = m_effects.erase(iter);

                if (iter == m_effects.end())
                {
                    break;
                }
            }
        }

        SpawnEnemies();
        SpawnPowerups();

        m_player.Update();

        m_timer.Update();
    }


    void Game::DrawHud()
    {
        const float HUD_SIZE                 = 48.0f;
        const float HUD_TEXT_HEIGHT          = 16.0f;
        const float HUD_NUMBER_HEIGHT        = 32.0f;
        const float HUD_WARNING_HEIGHT       = 48.0f;
        const float HUD_WARNING_BLINK_SPEED  = 4.0f;
        const float HUD_BOSS_APPROACH_HEIGHT = 16.0f;
        const float HUD_LIVES_X              = ORTHO_WIDTH / 4.0f;
        const float HUD_SCORE_X              = ORTHO_WIDTH / 2.0f;
        const float HUD_STREAK_X             = (ORTHO_WIDTH / 4.0f) * 3.0f;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, ORTHO_WIDTH, ORTHO_HEIGHT, 0.0, -1.0, 1.0);

#ifdef _DEBUG
        const float debug_height = 16.0f;
        float debug_y = 0; 
        FONTS.Print(HUD_FONT, 0.0f, debug_y, debug_height, ColourRGBA::White(), Font::ALIGN_LEFT,
            (boost::format("F:%1%, P:%2%") % m_flow % static_cast<int>(GetProgress() * 100)).str());
        debug_y += debug_height;
        m_phrases.DrawChars(HUD_FONT, debug_y, debug_height);
        debug_y += debug_height;
        FONTS.Print(HUD_FONT, 0.0f, debug_y, debug_height, ColourRGBA::White(), Font::ALIGN_LEFT,
            (boost::format("Pup: %1%") % (m_nextPowerupTime - GetTime())).str());
#endif

        glDisable(GL_TEXTURE_2D);

        glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
        glBegin(GL_TRIANGLES);
            glVertex2f(0.0f,                   ORTHO_HEIGHT);
            glVertex2f(HUD_SIZE,               ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(HUD_SIZE,               ORTHO_HEIGHT);

            glVertex2f(HUD_SIZE,               ORTHO_HEIGHT);
            glVertex2f(HUD_SIZE,               ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(ORTHO_WIDTH - HUD_SIZE, ORTHO_HEIGHT);

            glVertex2f(HUD_SIZE,               ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(ORTHO_WIDTH - HUD_SIZE, ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(ORTHO_WIDTH - HUD_SIZE, ORTHO_HEIGHT);

            glVertex2f(ORTHO_WIDTH - HUD_SIZE, ORTHO_HEIGHT);
            glVertex2f(ORTHO_WIDTH - HUD_SIZE, ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(ORTHO_WIDTH,            ORTHO_HEIGHT);
        glEnd();

        glLineWidth(1.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(0.0f,                   ORTHO_HEIGHT);
            glVertex2f(HUD_SIZE,               ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(ORTHO_WIDTH - HUD_SIZE, ORTHO_HEIGHT - HUD_SIZE);
            glVertex2f(ORTHO_WIDTH,            ORTHO_HEIGHT);
        glEnd();
        glLineWidth(1.0f);

        glEnable(GL_TEXTURE_2D);

        FONTS.Print(HUD_FONT, HUD_LIVES_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT - HUD_TEXT_HEIGHT, HUD_TEXT_HEIGHT,
            ColourRGBA::White(), Font::ALIGN_CENTER, "LIVES");
        FONTS.Print(HUD_FONT, HUD_LIVES_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT, HUD_NUMBER_HEIGHT,
            ColourRGBA::White(), Font::ALIGN_CENTER, boost::lexical_cast<std::string>(m_lives));

        FONTS.Print(HUD_FONT, HUD_SCORE_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT - HUD_TEXT_HEIGHT, HUD_TEXT_HEIGHT,
            ColourRGBA::White(), Font::ALIGN_CENTER, "SCORE");
        FONTS.Print(HUD_FONT, HUD_SCORE_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT, HUD_NUMBER_HEIGHT,
            ColourRGBA::White(), Font::ALIGN_CENTER, boost::lexical_cast<std::string>(m_score));

        FONTS.Print(HUD_FONT, HUD_STREAK_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT - HUD_TEXT_HEIGHT, HUD_TEXT_HEIGHT,
            ColourRGBA::White(), Font::ALIGN_CENTER, "STREAK");
        FONTS.Print(HUD_FONT, HUD_STREAK_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT, HUD_NUMBER_HEIGHT,
            ColourRGBA::White(), Font::ALIGN_CENTER, boost::lexical_cast<std::string>(m_streak));

        if (m_bossWavePending) {
            const float bossPendingTime = m_bossWaveStartTime - GetTime();
            const float warningAlpha = abs(sinf(static_cast<float>(M_PI) * (bossPendingTime / BOSS_WAVE_WAIT_TIME) * HUD_WARNING_BLINK_SPEED));
            ColourRGBA warningColour(ColourRGB::Red(), warningAlpha);

            FONTS.Print(HUD_FONT, ORTHO_WIDTH / 2.0f, 0, HUD_WARNING_HEIGHT,
                warningColour, Font::ALIGN_CENTER, "WARNING");
            FONTS.Print(HUD_FONT, ORTHO_WIDTH / 2.0f, HUD_WARNING_HEIGHT, HUD_BOSS_APPROACH_HEIGHT,
                warningColour, Font::ALIGN_CENTER, "BOSS APPROACHING");
        }
    }


    void Game::DrawBackground()
    {
        const float BACKGROUND_RING_WIDTH   = 150.0f;
        const float BACKGROUND_RING_COUNT   = 30.0f;
        const float BACKGROUND_START_DIST   = 50.0f;
        const float BACKGROUND_Z_COORD      = -50.0f;
        const float BACKGROUND_COS_THETA    = 0.866025f;
        const float BACKGROUND_SIN_THETA    = 0.5f;
        const float BACKGROUND_CURVE_FACTOR = 0.005f;

        glDisable(GL_TEXTURE_2D);
        glColor4f(0.1f, 0.1f, 0.1f, 1.0f);

        for (int i = 0; i < BACKGROUND_RING_COUNT; i++) {
            const float innerDist = BACKGROUND_START_DIST + i * BACKGROUND_RING_WIDTH;
            const float outerDist = BACKGROUND_START_DIST + (i + 1) * BACKGROUND_RING_WIDTH;

            const float innerZ = BACKGROUND_Z_COORD -
                (innerDist - BACKGROUND_START_DIST) * (innerDist - BACKGROUND_START_DIST) *
                BACKGROUND_CURVE_FACTOR * BACKGROUND_CURVE_FACTOR;
            const float outerZ = BACKGROUND_Z_COORD -
                (outerDist - BACKGROUND_START_DIST) * (outerDist - BACKGROUND_START_DIST) *
                BACKGROUND_CURVE_FACTOR * BACKGROUND_CURVE_FACTOR;

            const float red   = 0.5f / (1.0f + i * 0.3f);
            const float green = 0.2f / (1.0f + i * 0.3f);
            glColor3f(red, green, 0.0f);

            glBegin(GL_QUADS);
            glVertex3f(innerDist, 0.0f, innerZ);
            glVertex3f(outerDist, 0.0f, outerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);

            glVertex3f(innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);

            glVertex3f(innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(0.0f, outerDist, outerZ);
            glVertex3f(0.0f, innerDist, innerZ);

            glVertex3f(0.0f, innerDist, innerZ);
            glVertex3f(0.0f, outerDist, outerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);

            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);

            glVertex3f(-innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-outerDist, 0.0f, outerZ);
            glVertex3f(-innerDist, 0.0f, innerZ);

            glVertex3f(-innerDist, 0.0f, innerZ);
            glVertex3f(-outerDist, 0.0f, outerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);

            glVertex3f(-innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);

            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(0.0f, -outerDist, outerZ);
            glVertex3f(0.0f, -innerDist, innerZ);

            glVertex3f(0.0f, -innerDist, innerZ);
            glVertex3f(0.0f, -outerDist, outerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);

            glVertex3f(innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);

            glVertex3f(innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(outerDist, 0.0f, outerZ);
            glVertex3f(innerDist, 0.0f, innerZ);
            glEnd();

            glColor3f(0.1f, 0.1f, 0.1f);
            glBegin(GL_LINES);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(innerDist, 0.0f, innerZ);
            glVertex3f(innerDist, 0.0f, innerZ);
            glVertex3f(outerDist, 0.0f, outerZ);
            glVertex3f(outerDist, 0.0f, outerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);

            glVertex3f(innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);

            glVertex3f(0.0f, innerDist, innerZ);
            glVertex3f(innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(0.0f, outerDist, outerZ);

            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(0.0f, innerDist, innerZ);
            glVertex3f(0.0f, innerDist, innerZ);
            glVertex3f(0.0f, outerDist, outerZ);
            glVertex3f(0.0f, outerDist, outerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);

            glVertex3f(-innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);

            glVertex3f(-innerDist, 0.0f, innerZ);
            glVertex3f(-innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-innerDist * BACKGROUND_COS_THETA, innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-outerDist, 0.0f, outerZ);

            glVertex3f(-innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-innerDist, 0.0f, innerZ);
            glVertex3f(-innerDist, 0.0f, innerZ);
            glVertex3f(-outerDist, 0.0f, outerZ);
            glVertex3f(-outerDist, 0.0f, outerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);

            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);

            glVertex3f(0.0f, -innerDist, innerZ);
            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(-outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(0.0f, -outerDist, outerZ);

            glVertex3f(innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(0.0f, -innerDist, innerZ);
            glVertex3f(0.0f, -innerDist, innerZ);
            glVertex3f(0.0f, -outerDist, outerZ);
            glVertex3f(0.0f, -outerDist, outerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);

            glVertex3f(innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(innerDist * BACKGROUND_SIN_THETA, -innerDist * BACKGROUND_COS_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_SIN_THETA, -outerDist * BACKGROUND_COS_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);

            glVertex3f(innerDist, 0.0f, innerZ);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(innerDist * BACKGROUND_COS_THETA, -innerDist * BACKGROUND_SIN_THETA, innerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(outerDist * BACKGROUND_COS_THETA, -outerDist * BACKGROUND_SIN_THETA, outerZ);
            glVertex3f(outerDist, 0.0f, outerZ);
            glEnd();
        }

        glEnable(GL_TEXTURE_2D);
    }


    void Game::DrawEndScreen()
    {
        const float GAME_OVER_HEIGHT = 64.0f;
        const float SCORE_HEIGHT = 48.0f;
        const float ITEM_SPACING = 8.0f;
        const float CONTINUE_HEIGHT = 24.0f;

        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), 0.0f, 0.0f, ORTHO_WIDTH, ORTHO_HEIGHT);

        const float x = ORTHO_WIDTH / 2.0f;
        float y = (ORTHO_HEIGHT / 2.0f) - (SCORE_HEIGHT / 2.0f) - (GAME_OVER_HEIGHT + ITEM_SPACING);

        FONTS.Print(ENDGAME_FONT, x, y, GAME_OVER_HEIGHT, ColourRGBA::Red(), Font::ALIGN_CENTER,
            "Game Over!");
        y+= GAME_OVER_HEIGHT + ITEM_SPACING;
        FONTS.Print(ENDGAME_FONT, x, y, SCORE_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
            (boost::format("Score - %1%") % m_score).str());

        if (m_timer.GetTime() - END_GAME_SCREEN_PAUSE > m_gameEndTime)
        {
            y = ORTHO_HEIGHT - CONTINUE_HEIGHT;
            FONTS.Print(ENDGAME_FONT, x, y, CONTINUE_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                        "Press any key to continue...");
        }
    }


    void Game::Draw ()
    {
        if (IsActive())
        {
            // Set up the 3D camera for drawing the world
            m_camera.ApplyPerspective();

            DrawBackground();

            if (IsAlive())
            {
                m_player.Draw();

                // If we have a current target, draw a targetting line from the player
                // to the target.
                EntityPtr ent = m_targetEnt.lock();
                if (ent)
                {
                    glDisable(GL_TEXTURE_2D);
                    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);

                    const juzutil::Vector3& playerOrg = m_player.GetOrigin();
                    const juzutil::Vector3& targetOrg = ent->GetOrigin();

                    glBegin(GL_LINES);
                    glVertex3f(playerOrg[0], playerOrg[1], playerOrg[2]);
                    glVertex3f(targetOrg[0], targetOrg[1], targetOrg[2]);
                    glEnd();

                    glEnable(GL_TEXTURE_2D);
                }
            }

            for_each(m_entities.begin(), m_entities.end(), boost::mem_fn(&Entity::Draw3D));
            // TODO: one effects list with Draw2D and Draw3D, like ent
            for_each(m_effects.begin(), m_effects.end(), boost::mem_fn(&Effect::Draw));

            // Use an orthographic projection for drawing the phrases as we want the text
            // to appear the same size no matter where it is being drawn.
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, ORTHO_WIDTH, ORTHO_HEIGHT, 0.0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            for_each(m_entities.begin(), m_entities.end(), boost::mem_fn(&Entity::Draw2D));
            for_each(m_effects2d.begin(), m_effects2d.end(), boost::mem_fn(&Effect::Draw));

            if (!HasGameEnded())
            {
                DrawHud();
            }
            else
            {
                DrawEndScreen();
            }
        }
    }


    // When we've finished a phrase, decides what award to give to the player,
    // and adds it to the list to be displayed on screen.
    // The speeds are measured in chars typed per second
    void Game::PhraseFinished(EntityPtr &ent)
    {
        const float  EXCELLENT_SPEED      = 0.1f;
        const float  EXCELLENT_MULTIPLIER = 4.0f;
        const float  GOOD_SPEED           = 0.2f;
        const float  GOOD_MULTIPLIER      = 2.0f;
        const float  OK_SPEED             = 0.3f;
        const float  POOR_SPEED           = 0.5f;
        const float  POOR_MULTIPLIER      = 0.5f;
        const float  BAD_MULTIPLIER       = 0.25f;
        const float  AWARD_OFFSET         = -20.0f;
        unsigned int EXCELLENT_FLOW       = 10;
        unsigned int GOOD_FLOW            = 8;
        unsigned int OK_FLOW              = 5;
        unsigned int POOR_FLOW            = 2;
        unsigned int BAD_FLOW             = 1;

        unsigned int score = ent->GetScore();

        if (m_streakValid) {
            m_streak++;
            m_maxStreak = std::max(m_maxStreak, m_streak);
        }

        LaserPtr laser(new Laser(GetPlayerOrigin(), ent->GetOrigin(), ColourRGB::White()));
        AddEffect(laser);
        m_player.Fire();

        // Don't give an award or increase the flow if this was only a single-char entity.
        if (!ent->IsPhraseSingle()) {
            AwardType type;
            unsigned int flow;
            float speed = ent->GetTypingSpeed();

            if (speed <= EXCELLENT_SPEED) {
                type = AWARD_EXCELLENT;
                flow = EXCELLENT_FLOW;
                score = static_cast<unsigned int>(static_cast<float>(score) * EXCELLENT_MULTIPLIER);
                m_excellents++;
            } else if (speed <= GOOD_SPEED) {
                type = AWARD_GOOD;
                flow = GOOD_FLOW;
                score = static_cast<unsigned int>(static_cast<float>(score) * GOOD_MULTIPLIER);
                m_goods++;
            } else if (speed <= OK_SPEED) {
                type = AWARD_OK;
                flow = OK_FLOW;
                m_oks++;
            } else if (speed <= POOR_SPEED) {
                type = AWARD_POOR;
                flow = POOR_FLOW;
                score = static_cast<unsigned int>(static_cast<float>(score) * POOR_MULTIPLIER);
                m_poors++;
            } else {
                type = AWARD_BAD;
                flow = BAD_FLOW;
                score = static_cast<unsigned int>(static_cast<float>(score) * BAD_MULTIPLIER);
                m_bads++;
            }

            // Display the award on screen.
            if (!ent->SuppressAwardDisplay()) {
                juzutil::Vector2 screenOrg = m_camera.PerspectiveProject(ent->GetOrigin());
                screenOrg[1] += AWARD_OFFSET;
                AwardPtr award(new Award(screenOrg, type, GetTime()));
                AddEffect2d(award);
            }

            // Calculate the new flow value
            m_flow -= m_flowContribs[m_oldestFlowContribIndex];
            m_flowContribs[m_oldestFlowContribIndex] = flow;

            if (m_oldestFlowContribIndex == 0) {
                m_oldestFlowContribIndex = FLOW_CONTRIB_COUNT - 1;
            } else {
                m_oldestFlowContribIndex--;
            }

            m_flow += flow;
        }

        score   *= (m_streak == 0 ? 1 : m_streak);
        m_score += score;
    }


    void Game::OnType (const SDL_keysym sym)
    {
        bool miss = false;

        if (!IsActive())
        {
            return;
        }

        // If the high 9 bits of the unicode are 0, this is an ascii char
        // and we can work with it.
        char c   = 0;
        if (sym.unicode != 0 && (sym.unicode & 0xFF80) == 0) {
            c = sym.unicode & 0x7F;
        }

        if (HasGameEnded())
        {
            // If we are dead, and we have paused for long enough, go back to the main menu
            // on any keypress.
            if (GetTime() - END_GAME_SCREEN_PAUSE > m_gameEndTime)
            {
                Activate(false);

                if (SCORES.IsHighScore(m_score))
                {
                    MENU.Activate(NewHighScoreMenu::MENU_NAME);
                }
                else
                {
                    MENU.Activate(MainMenu::MENU_NAME);
                }
            }
        }
        else if (!IsPaused() && sym.sym == SDLK_ESCAPE)
        {
            // Escape pressed, pause and bring up the in-game menu
            Pause(true);
            MENU.Activate(PauseMenu::MENU_NAME);
        }
        else if (IsAlive() && !HasGameEnded() && !IsPaused() && sym.sym != SDLK_RETURN && c != 0)
        {
            EntityPtr ent = m_targetEnt.lock();
            if (!ent)
            {
                // There is no target entity, look for a entity starting with the letter
                // the player has typed.
                EntityList::iterator target =
                    find_if(m_entities.begin(), m_entities.end(),
                            boost::bind(&Entity::StartsWith, _1, c));

                if (target == m_entities.end())
                {
                    // We didn't find any targets starting with the letter the player typed,
                    // this is a miss.
                    miss = true;
                } else {
                    // We've got a new target.
                    m_targetEnt  = *target;
                    ent          = m_targetEnt.lock();

                    // Don't want to play the target sound if this entity's phrase is a
                    // single letter, as we are killing it immediately.
                    if (!ent->IsPhraseSingle())
                    {
                        SOUNDS.Play(TARGET_SOUND);
                    }

                    m_streakValid = true;
                }
            }

            if (ent)
            {
                bool finished;
                bool hit;
                ent->OnType(c, &hit, &finished);
                miss = !hit;

                if (finished)
                {
                    // We can miss and still finish the enemy (for example a BombEnemy dies on a miss),
                    // so only display awards etc. if we hit.
                    if (hit) {
					    PhraseFinished(ent);
                    }

                    m_targetEnt.reset();
                }
            }

            if (miss)
            {
                m_misses++;
                m_streakValid = false;
                m_streak      = 0;
                SOUNDS.Play(MISS_SOUND);
            }
            else
            {
                m_hits++;
            }
        }
    }

    void Game::Damage()
    {
        m_player.Damage();

        if (!m_invincible && m_lives > 0)
        {
            m_lives--;
            m_usedLives++;

            if (m_lives == 0)
            {
                ExplosionPtr explosion(new Explosion(GetPlayerOrigin(), ColourRGBA::Red()));
                AddEffect(explosion);

                EndGame(FINAL_DEATH_PAUSE);
            }
        }
    }


    void Game::EndGame(float pause)
    {
        const float MUSIC_FADE_OUT_TIME = 2.0f;

        m_lives = 0;
        m_gameEndTime = GetTime() + pause;

        Mix_FadeOutMusic(static_cast<int>((pause + MUSIC_FADE_OUT_TIME) * 1000));
    }
}
