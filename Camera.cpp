#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>
#include "Camera.h"
#include "App.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        glm::mat4 projection = glm::perspective(
                                1.30899694f, 
                                static_cast<float>(APP.GetScreenWidth()) /
                                static_cast<float>(APP.GetScreenHeight()),
                                0.1f, 10000.0f);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glm::vec3 origin(m_origin[0], m_origin[1], m_origin[2]);
        glm::vec3 center(m_lookat[0], m_lookat[1], m_lookat[2]);
        glm::vec3 up(m_up[0], m_up[1], m_up[2]);
        glm::mat4 lookat = glm::lookAt(origin, center, up);
        glLoadMatrixf(glm::value_ptr(lookat));

        glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&m_modelview);
        glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat *)&m_projection);
    }


    const juzutil::Vector2 Camera::PerspectiveProject(const juzutil::Vector3& worldCoords) const
    {
        int view[4];
        glGetIntegerv(GL_VIEWPORT, view);

        glm::vec3 obj(worldCoords[0], worldCoords[1], worldCoords[2]);
        glm::vec3 coords = glm::project(obj,
                                        m_modelview,
                                        m_projection, 
                                        glm::vec4(view[0],
                                                  view[1],
                                                  view[2],
                                                  view[3]));
    
        return juzutil::Vector2(
            coords.x, static_cast<float>(APP.GetScreenHeight()) - coords.y);
    }


    void Camera::ApplyOrtho() const
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, APP.GetScreenWidth(), APP.GetScreenHeight(),
                0, -1.0f, 1.0f);

        glMatrixMode(GL_MODELVIEW_MATRIX);
    }
}
