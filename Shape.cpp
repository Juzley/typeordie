#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Shape.h"
#include "Vector.h"
#include "Utils.h"

namespace typing
{
    void DrawPyramid(const ColourRGBA& faceColour, const ColourRGBA& lineColour)
    {
        DrawPyramid(faceColour);

        glDisable(GL_TEXTURE_2D);

        glColor4f(lineColour.GetRed(), lineColour.GetGreen(), lineColour.GetBlue(), lineColour.GetAlpha());
        glBegin(GL_LINE_STRIP);
            glVertex3f(0, 0, 0);
            glVertex3f(1, 1, 1);
            glVertex3f(1, 1, -1);
            glVertex3f(0, 0, 0);
            glVertex3f(-1, 1, -1);
            glVertex3f(-1, 1, 1);
            glVertex3f(0, 0, 0);
        glEnd();

        glBegin(GL_LINES);
            glVertex3f(1, 1, 1);
            glVertex3f(-1, 1, 1);
            glVertex3f(1, 1, -1);
            glVertex3f(-1, 1, -1);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    void DrawPyramid(const ColourRGBA& faceColour)
    {
        glDisable(GL_TEXTURE_2D);

        glColor4f(faceColour.GetRed(), faceColour.GetGreen(), faceColour.GetBlue(), faceColour.GetAlpha());
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f( 0, 0,  0);
            glVertex3f( 1, 1,  1);
            glVertex3i( 1, 1, -1);
            glVertex3i(-1, 1, -1);
            glVertex3i(-1, 1,  1);
            glVertex3i( 1, 1,  1);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    void DrawCube(const ColourRGBA& faceColour, const ColourRGBA& lineColour)
    {
        DrawCube(faceColour);

        glDisable(GL_TEXTURE_2D);

        glColor4f(lineColour.GetRed(), lineColour.GetGreen(), lineColour.GetBlue(), lineColour.GetAlpha());
        // Bottom lines
        glBegin(GL_LINE_LOOP);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);
        glEnd();

        // Top lines
        glBegin(GL_LINE_LOOP);
            glVertex3f(-0.5f,  0.5f, 0.5f);
            glVertex3f( 0.5f,  0.5f, 0.5f);
            glVertex3f( 0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
        glEnd();

        // Side lines
        glBegin(GL_LINES);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);

            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);

            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);

            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    void DrawCube(const ColourRGBA& faceColour)
    {
        glDisable(GL_TEXTURE_2D);

        glColor4f(faceColour.GetRed(), faceColour.GetGreen(), faceColour.GetBlue(), faceColour.GetAlpha());
        glBegin(GL_QUADS);
            // Bottom
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);

            // Top
            glVertex3f(-0.5f,  0.5f, 0.5f);
            glVertex3f( 0.5f,  0.5f, 0.5f);
            glVertex3f( 0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);

            // Left
            glVertex3f(-0.5f, -0.5f,  -0.5f);
            glVertex3f(-0.5f, -0.5f,   0.5f);
            glVertex3f(-0.5f,  0.5f,   0.5f);
            glVertex3f(-0.5f,  0.5f,  -0.5f);

            // Right
            glVertex3f(0.5f,  0.5f,  -0.5f);
            glVertex3f(0.5f,  0.5f,   0.5f);
            glVertex3f(0.5f, -0.5f,   0.5f);
            glVertex3f(0.5f, -0.5f,  -0.5f);

            // Up
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f( 0.5f, 0.5f, -0.5f);
            glVertex3f( 0.5f, 0.5f,  0.5f);
            glVertex3f(-0.5f, 0.5f,  0.5f);

            // Down
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    static void DrawDodecahedronFace(const juzutil::Vector3& p1,
                                     const juzutil::Vector3& p2,
                                     const juzutil::Vector3& p3,
                                     const juzutil::Vector3& p4,
                                     const juzutil::Vector3& p5,
                                     GLenum type)
    {
        glBegin(type);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);
            glVertex3f(p5[0], p5[1], p5[2]);
        glEnd();
    }

    void DrawDodecahedron(const ColourRGBA& faceColour, const ColourRGBA& lineColour)
    {
        const static float PHI = 1.6180339887f;
        const static juzutil::Vector3 points[] = {
            juzutil::Vector3(-1.0f/PHI,  0.0f,      PHI),
            juzutil::Vector3( 1.0f/PHI,  0.0f,      PHI),
            juzutil::Vector3(-1.0f,     -1.0f,     -1.0f),
            juzutil::Vector3(-1.0f,     -1.0f,      1.0f),
            juzutil::Vector3(-1.0f,      1.0f,     -1.0f),
            juzutil::Vector3(-1.0f,      1.0f,      1.0f),
            juzutil::Vector3(1.0f,      -1.0f,     -1.0f),
            juzutil::Vector3(1.0f,      -1.0f,      1.0f),
            juzutil::Vector3(1.0f,       1.0f,     -1.0f),
            juzutil::Vector3(1.0f,       1.0f,      1.0f),
            juzutil::Vector3(PHI,        1.0f/PHI,  0.0f),
            juzutil::Vector3(PHI,       -1.0f/PHI,  0.0f),
            juzutil::Vector3(-PHI,       1.0f/PHI,  0.0f),
            juzutil::Vector3(-PHI,      -1.0f/PHI, -0.0f),
            juzutil::Vector3(-1.0f/PHI,  0.0f,     -PHI),
            juzutil::Vector3(1.0f/PHI,   0.0f,     -PHI),
            juzutil::Vector3(0.0f,       PHI,       1.0f/PHI),
            juzutil::Vector3(0.0f,       PHI,      -1.0f/PHI),
            juzutil::Vector3(0.0f,      -PHI,       1.0f/PHI),
            juzutil::Vector3(0.0f,      -PHI,      -1.0f/PHI),
        };

        glDisable(GL_TEXTURE_2D);

        glColor4f(faceColour.GetRed(), faceColour.GetGreen(), faceColour.GetBlue(), faceColour.GetAlpha());
        DrawDodecahedronFace(points[0],  points[1],  points[9],  points[16], points[5],  GL_POLYGON);
        DrawDodecahedronFace(points[1],  points[0],  points[3],  points[18], points[7],  GL_POLYGON);
        DrawDodecahedronFace(points[1],  points[7],  points[11], points[10], points[9],  GL_POLYGON);
        DrawDodecahedronFace(points[11], points[7],  points[18], points[19], points[6],  GL_POLYGON);
        DrawDodecahedronFace(points[8],  points[17], points[16], points[9],  points[10], GL_POLYGON);
        DrawDodecahedronFace(points[2],  points[14], points[15], points[6],  points[19], GL_POLYGON);
        DrawDodecahedronFace(points[2],  points[13], points[12], points[4],  points[14], GL_POLYGON);
        DrawDodecahedronFace(points[2],  points[19], points[18], points[3],  points[13], GL_POLYGON);
        DrawDodecahedronFace(points[3],  points[0],  points[5],  points[12], points[13], GL_POLYGON);
        DrawDodecahedronFace(points[6],  points[15], points[8],  points[10], points[11], GL_POLYGON);
        DrawDodecahedronFace(points[4],  points[17], points[8],  points[15], points[14], GL_POLYGON);
        DrawDodecahedronFace(points[4],  points[12], points[5],  points[16], points[17], GL_POLYGON);

        glColor4f(lineColour.GetRed(), lineColour.GetGreen(), lineColour.GetBlue(), lineColour.GetAlpha());
        DrawDodecahedronFace(points[0],  points[1],  points[9],  points[16], points[5],  GL_LINE_LOOP);
        DrawDodecahedronFace(points[1],  points[0],  points[3],  points[18], points[7],  GL_LINE_LOOP);
        DrawDodecahedronFace(points[1],  points[7],  points[11], points[10], points[9],  GL_LINE_LOOP);
        DrawDodecahedronFace(points[11], points[7],  points[18], points[19], points[6],  GL_LINE_LOOP);
        DrawDodecahedronFace(points[8],  points[17], points[16], points[9],  points[10], GL_LINE_LOOP);
        DrawDodecahedronFace(points[2],  points[14], points[15], points[6],  points[19], GL_LINE_LOOP);
        DrawDodecahedronFace(points[2],  points[13], points[12], points[4],  points[14], GL_LINE_LOOP);
        DrawDodecahedronFace(points[2],  points[19], points[18], points[3],  points[13], GL_LINE_LOOP);
        DrawDodecahedronFace(points[3],  points[0],  points[5],  points[12], points[13], GL_LINE_LOOP);
        DrawDodecahedronFace(points[6],  points[15], points[8],  points[10], points[11], GL_LINE_LOOP);
        DrawDodecahedronFace(points[4],  points[17], points[8],  points[15], points[14], GL_LINE_LOOP);
        DrawDodecahedronFace(points[4],  points[12], points[5],  points[16], points[17], GL_LINE_LOOP);

        glEnable(GL_TEXTURE_2D);
    }

    void DrawSphere(const ColourRGBA& colour)
    {
        const unsigned int SPHERE_LATS = 10;
        const unsigned int SPHERE_LONGS = 10;

        glDisable(GL_TEXTURE_2D);
        glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

        for (unsigned int i = 0; i < SPHERE_LATS; i++) {
            const float lat0 = static_cast<float>(M_PI) * (-0.5f + static_cast<float>(i - 1) / static_cast<float>(SPHERE_LATS));
            const float z0 = sinf(lat0);
            const float zr0 = cosf(lat0);

            const float lat1 = static_cast<float>(M_PI) * (-0.5f + static_cast<float>(i) / static_cast<float>(SPHERE_LATS));
            const float z1 = sinf(lat1);
            const float zr1 = cosf(lat1);

            glBegin(GL_QUAD_STRIP);
            for(unsigned int j = 0; j <= SPHERE_LONGS; j++) {
                const float lng = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j - 1) / static_cast<float>(SPHERE_LONGS);
                const float x = cosf(lng);
                const float y = sinf(lng);

                glVertex3f(x * zr0, y * zr0, z0);
                glVertex3f(x * zr1, y * zr1, z1);
            }
            glEnd();
        }

        glEnable(GL_TEXTURE_2D);
    }
}
