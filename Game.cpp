#include <ctime>
#include <functional>
#include <random>
#include <string>
#include <boost/format.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace typing
{
    // The length of time, in seconds, between levels.
    static const float LEVEL_TIME = 60.0f;

    // The length of time, in seconds, between waves.
    static const float WAVE_INTERVAL_BASE = 10.0f;

    // The amount of that the wave interval decreases by each level.
    static const float WAVE_INTERVAL_SCALE = 1.0f;

    // The minimum interval between waves.
    static const float WAVE_INTERVAL_MIN = 4.0f;

    // The length of time to pause before starting a wave at the start of a
    // game.
    static const float GAME_START_WAVE_PAUSE = 1.0f;

    // The length of time to wait for the next wave if the player kills all
    // active waves.
    static const float WAVES_CLEARED_PAUSE = 0.5f; 

    // The length of time to pause on reaching the end of the level before
    // the boss appears.
    static const float BOSS_WAVE_WAIT_TIME = 5.0f;

    // The length of time the flash appears on the screen when the player is
    // damaged.
    static const float DAMAGE_FLASH_TIME = 0.5f;

    // The maximum streak that counts towards the score multiplier.
    static const unsigned int MAX_COMBO = 4;

    const float       Game::GAME_SCREEN_TOP = 1000.0f;
    const float       Game::GAME_SCREEN_BOTTOM = -100.0f;
    const float       Game::GAME_SCREEN_LEFT = -775.0f;
    const float       Game::GAME_SCREEN_RIGHT = 775.0f;
    const float       Game::FINAL_DEATH_PAUSE = 2.0f;
    const float       Game::MIN_POWERUP_SPAWN_TIME = 30.0f;
    const float       Game::MAX_POWERUP_SPAWN_TIME = 180.0f;
    const float       Game::ORTHO_WIDTH       = 800.0f;
    const float       Game::ORTHO_HEIGHT      = 600.0f;
    const std::string Game::HUD_FONT("fonts/hudfont.fnt");
    const std::string Game::ENDGAME_FONT("fonts/menufont.fnt");
    const std::string Game::MISS_SOUND("sounds/miss.wav");
    const std::string Game::TARGET_SOUND("sounds/target.wav");
    const std::string Game::GAME_MUSIC("music/music.ogg");

    std::auto_ptr<Game> Game::m_singleton(new Game);
    Game& Game::GetGame ()
    {
        return *(m_singleton.get());
    }

    Game::Game()
        : m_camera(juzutil::Vector3(0.0f, -200.0f, 500.0f),
                   juzutil::Vector3(0.0f, 200.0f, 0.0f)),
          m_active(false)
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
        if (!m_music) {
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

        // Initialise the wave creator, setting the minimum level that each
        // wave can be used at.
        m_waveCreator.AddWave<BasicEnemyWave>(0);
        m_waveCreator.AddWave<MissileEnemyWave>(0);
        m_waveCreator.AddWave<AccelEnemyWave>(1);
        m_waveCreator.AddWave<BombEnemyWave>(2);
        m_waveCreator.AddWave<SeekerEnemyWave>(3);

        m_bossWaveCreator.AddWave<KnockbackBossEnemyWave>();
        m_bossWaveCreator.AddWave<MissileBossEnemyWave>();
        m_bossWaveCreator.AddWave<ChargeBossEnemyWave>();

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
        m_activeWaves.clear();
        m_lives = GAME_START_LIVES;

        Pause(false);
        Activate(true);

        m_player.Reset();
        m_timer.Reset();

        m_level         = 0; 
        m_nextLevelTime = LEVEL_TIME;
        m_gameEndTime   = 0.0f;
        m_score         = 0;
        m_streakValid   = false;
        m_streak        = 0;
        m_damageTime    = 0.0f;

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

        m_targetEnt.reset();

        m_nextWaveTime      = GAME_START_WAVE_PAUSE;
        m_bossWavePending   = false;
        m_bossWaveActive    = false;
        m_bossWaveStartTime = 0.0f;

        m_nextPowerupTime = RAND.Range(MIN_POWERUP_SPAWN_TIME,
                                       MAX_POWERUP_SPAWN_TIME);

        Mix_FadeInMusic(m_music, -1, MUSIC_FADE_IN_TIME);

        APP.Log(App::LOG_DEBUG, "Starting new game");
    }


    void Game::SpawnEnemies()
    {
        // Work out whether we have waited long enough to start a new wave.
        // Don't spawn anything if a boss wave is pending or in progress.
        if (GetTime() >= m_nextWaveTime &&
            !m_bossWavePending && !m_bossWaveActive) {
            EnemyWavePtr wave = m_waveCreator.CreateWave(m_level);
            wave->Start();
            m_activeWaves.push_back(wave);

            m_nextWaveTime =
                GetTime() + std::max(WAVE_INTERVAL_MIN,
                                     WAVE_INTERVAL_BASE -
                                                WAVE_INTERVAL_SCALE * m_level);
            APP.Log(App::LOG_DEBUG,
                    boost::str(boost::format(
                        "%1%: Spawned new wave. Next wave at %2%")
                        % GetTime() % m_nextWaveTime));
        }

        // Check if we should start a boss wave.
        if (m_bossWavePending && m_activeWaves.size() == 0 &&
            GetTime() >= m_bossWaveStartTime) {
            EnemyWavePtr wave = m_bossWaveCreator.CreateWave();
            wave->Start();
            m_activeWaves.push_back(wave);
            m_bossWavePending = false;
            m_bossWaveActive = true;

            APP.Log(App::LOG_DEBUG,
                    boost::str(boost::format(
                        "%1%: Spawned boss wave.") % GetTime()));
        }

        // If we're waiting for a boss and there are still active waves, push
        // the boss spawn time back.
        if (m_bossWavePending && m_activeWaves.size() > 0) {
            m_bossWaveStartTime = GetTime() + BOSS_WAVE_WAIT_TIME;
        }

        // Spawn enemies from any active waves, remove any finished waves.
        for (WaveVec::iterator iter = m_activeWaves.begin();
             iter != m_activeWaves.end();) {
            (*iter)->Spawn();
            
            if ((*iter)->IsFinished()) {
                APP.Log(App::LOG_DEBUG,
                        boost::str(boost::format(
                            "%1%: Wave finished.") % GetTime()));
                iter = m_activeWaves.erase(iter);

                // Assume that if a boss wave is in progress, that this is
                // the only wave active.
                if (m_bossWaveActive) {
                    m_bossWaveActive = false;
                    m_level++;
                    m_nextLevelTime = GetTime() + LEVEL_TIME;
                }
            } else {
                ++iter;
            }
        }

        // If there are no active waves, bring the next wave time forward.
        // Don't do this if the boss is pending, or we're waiting to spawn
        // the first ever wave.
        if (!m_bossWavePending && m_activeWaves.size() == 0 &&
            GAME.GetTime() > GAME_START_WAVE_PAUSE &&
            m_nextWaveTime - GAME.GetTime() > WAVES_CLEARED_PAUSE) {
            m_nextWaveTime = GAME.GetTime() + WAVES_CLEARED_PAUSE;
            APP.Log(App::LOG_DEBUG,
                    boost::str(boost::format(
                        "%1%: All waves finished, spawning next wave at %2%")
                        % GetTime() % m_nextWaveTime));
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

        // The game is active, update the entities.
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

        if (!m_bossWavePending && !m_bossWaveActive &&
            GetTime() > m_nextLevelTime) {
            // Ready to go up to the next level, but need to spawn a boss
            // first.
            m_bossWavePending = true;
            m_bossWaveStartTime = GetTime() + BOSS_WAVE_WAIT_TIME;
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
        m_phrases.DrawChars(HUD_FONT, debug_y, debug_height);
        debug_y += debug_height;
        FONTS.Print(
            HUD_FONT, 0.0f, debug_y, debug_height,
            ColourRGBA::White(), Font::ALIGN_LEFT,
            (boost::format("Level: %1%") % m_level).str());
        debug_y += debug_height;
        FONTS.Print(
            HUD_FONT, 0.0f, debug_y, debug_height,
            ColourRGBA::White(), Font::ALIGN_LEFT,
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

        if (m_usedLives && (GetTime() - m_damageTime) < DAMAGE_FLASH_TIME) {
            glColor4f(1.0f, 1.0f, 1.0f,
                      1.0f - ((GetTime() - m_damageTime) / DAMAGE_FLASH_TIME));
            glBegin(GL_QUADS);
                glVertex2f(0.0f, ORTHO_HEIGHT);
                glVertex2f(ORTHO_WIDTH, ORTHO_HEIGHT);
                glVertex2f(ORTHO_WIDTH, 0.0f);
                glVertex2f(0.0f, 0.0f);
            glEnd();
        }

        glEnable(GL_TEXTURE_2D);

        FONTS.Print(HUD_FONT, HUD_LIVES_X,
                    ORTHO_HEIGHT - HUD_NUMBER_HEIGHT - HUD_TEXT_HEIGHT,
                    HUD_TEXT_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    "LIVES");
        FONTS.Print(HUD_FONT, HUD_LIVES_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT,
                    HUD_NUMBER_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    std::to_string(m_lives));
        FONTS.Print(HUD_FONT, HUD_SCORE_X,
                    ORTHO_HEIGHT - HUD_NUMBER_HEIGHT - HUD_TEXT_HEIGHT,
                    HUD_TEXT_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    "SCORE");
        FONTS.Print(HUD_FONT, HUD_SCORE_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT,
                    HUD_NUMBER_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    std::to_string(m_score));
        FONTS.Print(HUD_FONT, HUD_STREAK_X,
                    ORTHO_HEIGHT - HUD_NUMBER_HEIGHT - HUD_TEXT_HEIGHT,
                    HUD_TEXT_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    "STREAK");
        FONTS.Print(HUD_FONT, HUD_STREAK_X, ORTHO_HEIGHT - HUD_NUMBER_HEIGHT,
                    HUD_NUMBER_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    std::to_string(m_streak));

        if (m_bossWavePending && m_activeWaves.size() == 0) {
            const float bossPendingTime = m_bossWaveStartTime - GetTime();
            const float warningAlpha =
                            fabs(sinf(static_cast<float>(M_PI) *
                                     (bossPendingTime / BOSS_WAVE_WAIT_TIME) *
                                     HUD_WARNING_BLINK_SPEED));
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
        const float BACKGROUND_CURVE_FACTOR = 0.0006f;

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

                // If we have a current target, draw a targetting line from the
                // player to the target.
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

            for_each(m_entities.begin(),
                     m_entities.end(),
                     std::mem_fn(&Entity::Draw3D));
            for_each(m_effects.begin(),
                     m_effects.end(),
                     std::mem_fn(&Effect::Draw));

            // Use an orthographic projection for drawing the phrases as we
            // want the text to appear the same size no matter where it is
            // being drawn.
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, ORTHO_WIDTH, ORTHO_HEIGHT, 0.0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            for_each(m_entities.begin(),
                     m_entities.end(),
                     std::mem_fn(&Entity::Draw2D));
            for_each(m_effects2d.begin(),
                     m_effects2d.end(),
                     std::mem_fn(&Effect::Draw));

            if (!HasGameEnded()) {
                DrawHud();
            } else {
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
        const float  EXCELLENT_MULTIPLIER = 2.0f;
        const float  GOOD_SPEED           = 0.2f;
        const float  GOOD_MULTIPLIER      = 1.4f;
        const float  OK_SPEED             = 0.3f;
        const float  OK_MULTIPLIER        = 1.0f;
        const float  POOR_SPEED           = 0.5f;
        const float  POOR_MULTIPLIER      = 0.5f;
        const float  BAD_MULTIPLIER       = 0.25f;
        const float  AWARD_OFFSET         = -20.0f;
        
        // The score for the entity that has been finished is multiplied by
        // a factor depending on the speed that the phrase was finished, and
        // the the current streak. The streak multiplier is capped at
        // MAX_COMBO.
        float  multiplier = 1.0f;

        if (m_streakValid) {
            m_streak++;
            m_maxStreak = std::max(m_maxStreak, m_streak);
            multiplier = std::max(1U, std::min(m_streak, MAX_COMBO));
        }

        LaserPtr laser(new Laser(GetPlayerOrigin(),
                                 ent->GetOrigin(),
                                 ColourRGB::White()));
        AddEffect(laser);
        m_player.Fire();

        // Don't give an award if this was only a single-char entity.
        if (!ent->IsPhraseSingle()) {
            AwardType type;
            float speed = ent->GetTypingSpeed();

            if (speed <= EXCELLENT_SPEED) {
                type = AWARD_EXCELLENT;
                multiplier *= EXCELLENT_MULTIPLIER;
                m_excellents++;
            } else if (speed <= GOOD_SPEED) {
                type = AWARD_GOOD;
                multiplier *= GOOD_MULTIPLIER;
                m_goods++;
            } else if (speed <= OK_SPEED) {
                type = AWARD_OK;
                multiplier *= OK_MULTIPLIER;
                m_oks++;
            } else if (speed <= POOR_SPEED) {
                type = AWARD_POOR;
                multiplier *= POOR_MULTIPLIER;
                m_poors++;
            } else {
                type = AWARD_BAD;
                multiplier *= BAD_MULTIPLIER;
                m_bads++;
            }

            // Display the award on screen.
            if (!ent->SuppressAwardDisplay()) {
                juzutil::Vector2 screenOrg = m_camera.PerspectiveProject(
                                                            ent->GetOrigin());
                screenOrg[1] += AWARD_OFFSET;
                AwardPtr award(new Award(screenOrg, type, GetTime()));
                AddEffect2d(award);
            }
        }

        m_score += static_cast<float>(ent->GetScore()) * multiplier;
    }


    void Game::OnKeyDown (SDL_Keycode keycode)
    {
        if (HasGameEnded()) {
            // If we are dead, and we have paused for long enough,
            // go back to the main menu on any keypress.
            if (GetTime() - END_GAME_SCREEN_PAUSE > m_gameEndTime) {
                Activate(false);

                if (SCORES.IsHighScore(m_score)) {
                    MENU.Activate(NewHighScoreMenu::MENU_NAME);
                } else {
                    MENU.Activate(MainMenu::MENU_NAME);
                }
            }
        } else if (!IsPaused() && keycode == SDLK_ESCAPE) {
            // Escape pressed, pause and bring up the in-game menu
            Pause(true);
            MENU.Activate(PauseMenu::MENU_NAME);
        } 
    }


    void Game::OnType (char c)
    {
        using namespace std::placeholders;

        bool miss = false;

        if (!IsActive()) {
            return;
        }

        if (IsAlive() && !HasGameEnded() && !IsPaused()) {            
            EntityPtr ent = m_targetEnt.lock();
            if (!ent) {
                // There is no target entity, look for a entity starting
                // with the letter the player has typed.
                EntityList::iterator target =
                    find_if(m_entities.begin(), m_entities.end(),
                            std::bind(&Entity::StartsWith, _1, c));

                if (target == m_entities.end()) {
                    // We didn't find any targets starting with the letter the
                    // player typed, this is a miss.
                    miss = true;
                } else {
                    // We've got a new target.
                    m_targetEnt  = *target;
                    ent          = m_targetEnt.lock();

                    // Don't want to play the target sound if this entity's
                    // phrase is a single letter, as we are killing it
                    // immediately.
                    if (!ent->IsPhraseSingle()) {
                        SOUNDS.Play(TARGET_SOUND);
                    }

                    m_streakValid = true;
                }
            }

            if (ent) {
                bool finished;
                bool hit;
                ent->OnType(c, &hit, &finished);
                miss = !hit;

                if (finished) {
                    // We can miss and still finish the enemy (for example a
                    // BombEnemy dies on a miss), so only display awards etc.
                    // if we hit.
                    if (hit) {
                        PhraseFinished(ent);
                    }

                    m_targetEnt.reset();
                }
            }

            if (miss) {
                m_misses++;
                m_streakValid = false;
                m_streak      = 0;
                SOUNDS.Play(MISS_SOUND);
            } else {
                m_hits++;
            }
        }
    }

    void Game::Damage()
    {
        m_player.Damage();

        if (m_lives > 0) {
            m_lives--;
            m_usedLives++;
            m_damageTime = GetTime();

            for_each(m_entities.begin(),
                     m_entities.end(),
                     std::mem_fn(&Entity::OnPlayerDie));

            if (m_lives == 0) {
                ExplosionPtr explosion(
                    new Explosion(GetPlayerOrigin(), ColourRGBA::Red()));
                AddEffect(explosion);
                EndGame(FINAL_DEATH_PAUSE);
            }
        }
    }


    void Game::EndGame(float pause)
    {
        const float MUSIC_FADE_OUT_TIME = 2.0f;
        m_gameEndTime = GetTime() + pause;
        Mix_FadeOutMusic(static_cast<int>((pause + MUSIC_FADE_OUT_TIME) * 1000));
    }
}
