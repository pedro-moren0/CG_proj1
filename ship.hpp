#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"
#include "bullet.hpp"

#include <vector>

class OpenGLWindow;

class Ship {
    public:
        void initializeGL(GLuint program);
        void paintGL(const GameData &gameData, float deltaTime);
        void terminateGL();

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLuint m_vboPositions{};
        GLuint m_vboBodyColors{};
        GLuint m_eboBody{};
        GLuint m_vaoBody{};

        GLuint m_vboLinesColors{};
        GLuint m_eboLines{};
        GLuint m_vaoLines{};

        //uniform locations
        GLint m_startPositionLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        //uniform values
        glm::vec2 m_startPosition{glm::vec2(0.0f, -0.95f)};
        glm::vec2 m_translation{glm::vec2(0)};
        float m_scale{0.125f};

        //useful
        glm::vec2 m_minXYpos {};
        glm::vec2 m_maxXYpos {};

        Bullet m_bullet;
        bool m_spacebarStatus {};
        // BulletState m_bulletState;
};

#endif
