#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "Camera.h"

namespace typing
{
    Camera::Camera(const juzutil::Vector3& origin, const juzutil::Vector3& lookat)
        : m_origin(origin), m_lookat(lookat)
    {
        m_forward = (lookat - origin);
        m_forward.Normalize();

        if (m_forward[0] == 1.0f || m_forward[0] == -1.0f) {
            m_right.Set(0.0f, -m_forward[0], 0.0f);     
        } else if (m_forward[1] == 1.0f || m_forward[1] == -1.0f) {
            m_right.Set(0.0f, m_forward[1], 0.0f);
        } else if(m_forward[2] == 1.0f || m_forward[2] == -1.0f) {
            m_right.Set(1.0f, 0.0f, 0.0f);
        } else {
            m_right = m_forward.GetMajorAxis() % m_forward;
            m_right.Normalize();
        }
       
        m_up = m_forward % m_right;
        m_up.Normalize();
    }


    void Camera::ApplyPerspective()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(75.0, 800.0 / 600.0, 0.1f, 10000.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(m_origin[0], m_origin[1], m_origin[2],
                  m_lookat[0], m_lookat[1], m_lookat[2],
                  m_up[0], m_up[1], m_up[2]);

        glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
    }


    const juzutil::Vector2 Camera::PerspectiveProject(const juzutil::Vector3& worldCoords) const
    {
        double x, y, z;
        int view[4];

        glGetIntegerv(GL_VIEWPORT, view);

        gluProject(worldCoords[0], worldCoords[1], worldCoords[2],
            m_modelview, m_projection, view, &x, &y, &z);
    
        return juzutil::Vector2(static_cast<float>(x), static_cast<float>(600 - y));
    }


    void Camera::ApplyOrtho() const
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // TODO: Get rid of magic numbers
        glOrtho(0, 800, 600, 0, -1.0f, 1.0f);

        glMatrixMode(GL_MODELVIEW_MATRIX);
    }
}
