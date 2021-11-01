#ifndef BULLET_HPP_
#define BULLET_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

#include <vector>
#include <list>

class OpenGLWindow;
class Ship;

class Bullet {
    public:
        void initializeGL(GLuint program, float shipMaxY);
        void paintGL(
            glm::vec2 shipTranslation,
            float deltaTime,
            bool spacebarActive
        );
        void terminateGL();

    private:
        friend OpenGLWindow;
        friend Ship;

        GLuint m_program{};
        GLuint m_vboPositions{};
        GLuint m_vao{};

        //uniform locations
        GLint m_translationLoc{};
        GLint m_shipPositionLoc{};

        // uniform values
        glm::vec2 m_translation{glm::vec2(0.0f, 0.0f)};
        glm::vec2 m_shipPosition{};

        //generic start position to draw
        //inicializado em initializeGL
        glm::vec2 m_startPosition{}; // = (0, ponta da nave (isto e, yMax))

        //bullet tracker list
        std::list<glm::vec2> m_bulletQueue {};

        //timer
        abcg::ElapsedTimer m_timer;
};

#endif
