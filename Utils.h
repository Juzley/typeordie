#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <random>
#include "Vector.h"
#include "Matrix.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class ColourRGBA;

namespace typing
{
    // Drawing utility funcs
    void DrawTexturedRect(const std::string& texture, float x, float y, float width, float height);
    void DrawTexturedRect(const std::string& texture, const ColourRGBA& col, float x, float y, float width, float height);
    void DrawRect(ColourRGBA col, float x, float y, float width, float height);
    void DrawLine(const ColourRGBA& col, float startX, float startY, float startZ, float endX, float endY, float endZ);
    void DrawLine(const ColourRGBA& col, const juzutil::Vector3& start, const juzutil::Vector3& end);
    void DrawLine(const ColourRGBA& col, float startX, float startY, float endX, float endY);
    void DrawLine(const ColourRGBA& col, const juzutil::Vector2& start, const juzutil::Vector2& end);

    // Enemy spawn utility funcs
    const juzutil::Vector2 SelectEdgeSpawnPoint(std::mt19937& random);
    const juzutil::Vector2 SelectMidSpawnPoint(std::mt19937& random);
    const juzutil::Vector2 SelectEnemyDirection(const juzutil::Vector3& spawnPoint, float dist);

	// Misc Utils
	static inline float RangeFromRatio(float min, float max, float ratio)
	{
		return ((max - min) * ratio + min);
	}

    static inline int RangeFromRatio(int min, int max, float ratio)
    {
        return (static_cast<int>(RangeFromRatio(static_cast<float>(min), static_cast<float>(max), ratio)));
    }

	static inline float RangeFromRatioInverse(float min, float max, float ratio)
	{
		return (max - (max - min ) * ratio);
	}

    static inline float DegToRad(float deg)
    {
        return (deg * M_PI / 180.0f);
    }

    static inline float RadToDeg(float rad)
    {
        return (rad * 180.0f / M_PI);
    }
}

#endif // __UTILS_H__
